#ifndef _GAME_INPUT_H_
#define _GAME_INPUT_H_

#include "game_cmd.h"
#include "frame_time.h"
#include <list>

struct FrameCmd;
class FrameCmdQueue;
class GameInput
{
public:

	GameInput();
	~GameInput();

	void Reset();
	
	struct InputCmd
	{
		FrameID frame_id;
		Cmd* cmd;
		unsigned short cmd_len;

		InputCmd()
		{
			cmd = nullptr;
			cmd_len = 0;
			Reset();
		}

		void Reset();

		bool IsWritten() { return (cmd && cmd->id!=0); }
	};

	void PushCmd(FrameID frame_id ,Cmd* cmd);
	void InputCmdOutput(FrameTime* time,FrameCmdQueue* out_queue);
	InputCmd& GetInputCmd(int cmd_id) {return input_cmd_[cmd_id]; }
	void ClearAllCmd();

	SeqID GetNextSeqID() { return ++seq_id_; }
	
//protected:

	UID		  uid_;
	SeqID     seq_id_;
	InputCmd  input_cmd_[GAME_CMD_NUM];
	
};

//-------------------------------
class GameInputCache
{
public:
	GameInputCache();
	void PushInput(GameInput::InputCmd* svr_cmd);
	std::list<GameInput::InputCmd*>& GetCmdList();
	bool Conflict(FrameCmd* cmd);
	void ClearCmdCache(FrameID frame_id);
	//void ClearCmdAbove(FrameID frame_id);
	int  frame_dis_;
private:
	std::list<GameInput::InputCmd*> input_cache_;
};

#endif