#ifndef _DEFINE_H_
#define _DEFINE_H_


typedef unsigned int Time;
typedef unsigned int FrameID;
typedef unsigned short CmdID;
typedef unsigned int SeqID;
typedef unsigned int UID;

const int FIXED_UPDATE_MS = 66;
const int UPDATE_MS = 33;

const int FRAME_START_CMD = 0; 
const int FRAME_END_CMD = 1;   
const int FRAME_INTERNAL_CMD_NUM = 10; //内部预留服务端10个命令,业务cmd id 往后排
//const int FRAME_CMD_MAX = 128;


#pragma pack(push) 
#pragma pack(1)

struct Cmd
{
	CmdID  id;
	SeqID  seq_id;
	UID     uid;
	
	Cmd()
	{
		seq_id = 0;
		uid = 0;
	}
};
#pragma pack(pop)



#endif