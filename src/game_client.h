#ifndef _GAME_CLIENT_H_
#define _GAME_CLIENT_H_

#include<network.h>
#include "define.h"
#include "game_cmd.h"
#include "frame_cmd.h"
#include <map>

class FrameWorldSlave;
class FrameCmdQueue;
struct FrameCmd;
class GameInput;

class GameWorldClient;
class GameClient
{
public:
	GameClient();
	~GameClient();
	void OnFixedUpdate(unsigned int time, int delta);
	void OnUpdate(unsigned int time, int delta);

	void OnPreFrame();
	void OnPostFrame();

	bool IsFrameEnd() { return frame_end_; }
	void Start();


	//game logic
	GameWorldClient* game_world_;
private:
	void FrameStartHandle(FrameCmd* cmd);
	void FrameEndHandle(FrameCmd* cmd);
	void CreateEntityHandle(FrameCmd* cmd);
	void InputCmdHandle(FrameCmd* cmd);
	
private:
	FrameWorldSlave* frame_world_;
	FrameCmdQueue* cmd_queue_in_;
	FrameCmdQueue* cmd_queue_out_;
	bool frame_end_;

	GameStatus status_;

	
	
	//net
	sl_client_t* net_client_;
	static void c_on_recv_data(char* buff, unsigned int size, void* conn, int channel, void* user_data);
	static void c_on_connected(void* conn, int status, int channel, void* user_data);
	static void c_on_close(void* conn, int channel, void* user_data);
};


#endif