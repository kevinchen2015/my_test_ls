#ifndef _FRAME_WORLD_H_
#define _FRAME_WORLD_H_


#include "define.h"
#include "frame_cmd.h"
#include "frame_time.h"
#include <map>
#include <functional>


//帧世界驱动框架，只做一件事情，以帧的概念驱动框架执行cmd处理和FrameTime更新
//子类根据时间和帧来驱动来区分master slave

class FrameWorld
{
public:
	FrameWorld(int cmd_max, FrameCmdQueue* queue);
	virtual ~FrameWorld();
	void Start();
	void Stop();
	virtual void OnFixedUpdate(unsigned int time, int delte) = 0;
	void RegistHandle(CmdID id, std::function<void(FrameCmd*)> func);
	void UnRigistHandle(CmdID id);
	FrameTime* GetFrameTime() { return &frame_time_; }

	std::function<void()> on_pre_frame;
	std::function<void()> on_post_frame;
	
	std::function<void(FrameCmd*)> on_pre_handle_cmd;
	std::function<void(FrameCmd*)> on_post_handle_cmd;
protected:
	FrameCmdQueue* cmd_queue_in_;
	unsigned int fixed_update_ms_;
	FrameTime frame_time_;
	int cmd_max_;
	std::function<void(FrameCmd*)>* cmd_handlers_;
	bool runing_;

	virtual void OnFrameBegin();
	virtual void OnFrameEnd();

	void HandleOneCmd(FrameCmd* cmd);
private:
	
};



#endif