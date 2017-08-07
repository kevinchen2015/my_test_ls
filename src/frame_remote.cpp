#include "frame_remote.h"
#include <assert.h>

FrameRemote::FrameRemote()
{
	frame_time_.Reset();
	user_data_ = nullptr;
	seq_id_ = 0;
}

