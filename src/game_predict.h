#ifndef _GAME_PREDICT_H_
#define _GAME_PREDICT_H_

#include "game_input.h"
#include "frame_cmd.h"
#include "frame_time.h"

namespace ecs
{
	class Entity;
}

class FrameCmdQueue;
class GamePredict
{
public:
	GamePredict();

	void ResetUID(UID id);
	void OnRecivCmdBack(FrameCmd* cmd);

	void RestorHandleInputFromCache();
	void ResetInput();
	void HandleInput();
	void SyncInput();
	void ClearInputCache(FrameID frame_id);
	GameInput* GetPredictInput() {return &predict_input_; }
	FrameTime* GetFrameTime() {return &predict_time_;}

	FrameCmdQueue* predict_sync_out_;
	ecs::Entity*   predict_entity_;
	GameInputCache predict_cmd_cache_;
private:
	void _HandleOneInput(GameInput::InputCmd* cmd);
	friend class GameWorldClient;

	FrameTime predict_time_;
	GameInput predict_input_;

	
	bool predict_conflict_;

};


#endif