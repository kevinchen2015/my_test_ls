#include "game_server.h"
#include "frame_world_master.h"
#include "frame_cmd.h"
#include "frame_time.h"
#include "game_cmd.h"
#include "game_remote.h"
#include "game_msg_helper.h"
#include "game_world.h"
#include "move_system.h"
#include "game_system.h"
#include "actor_helper.h"

#include <iostream>


void GameServer::s_on_recv_data(char* buff, unsigned int size, void* conn, int channel, void* user_data)
{
	GameServer* svr = static_cast<GameServer*>(user_data);

	if (channel == TCP && GAME_WAIT_READY == svr->status_)
	{
		if (size == 4)
		{
			int conv = ++svr->uid_counter_;
			memcpy(&conv, buff, 4);

			sl_server_set_conv(svr->net_server_, conn, conv);
			sl_server_send_data(svr->net_server_, conn, (char*)&conv, 4);

			GameRemote* remote = (GameRemote*)sl_server_get_conn_user_data(conn);
			if (remote)
			{
				remote->remote_input_.uid_ = conv;
				svr->game_remotes_[remote->remote_input_.uid_] = remote;
			}
			return;
		}
	}

	if (svr->status_ != GAME_RUNNING)return;

	//todo parse net msg
	void* remote = sl_server_get_conn_user_data(conn);
	ParseMsg(buff, size, remote,svr->cmd_queue_in_);
}


void GameServer::s_on_connected(void* conn, int status, int channel, void* user_data)
{
	GameServer* svr = static_cast<GameServer*>(user_data);

	if (channel == TCP)
	{
		GameRemote* remote = new GameRemote();
		remote->conn_ = conn;
		sl_server_set_conn_user_data(conn, remote);
		svr->game_remote_list_.push_back(remote);
	}

	if (channel == KCP)
	{
		svr->Start();
	}
}

void GameServer::s_on_close(void* conn, int channel, void* user_data)
{
	GameServer* svr = static_cast<GameServer*>(user_data);

	int channel_mode = sl_server_get_channel_mode(svr->net_server_, conn);
	if (channel == channel_mode)
	{
		GameRemote* remote = (GameRemote*)sl_server_get_conn_user_data(conn);
		if (remote)
		{
			sl_server_set_conn_user_data(conn, nullptr);
			svr->game_remote_list_.remove(remote);
			if (remote->remote_input_.uid_ > 0)
			{
				svr->game_remotes_.erase(remote->remote_input_.uid_);
			}
			delete remote;
		}
	}
}

//------------------------------------------------------------------------------

GameServer::GameServer()
{
	uid_counter_ = 0;

	frame_end_ = false;
	cmd_queue_in_ = new FrameCmdQueue();
	cmd_queue_out_ = new FrameCmdQueue();

	frame_world_ = new FrameWorldMaster(GAME_CMD_NUM,cmd_queue_in_);

	frame_world_->on_pre_frame = std::bind(&GameServer::OnPreFrame,this);
	frame_world_->on_post_frame = std::bind(&GameServer::OnPostFrame,this);
	frame_world_->preprocess_callback = std::bind(&GameServer::PreprocessCmd, this, std::placeholders::_1, std::placeholders::_2);

	frame_world_->RegistHandle(GAME_MOVE_CMD, std::bind(&GameServer::InputCmdHandle, this, std::placeholders::_1));

	//net
	sl_server_create(&net_server_,this);
	sl_server_cb(net_server_, s_on_connected, s_on_recv_data, s_on_close);
	sl_server_start(net_server_, "127.0.0.1", 6000, 6668, 1024);

	status_ = GAME_INIT;
	status_ = GAME_WAIT_READY;

	game_world_ = new GameWorld(GAME_CMD_NUM);

	//add system
	GameSystem* system = game_world_->GetSystem();
	system->AddSystem(new MoveSystem());
	game_world_->RegistHandler(GAME_MOVE_CMD, std::bind(handle_move_cmd, std::placeholders::_1, std::placeholders::_2));


}

GameServer::~GameServer()
{
	delete game_world_;
	delete frame_world_;
	delete cmd_queue_in_;
	delete cmd_queue_out_;

	for (auto i : game_remote_list_)
	{
		delete i;
	}

	sl_server_shutdown(net_server_);
	sl_server_release(net_server_);
}

void GameServer::Start()
{
	entity_id_counter_ = 0;
	frame_world_->Start();
	status_ = GAME_RUNNING;
}

void GameServer::OnFixedUpdate(unsigned int time, int delta)
{
	sl_server_update(net_server_);
	frame_world_->OnFixedUpdate(time, delta);
}

GameRemote* GameServer::GetRemote(UID uid)
{
	auto iter = game_remotes_.find(uid);
	if (iter != game_remotes_.end())
		return iter->second;
	return nullptr;
}

void GameServer::OnPreFrame()
{
	frame_end_ = false;
	//cmd_queue_out_->Clear();

	//frame start
	FrameCmd* newCmd = new FrameCmd();
	FrameTime* frame_time = frame_world_->GetFrameTime();
	newCmd->frame_id = frame_time->GetFrameID();
	newCmd->time = frame_time->GetTime();
	newCmd->cmd_len = sizeof(Cmd);
	newCmd->cmd = (Cmd*)malloc(newCmd->cmd_len);
	newCmd->cmd->id = FRAME_START_CMD;
	cmd_queue_out_->PushFrameCmd(newCmd);

	//test
	if (frame_time->GetFrameID() == 1)
	{
		//create test entity
		for (auto iter : game_remotes_)
		{
			UID uid = (iter.second)->remote_input_.uid_;

			//sync
			FrameCmd* newCmd = new FrameCmd();
			FrameTime* frame_time = frame_world_->GetFrameTime();
			newCmd->frame_id = frame_time->GetFrameID();
			newCmd->time = frame_time->GetTime();
			newCmd->cmd_len = sizeof(CreateEntityCmd);
			CreateEntityCmd* cmd = (CreateEntityCmd*)malloc(newCmd->cmd_len);
			newCmd->cmd = cmd;

			cmd->id = GAME_CREATE_ENTITY_CMD;
			cmd->entity_id = ++entity_id_counter_;
			cmd->type = ENTITY_ACTOR;
			cmd->res_id = 0;
			cmd->uid = uid;

			cmd_queue_out_->PushFrameCmd(newCmd);

			//create acotr
			ecs::Entity* entity = ActorHelper::CreateServerActor(cmd->entity_id);
			entity->user_data_ = &GetRemote(cmd->uid)->remote_input_;
			game_world_->AddEntity(entity);

		}
	}
}

void GameServer::OnPostFrame()
{
	frame_end_ = true;

	//update game world
	game_world_->OnFixedUpdate(frame_world_->GetFrameTime());

	//add game input
	for (auto c : game_remotes_)
	{
		(c.second)->remote_input_.InputCmdOutput(frame_world_->GetFrameTime(), cmd_queue_out_);
		(c.second)->remote_input_.ClearAllCmd();
	}

	//frame end
	FrameCmd* newCmd = new FrameCmd();
	FrameTime* frame_time = frame_world_->GetFrameTime();
	newCmd->frame_id = frame_time->GetFrameID();
	newCmd->time = frame_time->GetTime();
	newCmd->cmd_len = sizeof(Cmd);
	newCmd->cmd = (Cmd*)malloc(newCmd->cmd_len);
	newCmd->cmd->id = FRAME_END_CMD;
	cmd_queue_out_->PushFrameCmd(newCmd);

	//cmd_queue_out_->AdjustFrameTime(frame_time);

	//combin cmd
	char* buf = nullptr;
	unsigned int size = 0;
	PackMsg(cmd_queue_out_, buf, size);
	cmd_queue_out_->Clear();

	//brocast cmd
	Broadcast(buf, size);
}

void GameServer::PreprocessCmd(FrameCmdQueue* queue, FrameTime* time)
{
	for (FrameCmd* cmd : queue->GetCmdList())
	{
		//为调试，记录客户端上报帧,不使用该数据在逻辑里
		GameRemote* remote = static_cast<GameRemote*>(cmd->user_data);
		if (remote)
		{
			if (cmd->frame_id > remote->remote_frame_time_.GetFrameID())
			{
				remote->remote_frame_time_.SetFrameTime(cmd->frame_id, cmd->time);
			}
		}

		//为兼容网络抖动，对于先于服务端帧数的包进行cache,控制范围，其他当帧处理,
		if (cmd->frame_id > time->GetFrameID()
			&& cmd->frame_id < time->GetFrameID() + PREDICT_MAX
			)
		{
			//cache
			//std::cout << " svr_frame_id:" << time->GetFrameID();
			//std::cout << " cmd_frame_id:" << cmd->frame_id;
			//std::cout << " cmd_seq id:" << cmd->cmd->seq_id << std::endl;
		}
		else
		{
			//to handle
			cmd->frame_id = time->GetFrameID();
			cmd->time = time->GetTime();
		}
	}
}

void GameServer::Broadcast(char* buf, unsigned int size)
{
	if (size > 0)
	{
		for (auto c : game_remotes_)
		{
			sl_server_send_data(net_server_, (c.second)->conn_, buf, size);
		}
	}
}

//--------------------------------------------------------------------

void GameServer::InputCmdHandle(FrameCmd* cmd)
{
	//通用输入请求同步处理
	GameRemote* remote = (GameRemote*)cmd->user_data;
	if (remote)
	{
		remote->remote_input_.PushCmd(cmd->frame_id, cmd->cmd);
		game_world_->HandleCmd(cmd);
	}
}




