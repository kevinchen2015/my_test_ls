#ifndef _GAME_REMOTE_H_
#define _GAME_REMOTE_H_


#include "game_cmd.h"
#include "frame_time.h"
#include "game_input.h"

//����Զ��һЩ���ݣ��������
class GameRemote
{
public:
	GameRemote();
	void Reset();
private:
	friend class GameServer;

	void* conn_;
	void* user_data_;

	GameInput remote_input_;
	FrameTime remote_frame_time_;			//just for debug
};



#endif