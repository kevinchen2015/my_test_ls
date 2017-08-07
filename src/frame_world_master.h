#ifndef _FRAME_WORLD_MASTER_H_
#define _FRAME_WORLD_MASTER_H_


#include"frame_world.h"
#include <map>

//主帧驱动，做服务器以时间片做帧驱动

class FrameWorldMaster : public FrameWorld
{
public:
	FrameWorldMaster(int cmd_max, FrameCmdQueue* queue);
	~FrameWorldMaster();

	virtual void OnFixedUpdate(unsigned int time, int delte);

	std::function<void(FrameCmdQueue*, FrameTime*)> preprocess_callback;

private:
	Time time_couter;
	std::list<FrameCmd*> remove_list_;
	void GameLoop();
};



#endif