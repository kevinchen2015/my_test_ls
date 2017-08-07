#ifndef _GAME_WORLD_CLIENT_H_
#define _GAME_WORLD_CLIENT_H_

#include "game_world.h"
#include "define.h"
#include<map>

class GamePredict;
struct FrameCmd;
class GameInput;

//�ͻ�����Ϸ�߼������˱��ֲ��Ԥ����صĶ���
class GameWorldClient : public GameWorld
{
public:
	GameWorldClient(int cmd_max);
	~GameWorldClient();
	//��ʵ֡�߼�����
	virtual void OnFixedUpdate(FrameTime* game_time);
	
	//����ʱ�����,����Ԥ��
	void OnLocalFixedUpdate(unsigned int time, int delta);
	//���ر��ָ��£����ڱ���
	void OnLocalUpdate(unsigned int time, int delta);
	void OnRecvNetCmd(FrameCmd* cmd);

	void SetRealFrameTime(FrameTime* time) { real_time_ = time; }

	virtual void AddEntity(ecs::Entity* entity);
	virtual void RemoveEntity(ecs::Entity* entity);
	UID GetPredictUID();
	//����Ԥ��
	GamePredict* game_predict_;
	GameSystem*  GetPredictSystem() { return predict_systems_; }
	GameInput*   GetInput(UID uid);
private:
	bool predict_start_;
	Time time_couter;
	int  wait_frame_id_;

	//ȫ��Զ������
	std::map<UID, GameInput*> remote_inputs_;
	FrameTime* real_time_;

	GameSystem*  predict_systems_;

	void PredictOneFrame();
	void RollPredictFrame();
	void RollforthOneFrame();
	void PredictFixedUpdate();
};


#endif