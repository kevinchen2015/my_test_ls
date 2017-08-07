#ifndef _FRAME_REMOTE_H_
#define _FRAME_REMOTE_H_

#include"frame_time.h"

//主要用来debug

class FrameRemote
{
public:
	FrameRemote();

	FrameTime frame_time_;
	void*     user_data_;
	SEQ_ID    seq_id_;
	UID       uid_;
private:
	
};


#endif