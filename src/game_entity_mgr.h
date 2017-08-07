#ifndef _GAME_ENTITY_MGR_H_
#define _GAME_ENTITY_MGR_H_

#include <map>
#include "ecs_entity.h"

class GameEntityMgr
{
public:
	void AddEntity(ecs::Entity* entity);
	ecs::Entity* GetEntity(ecs::EntityID id);
	void RemoveEntity(ecs::EntityID id);
private:
	std::map<int, ecs::Entity*> entitys_;
};


#endif