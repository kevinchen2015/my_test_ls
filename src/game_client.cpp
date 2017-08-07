
#include "game_client.h"
#include "frame_world_slave.h"
#include "frame_cmd.h"
#include "game_cmd.h"
#include "game_msg_helper.h"
#include "game_input.h"
#include "game_predict.h"
#include "game_world_client.h"
#include "game_system.h"
#include "move_system.h"
#include "actor_helper.h"

#include <iostream>
#include <assert.h>

void GameClient::c_on_recv_data(char* buff, unsigned int size, void* conn, int channel, void* user_data)
{
	GameClient* client = static_cast<GameClient*>(user_data);

	if (client->status_ == GAME_WAIT_READY && channel == TCP)
	{
		if (size == 4)
		{
			int conv = 0;
			memcpy(&conv, buff, 4);

			client->game_world_->game_predict_->ResetUID(conv);
			client->frame_world_->Start();
			
			sl_client_switch_kcp(client->net_client_, conv);
			return;
		}
	}

	if (client->status_ != GAME_RUNNING)return;

	//todo parse net msg
	ParseMsg(buff,size, nullptr,client->cmd_queue_in_);
}

void GameClient::c_on_connected(void* conn, int status, int channel, void* user_data)
{
	GameClient* client = static_cast<GameClient*>(user_data);

	if (channel == TCP && status == 0)
	{
		int conv = 1;  //just for test!
		sl_client_send_data(client->net_client_, (char*)&conv, 4);
	}

	if (client->status_ == GAME_WAIT_READY && channel == KCP && status == 0)
	{
		client->status_ = GAME_RUNNING;
	}
}

void GameClient::c_on_close(void* conn, int channel, void* user_data)
{
	GameClient* client = static_cast<GameClient*>(user_data);
}

//----------------------------------------------------------------------

GameClient::GameClient()
{
	
	frame_end_ = false;
	cmd_queue_in_ = new FrameCmdQueue();
	cmd_queue_out_ = new FrameCmdQueue();
	frame_world_ = new FrameWorldSlave(GAME_CMD_NUM,cmd_queue_in_);

	game_world_ = new GameWorldClient(GAME_CMD_NUM);
	game_world_->SetRealFrameTime(frame_world_->GetFrameTime());
	game_world_->game_predict_->predict_sync_out_ = cmd_queue_out_;

	frame_world_->on_pre_frame = std::bind(&GameClient::OnPreFrame, this);
	frame_world_->on_post_frame = std::bind(&GameClient::OnPostFrame, this);

	frame_world_->RegistHandle(FRAME_START_CMD, std::bind(&GameClient::FrameStartHandle, this, std::placeholders::_1));
	frame_world_->RegistHandle(FRAME_END_CMD, std::bind(&GameClient::FrameEndHandle, this, std::placeholders::_1));
	frame_world_->RegistHandle(GAME_MOVE_CMD, std::bind(&GameClient::InputCmdHandle, this, std::placeholders::_1));
	frame_world_->RegistHandle(GAME_CREATE_ENTITY_CMD, std::bind(&GameClient::CreateEntityHandle, this, std::placeholders::_1));

	sl_client_create(&net_client_, this);
	sl_client_cb(net_client_, c_on_connected, c_on_recv_data, c_on_close);
	
	status_ = GAME_INIT;


	GameSystem* system = game_world_->GetSystem();
	system->AddSystem(new MoveSystem());
	game_world_->RegistHandler(GAME_MOVE_CMD, std::bind(handle_move_cmd, std::placeholders::_1, std::placeholders::_2));

	game_world_->GetPredictSystem()->AddSystem(new MovePredictSystem());

}

GameClient::~GameClient()
{
	delete game_world_;
	delete frame_world_;
	delete cmd_queue_in_;
	delete cmd_queue_out_;

	sl_client_shutdown(net_client_);
	sl_client_release(net_client_);
}

void GameClient::Start()
{
	status_ = GAME_WAIT_READY;
	sl_client_init(net_client_, "127.0.0.1", 6000, 6668);
	sl_client_connect(net_client_);
}

void GameClient::OnFixedUpdate(unsigned int time, int delta)
{
	sl_client_update(net_client_);
	frame_world_->OnFixedUpdate(time, delta);
	game_world_->OnLocalFixedUpdate(time, delta);

	if (cmd_queue_out_->GetSize() > 0)
	{
		//·¢ËÍÔ¤²âÊäÈë
		char* buf = nullptr;
		unsigned int size = 0;
		PackMsg(cmd_queue_out_,buf,size);
		cmd_queue_out_->Clear();

		if(size > 0)
			sl_client_send_data(net_client_, buf, size);
	}
}

void GameClient::OnUpdate(unsigned int time, int delta)
{
	game_world_->OnLocalUpdate(time, delta);
}

void GameClient::OnPreFrame()
{
	frame_end_ = false;
	//cmd_queue_out_->Clear();
}

void GameClient::OnPostFrame()
{
	game_world_->OnFixedUpdate(frame_world_->GetFrameTime());
	frame_end_ = true;
}

void GameClient::FrameStartHandle(FrameCmd* cmd)
{
	//if(cmd->frame_id % 15 == 1)
	//	std::cout <<"frame start cmd,id:"<<cmd->frame_id << " time:"<< cmd->time << std::endl;
}

void GameClient::FrameEndHandle(FrameCmd* cmd)
{
	if (cmd->frame_id % 15 == 1)
	{
		//	std::cout << "frame end cmd,id:" << cmd->frame_id << " time:" << cmd->time << std::endl;
		//	std::cout << "predict end cmd,id:" << game_world_->game_predict_->GetFrameTime()->GetFrameID() << " time:" << game_world_->game_predict_->GetFrameTime()->GetTime() << std::endl;
	
		FrameID real_frame_id = frame_world_->GetFrameTime()->GetFrameID();
		FrameID predict_frame_id = game_world_->game_predict_->GetFrameTime()->GetFrameID();
		int frame_dis = predict_frame_id - real_frame_id;
		std::cout <<"frame dis :"<< frame_dis << " predict frame id:" << predict_frame_id << " real frame id :" << real_frame_id << std::endl;
	}
}

void GameClient::InputCmdHandle(FrameCmd* cmd)
{
	//std::cout << "move cmd,frame_id:"<< cmd->frame_id << std::endl;
	game_world_->OnRecvNetCmd(cmd);

}

void GameClient::CreateEntityHandle(FrameCmd* cmd)
{
	CreateEntityCmd* create_cmd = (CreateEntityCmd*)cmd->cmd;

	if (create_cmd->type == ENTITY_ACTOR)
	{
		bool predict = create_cmd->uid == game_world_->GetPredictUID();
		ecs::Entity* entity = ActorHelper::CreateClientActor(create_cmd->entity_id, predict);
		entity->user_data_ = game_world_->GetInput(create_cmd->uid);
		entity->user_data2_ = game_world_->game_predict_->GetPredictInput();
		game_world_->AddEntity(entity);
	}
}



