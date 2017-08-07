#ifndef _GAME_SERVER_H_
#define _GAME_SERVER_H_


class FrameWorldMaster;
class FrameCmdQueue;
struct FrameCmd;
class GameRemote;
class FrameTime;

#include<network.h>
#include "define.h"
#include "game_cmd.h"
#include<map>
#include<list>


//暂时测试功能，把网络和Room合在一起
class GameWorld;
class GameServer
{
public:
	GameServer();
	~GameServer();

	void OnFixedUpdate(unsigned int time,int delta);

	bool IsFrameEnd() { return frame_end_; }
	void Start();

	void Broadcast(char* buf, unsigned int size);

	GameRemote* GetRemote(UID uid);
private:
	FrameWorldMaster* frame_world_;
	bool frame_end_;
	FrameCmdQueue* cmd_queue_in_;
	FrameCmdQueue* cmd_queue_out_;
	UID			   uid_counter_;
	GameStatus	   status_;

	std::map<UID,GameRemote*> game_remotes_;
	std::list<GameRemote*>    game_remote_list_;

	//net
	sl_server_t* net_server_;
	static void s_on_recv_data(char* buff, unsigned int size, void* conn, int channel, void* user_data);
	static void s_on_connected(void* conn, int status, int channel, void* user_data);
	static void s_on_close(void* conn, int channel, void* user_data);

	//game logic
	GameWorld* game_world_;
	unsigned int entity_id_counter_;
private:

	void OnPreFrame();
	void OnPostFrame();
	void PreprocessCmd(FrameCmdQueue* queue, FrameTime* time);

private:

	void InputCmdHandle(FrameCmd* cmd);

	
};




#endif