#ifndef _LOCAL_FRAME_WORLD_H_
#define _LOCAL_FRAME_WORLD_H_


#include "frame_world.h"


//被动帧驱动,作为客户端以主驱动的帧指令来驱动帧

class FrameWorldSlave : public FrameWorld
{
public:

	FrameWorldSlave(int cmd_max, FrameCmdQueue* queue);
	~FrameWorldSlave();
	
	virtual void OnFixedUpdate(unsigned int time, int delte);
	
	
protected:
	

private:
	void HandleCmd();

};




#endif