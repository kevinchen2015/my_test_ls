#ifndef _FRAME_TIME_H_
#define _FRAME_TIME_H_

#include "define.h"

class FrameTime
{
public:
	FrameTime()
	{
		Reset();
	}

	void Reset()
	{
		frame_id_ = 0;
		time_ = 0;
	}

	FrameID GetFrameID() { return frame_id_; }
	Time GetTime() { return time_; }
	int  GetDelta() { return delta_; }

	void Increase(Time delta)
	{
		++frame_id_;
		time_ += delta;
		delta_ = delta;
	}

	void SetFrameTime(FrameID frame_id,Time time)
	{
		frame_id_ = frame_id;
		time_ = time;
	}
private:

	FrameID frame_id_;
	Time time_;
	int  delta_;
};




#endif