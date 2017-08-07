#include "frame_world_slave.h"
#include "frame_cmd.h"
#include <assert.h>

FrameWorldSlave::FrameWorldSlave(int cmd_max, FrameCmdQueue* queue)
	:FrameWorld(cmd_max,queue)
{

}

FrameWorldSlave::~FrameWorldSlave()
{

}

void FrameWorldSlave::OnFixedUpdate(unsigned int time, int delte)
{
	if (!runing_) return;

	while (cmd_queue_in_->GetCmdList().size() > 0)
	{
		for (auto& cmd : cmd_queue_in_->GetCmdList())
		{
			if (cmd->frame_id == 1 + frame_time_.GetFrameID())
			{
				if (cmd->cmd->id == FRAME_START_CMD)
				{
					//begin new frame
					OnFrameBegin();

					//handl
					HandleCmd();
				}
				else
				{
					assert(0);
				}
			}
			else if (cmd->frame_id == frame_time_.GetFrameID())
			{
				if (FRAME_END_CMD == cmd->cmd->id)
				{
					OnFrameEnd();
					goto __END;
				}
				else
				{
					//continue
					HandleCmd();
				}
			}
			else if (cmd->frame_id < frame_time_.GetFrameID())
			{
				assert(0);
			}
			
			break; //handle once
		}
	}

__END:

	return;
}

void FrameWorldSlave::HandleCmd()
{
	std::list<FrameCmd*> remove;
	for (auto& cmd : cmd_queue_in_->GetCmdList())
	{
		if (cmd->frame_id == frame_time_.GetFrameID())
		{
			remove.push_back(cmd);
			HandleOneCmd(cmd);

			if (FRAME_END_CMD == cmd->cmd->id)
			{
				OnFrameEnd();
				break;
			}
		}
		else
		{
			break;
		}
	}
	cmd_queue_in_->ClearCmd(remove);
}







