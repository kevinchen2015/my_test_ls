#include "frame_cmd.h"
#include "frame_time.h"
#include <assert.h>


FrameCmdQueue::~FrameCmdQueue()
{
	Clear();
}

void FrameCmdQueue::PushFrameCmd(FrameCmd* frame_cmd)
{
	frame_cmds_.push_back(frame_cmd);
}

void FrameCmdQueue::Clear()
{
	for (auto cmd : frame_cmds_)
	{
		free(cmd->cmd);
		delete cmd;
	}
	frame_cmds_.clear();
}

void FrameCmdQueue::ClearCmd(std::list<FrameCmd*>& cmds)
{
	for (auto cmd : cmds)
	{
		frame_cmds_.remove(cmd);

		free(cmd->cmd);
		delete cmd;
	}
}

void FrameCmdQueue::AdjustFrameTime(FrameTime* time)
{
	for (auto cmd : frame_cmds_)
	{
		cmd->frame_id = time->GetFrameID();
		cmd->time = time->GetTime();
	}
}





