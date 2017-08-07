#ifndef _GAME_WORLD_CLIENT_H_
#define _GAME_WORLD_CLIENT_H_

#include "game_world.h"
#include "define.h"
#include<map>

class GamePredict;
struct FrameCmd;
class GameInput;

//客户端游戏逻辑，多了表现层和预测相关的东西
class GameWorldClient : public GameWorld
{
public:
	GameWorldClient(int cmd_max);
	~GameWorldClient();
	//真实帧逻辑更新
	virtual void OnFixedUpdate(FrameTime* game_time);
	
	//本地时间更新,用于预测
	void OnLocalFixedUpdate(unsigned int time, int delta);
	//本地表现更新，用于表现
	void OnLocalUpdate(unsigned int time, int delta);
	void OnRecvNetCmd(FrameCmd* cmd);

	void SetRealFrameTime(FrameTime* time) { real_time_ = time; }

	virtual void AddEntity(ecs::Entity* entity);
	virtual void RemoveEntity(ecs::Entity* entity);
	UID GetPredictUID();
	//本地预测
	GamePredict* game_predict_;
	GameSystem*  GetPredictSystem() { return predict_systems_; }
	GameInput*   GetInput(UID uid);
private:
	bool predict_start_;
	Time time_couter;
	int  wait_frame_id_;

	//全部远端输入
	std::map<UID, GameInput*> remote_inputs_;
	FrameTime* real_time_;

	GameSystem*  predict_systems_;

	void PredictOneFrame();
	void RollPredictFrame();
	void RollforthOneFrame();
	void PredictFixedUpdate();
};


#endif