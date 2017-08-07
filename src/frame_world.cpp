#include "frame_world.h"
#include "frame_cmd.h"
#include <assert.h>

FrameWorld::FrameWorld(int cmd_max,FrameCmdQueue* queue)
	:runing_(false)
	,cmd_queue_in_(queue)
	, fixed_update_ms_(FIXED_UPDATE_MS)
{
	cmd_max_ = cmd_max;
	on_pre_frame = nullptr;
	on_post_frame = nullptr;
	
	on_pre_handle_cmd = nullptr;
	on_post_handle_cmd = nullptr;

	cmd_handlers_ = new std::function<void(FrameCmd*)>[cmd_max_];

	for (int i = 0; i < cmd_max_; ++i)
	{
		cmd_handlers_[i] = nullptr;
	}
}

FrameWorld::~FrameWorld()
{
	delete[] cmd_handlers_;
}

void FrameWorld::RegistHandle(CmdID id, std::function<void(FrameCmd*)> func)
{
   assert(id >= 0 && id < cmd_max_);
   auto f = cmd_handlers_[id];
   assert(f == nullptr);
   cmd_handlers_[id] = func;
}

void FrameWorld::UnRigistHandle(CmdID id)
{
	assert(id > 0 && id < cmd_max_);
	cmd_handlers_[id] = nullptr;
}

void FrameWorld::Start()
{
	runing_ = true;
	frame_time_.Reset();
}

void FrameWorld::Stop()
{
	runing_ = false;
}


void FrameWorld::OnFrameBegin()
{
	frame_time_.Increase(fixed_update_ms_);

	if (on_pre_frame != nullptr)
	{
		on_pre_frame();
	}
}

void FrameWorld::HandleOneCmd(FrameCmd* cmd)
{
	assert(cmd->cmd->id >= 0 && cmd->cmd->id < cmd_max_);
	auto func = cmd_handlers_[cmd->cmd->id];
	if (func != nullptr)
	{
		if (on_pre_handle_cmd != nullptr)
		{
			 on_pre_handle_cmd(cmd);
		}
		if (!cmd->no_need_handle)
		{
			func(cmd);
			if (on_post_handle_cmd != nullptr)
			{
				on_post_handle_cmd(cmd);
			}
		}
	}
	else
	{
		assert(0);
	}
}

void FrameWorld::OnFrameEnd()
{
	//post frame
	if (on_post_frame != nullptr)
	{
		on_post_frame();
	}
}
