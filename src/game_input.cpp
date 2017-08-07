#include "game_input.h"
#include "frame_cmd.h"
#include <assert.h>
#include <iostream>

GameInput::GameInput()
{
	//new client request game cmd
	Reset();

	input_cmd_[GAME_MOVE_CMD].cmd = new MoveCmd();
	input_cmd_[GAME_MOVE_CMD].cmd_len = sizeof(MoveCmd);
}

GameInput::~GameInput()
{

}

void GameInput::InputCmd::Reset()
{
	frame_id = 0;
	if (cmd)
	{
		memset(cmd, 0, cmd_len);//!!!!注意， cmd为 扁平结构，必须无虚函数
	}
}

void GameInput::Reset()
{
	uid_ = 0;
	seq_id_ = 0;

	for (int i = 0; i < GAME_CMD_NUM; ++i)
	{
		input_cmd_[i].Reset();
	}
}

void GameInput::PushCmd(FrameID frame_id, Cmd* cmd)
{
	assert(cmd->id < GAME_CMD_NUM);
	InputCmd& input = input_cmd_[cmd->id];

	if (frame_id >= input.frame_id)
	{
		if (seq_id_ < cmd->seq_id)
			seq_id_ = cmd->seq_id;

		input.frame_id = frame_id;
		memcpy(input.cmd, cmd, input.cmd_len);  //!!!!注意， cmd为 扁平结构，必须无虚函数
	}
}

void GameInput::InputCmdOutput(FrameTime* time, FrameCmdQueue* out_queue)
{
	for (int i = 0; i < GAME_CMD_NUM; ++i)
	{
		InputCmd& input = input_cmd_[i];
		if (input.cmd && input.frame_id == time->GetFrameID())
		{
			FrameCmd* cmd_cmd = new FrameCmd();
			cmd_cmd->frame_id = time->GetFrameID();
			cmd_cmd->time = time->GetTime();
			cmd_cmd->cmd = (Cmd*)malloc(input.cmd_len);
			cmd_cmd->cmd_len = input.cmd_len;
			memcpy(cmd_cmd->cmd, input.cmd, input.cmd_len);
			out_queue->PushFrameCmd(cmd_cmd);
		}
	}
}

void GameInput::ClearAllCmd()
{
	for (int i = 0; i < GAME_CMD_NUM; ++i)
	{
		input_cmd_[i].Reset();
	}
}


//-----------------------------------------------------

GameInputCache::GameInputCache()
{
	frame_dis_ = 0;
}

void GameInputCache::PushInput(GameInput::InputCmd* cmd)
{
	GameInput::InputCmd* new_cmd = new GameInput::InputCmd();
	new_cmd->frame_id = cmd->frame_id;
	new_cmd->cmd_len = cmd->cmd_len;
	new_cmd->cmd = (Cmd*)malloc(cmd->cmd_len);
	memcpy(new_cmd->cmd, cmd->cmd, cmd->cmd_len);

	input_cache_.push_back(new_cmd);
}

std::list<GameInput::InputCmd*>& GameInputCache::GetCmdList()
{
	return input_cache_;
}


bool compare_buff_concent(char* src, char* des, size_t size)
{
	for (int i = 0; i < size; ++i)
	{
		if (src[i] != des[i])
			return false;
	}
	return true;
}

bool GameInputCache::Conflict(FrameCmd* svr_cmd)
{
	bool conflict = false;
	auto iter = input_cache_.begin();
	for (; iter != input_cache_.end();)
	{
		GameInput::InputCmd* cmd = *iter;
		//比较流水号，流水小了，说明跳过了
		if (cmd->cmd->seq_id < svr_cmd->cmd->seq_id)
		{
			//conflict = true;
			free(cmd->cmd);
			delete cmd;
			iter = input_cache_.erase(iter);
			continue;
		}

		//if (conflict)
		//	return true;

		//比较命令
		if (cmd->frame_id == svr_cmd->frame_id
			&& compare_buff_concent((char*)cmd->cmd, (char*)svr_cmd->cmd, svr_cmd->cmd_len))
		{
			frame_dis_ = 0;

			free(cmd->cmd);
			delete cmd;
			iter = input_cache_.erase(iter);
			return false;
		}

		frame_dis_ = svr_cmd->frame_id - cmd->frame_id;
		std::cout << "frame_dis : " << frame_dis_;
		std::cout << " svr_frame_id:" << svr_cmd->frame_id;
		std::cout << " svr_seq id:" << svr_cmd->cmd->seq_id;
		std::cout << " predict_frame_id:" << cmd->frame_id;
		std::cout << " predict_seq id:" << cmd->cmd->seq_id << std::endl;

		if (cmd->cmd->seq_id == svr_cmd->cmd->seq_id)
		{
			free(cmd->cmd);
			delete cmd;
			iter = input_cache_.erase(iter);
			return true;
		}
		return true;
	}
	return true;
}

void GameInputCache::ClearCmdCache(FrameID frame_id)
{
	auto iter = input_cache_.begin();
	for (; iter != input_cache_.end();)
	{
		GameInput::InputCmd* cmd = *iter;
		if (cmd->frame_id <= frame_id)
		{
			iter = input_cache_.erase(iter);
			free(cmd->cmd);
			delete cmd;
		}
		else
		{
			++iter;
		}
	}
}



