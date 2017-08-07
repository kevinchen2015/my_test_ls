#include "game_world_client.h"
#include "frame_time.h"
#include "game_predict.h"
#include "frame_cmd.h"
#include "game_input.h"
#include "game_system.h"
#include "ecs_entity.h"

GameWorldClient::GameWorldClient(int cmd_max)
	:GameWorld(cmd_max)
{
	predict_systems_ = new GameSystem();
	game_predict_ = new GamePredict();
	time_couter = 0;
	predict_start_ = false;
	wait_frame_id_ = 0;
}

GameWorldClient::~GameWorldClient()
{
	delete game_predict_;
	delete predict_systems_;

	for (auto kv : remote_inputs_)
	{
		delete (kv.second);
	}
	remote_inputs_.clear();
}

void GameWorldClient::OnFixedUpdate(FrameTime* game_time)
{
	GameWorld::OnFixedUpdate(game_time);

	//clear all cmd
	for (auto kv : remote_inputs_)
	{
		(kv.second)->ClearAllCmd();
	}

	if (game_time->GetFrameID() == 1)
	{
		predict_start_ = true;
		time_couter = real_time_->GetDelta() * 2;
		(game_predict_->GetFrameTime())->SetFrameTime(real_time_->GetFrameID() * 2, time_couter);
	}
}


void GameWorldClient::OnLocalFixedUpdate(unsigned int time, int delta)
{
	if (!predict_start_) return;

	//Ԥ�����
	int old_time = time_couter;
	time_couter += delta;
	unsigned int frameId = time_couter / FIXED_UPDATE_MS;
	
	//��һ�£��Ȼع�
	if (game_predict_->predict_conflict_
		)
	{
		game_predict_->predict_conflict_ = false;
		RollPredictFrame();
	}

	

	int frame_step = frameId - game_predict_->predict_time_.GetFrameID();

	//��̬����Ԥ��ķ���,����Ǹ��������Ҫ�ȼ�֡
	if (wait_frame_id_ > 0)
	{
		for (int i = 0; i < frame_step; ++i)
		{
			wait_frame_id_ -= 1;
			time_couter -= FIXED_UPDATE_MS;
			if (wait_frame_id_ == 0)
			{
				break;
			}
		}
	
		if (wait_frame_id_ < 0)
		{
			return;
		}

		if (time_couter <= old_time)
		{
			return;
		}

		//������ʱ���ˣ���Ҫ���¼������֡��
		frameId = time_couter / FIXED_UPDATE_MS;
	}
	else
	{
		if (frameId > real_time_->GetFrameID() + PREDICT_MAX)
		{
			wait_frame_id_ = PREDICT_MAX - 3; //�ȸ���֡
			time_couter = old_time;
			return;
		}
	}

	int frame_dis = game_predict_->predict_cmd_cache_.frame_dis_;
	game_predict_->predict_cmd_cache_.frame_dis_ = 0;

	if (frame_dis != 0)
	{
		if (frame_dis < 0)
		{
			//�ȴ���֡
			wait_frame_id_ = (-frame_dis);

			return;
		}
		else
		{
			//�ӿ켸֡
			time_couter = time_couter + real_time_->GetDelta() * frame_dis;
			(game_predict_->GetFrameTime())->SetFrameTime(real_time_->GetFrameID() + frame_dis, time_couter);
			//������ʱ���ˣ���Ҫ���¼������֡��
			frameId = time_couter / FIXED_UPDATE_MS;
		}
	}

	//����Ԥ��
	frame_step = frameId - game_predict_->predict_time_.GetFrameID();
	for (int i = 0; i < frame_step; ++i)
	{
		PredictOneFrame();
	}
}

void GameWorldClient::RollPredictFrame()
{
	int preict_frame_id = game_predict_->predict_time_.GetFrameID();
	int real_frame_id = real_time_->GetFrameID();

	//���ݽ���Ѿ����µ�real_frame_id ִ����֮���״̬,����һ֡���¿�ʼģ��
	(game_predict_->predict_time_).SetFrameTime(real_time_->GetFrameID(), real_time_->GetTime());

	//����cache��� С�ڵ���real_frame_id
	game_predict_->ClearInputCache(real_time_->GetFrameID());

	//restore data,logic and display
	predict_systems_->RollBack((game_predict_->predict_time_).GetTime());

	int frame_step = preict_frame_id - real_frame_id;
	frame_step = frame_step > PREDICT_MAX ? PREDICT_MAX : frame_step;
	for (int i = 0; i < frame_step ; ++i)
	{
		//fixedupdate
		RollforthOneFrame();
	}
	//todo...update display once!
}

void GameWorldClient::RollforthOneFrame()
{
	game_predict_->predict_time_.Increase(FIXED_UPDATE_MS);

	//restor input cmd from cache
	game_predict_->RestorHandleInputFromCache();

	//fixedupdate logic
	PredictFixedUpdate();
}

void GameWorldClient::PredictOneFrame()
{
	game_predict_->predict_time_.Increase(FIXED_UPDATE_MS);

	//read cmd from predict input,(�ⲿ����д��)
	game_predict_->HandleInput();

	//fixedupdate logic
	PredictFixedUpdate();

	//send cmd on frame end
	game_predict_->SyncInput();

	//rest input
	game_predict_->ResetInput();
}

void GameWorldClient::PredictFixedUpdate()
{
	predict_systems_->OnFixedUpdate((game_predict_->predict_time_).GetTime(), (game_predict_->predict_time_).GetDelta());
}

void GameWorldClient::OnLocalUpdate(unsigned int time, int delta)
{
	if (!predict_start_) return;

	if (wait_frame_id_ < 0)
	{
		return;
	}

	//todo
	//predict_systems_->OnUpdate()
}

void GameWorldClient::OnRecvNetCmd(FrameCmd* cmd)
{
	//ͨ����������ͬ������
	UID uid = cmd->cmd->uid;
	GameInput* input = GetInput(uid);
	input->PushCmd(cmd->frame_id, cmd->cmd);
	this->HandleCmd(cmd);

	//����Ԥ��У��
	game_predict_->OnRecivCmdBack(cmd);
}

GameInput* GameWorldClient::GetInput(UID uid)
{
	auto iter = remote_inputs_.find(uid);
	GameInput* input = nullptr;
	if (iter == remote_inputs_.end())
	{
		input = new GameInput;
		input->Reset();
		input->uid_ = uid;
		remote_inputs_[uid] = input;
	}
	else
	{
		input = iter->second;
	}
	return input;
}

void GameWorldClient::AddEntity(ecs::Entity* entity)
{
	if (entity->GetTagID() | PREDICT_TAG)
	{
		game_predict_->predict_entity_ = entity;
	}
	GameWorld::AddEntity(entity);
	predict_systems_->OnEntityEnter(entity);
}

void GameWorldClient::RemoveEntity(ecs::Entity* entity)
{
	GameWorld::RemoveEntity(entity);
	predict_systems_->OnEntityExit(entity);

	if (entity->GetTagID() | PREDICT_TAG)
	{
		game_predict_->predict_entity_ = nullptr;
	}
}

UID GameWorldClient::GetPredictUID()
{
	return game_predict_->predict_input_.uid_;
}