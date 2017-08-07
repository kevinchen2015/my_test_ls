#ifndef _GAME_CMD_H_
#define _GAME_CMD_H_

#include "define.h"
#include "Vector3D.h"

enum GameStatus
{
	GAME_INIT = 0,
	GAME_WAIT_READY,
	GAME_RUNNING,
	GAME_OVER,
};

#define GAME_CMD_NUM 128
enum GameCmdType
{
	GAME_MOVE_CMD = FRAME_INTERNAL_CMD_NUM + 1,  //移动消息在服务端预测发包，客户端剩流量不发包，要求第一个处理

	GAME_CREATE_ENTITY_CMD,
};

enum EntityType
{
	ENTITY_ACTOR = 0,

};

enum MoveType
{
	MOVE_STOP = 0,
	MOVE_FOWARD,
	MOVE_BACK,
	MOVE_LEFT,
	MOVE_RIGHT,
};

const int PREDICT_TAG = 0x01;

#pragma pack(push) 
#pragma pack(1)

struct NetMsg
{
	unsigned short size;
	FrameID  frame_id;
	Time	 time;

};

struct CreateEntityCmd : public Cmd
{
	unsigned int entity_id;
	EntityType   type;
	unsigned int res_id;

	CreateEntityCmd()
	{
		id = GAME_CREATE_ENTITY_CMD;
		entity_id = 0;
		type = ENTITY_ACTOR;
		res_id = 0;
		uid = 0;
	}
};


struct MoveCmd : public Cmd
{
	unsigned int entity_id;
	MoveType     move_type;
	int			 speed;
	Vector3D     face;

	MoveCmd()
		:Cmd()
	{
		id = GAME_MOVE_CMD;
		entity_id = 0;
		move_type = MOVE_STOP;
	}
};


#pragma pack(pop)




#endif