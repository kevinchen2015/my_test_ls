#include "frame_world_master.h"
#include "frame_cmd.h"
#include <assert.h>

FrameWorldMaster::FrameWorldMaster(int cmd_max,FrameCmdQueue* queue)
	:FrameWorld(cmd_max,queue)
{
	time_couter = 0;
	preprocess_callback = nullptr;
}

FrameWorldMaster::~FrameWorldMaster()
{

}

void FrameWorldMaster::OnFixedUpdate(unsigned int time, int delte)
{
	if (!runing_) return;

	time_couter += delte;
	unsigned int frameId = time_couter / fixed_update_ms_;
	int frameStep = frameId - frame_time_.GetFrameID();
	for (int i = 0; i < frameStep; ++i)
	{
		GameLoop();
	}
}

void FrameWorldMaster::GameLoop()
{
	OnFrameBegin();

	//handl all on frame
	if (preprocess_callback != nullptr)
	{
		//pre handle cmd
		preprocess_callback(cmd_queue_in_, GetFrameTime());
	}
	else
	{
		//default handle!
		for (auto& cmd : cmd_queue_in_->GetCmdList())
		{
			//adjust frame id and time
			cmd->frame_id = frame_time_.GetFrameID();
			cmd->time = frame_time_.GetTime();
		}
	}

	remove_list_.clear();

	for (auto& cmd : cmd_queue_in_->GetCmdList())
	{
		if (cmd->frame_id == frame_time_.GetFrameID())
		{
			HandleOneCmd(cmd);
			remove_list_.push_back(cmd);
		}
		else
		{
			int x = 0;
		}
	}

	//要做cache，不能直接清空了,需要清空处理过的
	cmd_queue_in_->ClearCmd(remove_list_);

	OnFrameEnd();
}



