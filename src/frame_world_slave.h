#ifndef _LOCAL_FRAME_WORLD_H_
#define _LOCAL_FRAME_WORLD_H_


#include "frame_world.h"


//����֡����,��Ϊ�ͻ�������������ָ֡��������֡

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