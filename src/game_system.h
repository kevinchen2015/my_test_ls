#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include <list>

namespace ecs
{
	class System;
	class Entity;
}
class FrameTime;
class GameSystem
{
public:
	~GameSystem();
	void OnFixedUpdate(unsigned int time, int delta);
	void RollBack(unsigned int time);
	void OnUpdate(unsigned int time, int delta);

	void AddSystem(ecs::System* system);
	void OnEntityEnter(ecs::Entity* entity);
	void OnEntityExit(ecs::Entity* entity);
private:
	std::list<ecs::System*> systems_;
};


#endif