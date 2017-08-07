#ifndef _FRAME_CMD_H_
#define _FRAME_CMD_H_


#include "define.h"
#include<list>

struct FrameCmd
{
	Time	 time;
	FrameID  frame_id;
	Cmd*	 cmd;
	void*	 user_data;
	bool	 no_need_handle;
	unsigned short cmd_len;

	FrameCmd()
	{
		cmd_len = sizeof(Cmd);
		cmd = nullptr;
		user_data = nullptr;
		no_need_handle = false;
	}
};

class FrameTime;
class FrameCmdQueue
{
public:
	~FrameCmdQueue();
	void PushFrameCmd(FrameCmd* frame_cmd);
	void Clear();
	void ClearCmd(std::list<FrameCmd*>& cmds);
	int GetSize() { return frame_cmds_.size(); }
	std::list<FrameCmd*>& GetCmdList() {return frame_cmds_;}
	void AdjustFrameTime(FrameTime* time);
private:
	std::list<FrameCmd*> frame_cmds_;
};





#endif