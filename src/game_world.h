#ifndef _GAME_WORLD_H_
#define _GAME_WORLD_H_

class GameSystem;
class FrameTime;
class GameEntityMgr;
class FrameCmd;

#include <functional>

#define PREDICT_MAX 15  //预测上限
namespace ecs
{
	class Entity;
}

//游戏逻辑主框架,被某种方式（帧同步）驱动
class GameWorld
{
public:
	GameWorld(int cmd_max);
	virtual ~GameWorld();
	virtual void OnFixedUpdate(FrameTime* game_time);
	virtual void AddEntity(ecs::Entity* entity);
	virtual void RemoveEntity(ecs::Entity* entity);

	GameSystem* GetSystem() { return systems_; }
	GameEntityMgr* GetEntityMgr() { return entity_mgr_; }

	void RegistHandler(int cmd_id, std::function<void(FrameCmd*, GameWorld*)> func)
	{
		cmd_handlers_[cmd_id] = func;
	}
	void HandleCmd(FrameCmd* cmd);
protected:
	GameSystem* systems_;
	GameEntityMgr* entity_mgr_;
	int cmd_max_;
	std::function<void(FrameCmd*, GameWorld*)>* cmd_handlers_;
};


#endif