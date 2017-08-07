#include "game_predict.h"
#include "move_component.h"
#include "ecs_entity.h"

GamePredict::GamePredict()
{
	predict_input_.Reset();
	predict_conflict_ = false;
	predict_entity_ = nullptr;
}


void GamePredict::ResetUID(UID id)
{
	predict_input_.Reset();
	predict_input_.uid_ = id;
}

void GamePredict::OnRecivCmdBack(FrameCmd* cmd)
{
	if (predict_input_.uid_ == cmd->cmd->uid)
	{
		predict_conflict_ = predict_cmd_cache_.Conflict(cmd);
	}
}

void GamePredict::RestorHandleInputFromCache()
{
	auto cmd_list = predict_cmd_cache_.GetCmdList();
	for (auto cmd : cmd_list)
	{
		if (cmd->frame_id == predict_time_.GetFrameID())
		{
			_HandleOneInput(cmd);
		}
	}
}

void GamePredict::ResetInput()
{
	predict_input_.ClearAllCmd();
}

void GamePredict::_HandleOneInput(GameInput::InputCmd* cmd)
{
	if (GAME_MOVE_CMD == cmd->cmd->id)
	{
		MoveCmd* move_cmd = (MoveCmd*)cmd->cmd;
		move_cmd->entity_id = predict_entity_->GetEntityID();

		//handle move
		MovePredictComponent* move_predict = predict_entity_->GetComponent<MovePredictComponent>();
		handle_predict_move_cmd(move_cmd, move_predict);
	}
}

void GamePredict::HandleInput()
{
	for (int i = 0; i < GAME_CMD_NUM; ++i)
	{
		GameInput::InputCmd& cmd = predict_input_.GetInputCmd(i);
		if (cmd.cmd == nullptr) continue;

		if (cmd.cmd->id != 0)
		{
			cmd.frame_id = predict_time_.GetFrameID();
			cmd.cmd->seq_id = predict_input_.GetNextSeqID();
			cmd.cmd->uid = predict_input_.uid_;

			_HandleOneInput(&cmd);
		}
		
	}
}

void GamePredict::SyncInput()
{
	//todo...预处理发包，如果只有移动包，且上一帧就是相同的移动状态，则可以不发

	predict_input_.InputCmdOutput(&predict_time_, predict_sync_out_);
	
	//push to cache
	for (int i = 0; i < GAME_CMD_NUM; ++i)
	{
		GameInput::InputCmd& cmd = predict_input_.GetInputCmd(i);
		if (cmd.cmd == nullptr) continue;

		if (cmd.cmd->id != 0)
		{
			predict_cmd_cache_.PushInput(&cmd);
		}
	}
}

void GamePredict::ClearInputCache(FrameID frame_id)
{
	predict_cmd_cache_.ClearCmdCache(frame_id);
}

