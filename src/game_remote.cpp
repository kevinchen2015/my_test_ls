#include "game_remote.h"
#include <assert.h>


GameRemote::GameRemote()
{
	Reset();
}

void GameRemote::Reset()
{
	conn_ = nullptr;
	user_data_ = nullptr;
	remote_frame_time_.Reset();
	remote_input_.Reset();
}


