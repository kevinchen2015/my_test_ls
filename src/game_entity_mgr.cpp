#include "game_entity_mgr.h"
#include "ecs_entity.h"

void GameEntityMgr::AddEntity(ecs::Entity* entity)
{
	entitys_[entity->GetEntityID()] = entity;
}

ecs::Entity* GameEntityMgr::GetEntity(ecs::EntityID id)
{
	auto iter = entitys_.find(id);
	if (iter != entitys_.end())
		return iter->second;

	return nullptr;
}

void GameEntityMgr::RemoveEntity(ecs::EntityID id)
{
	entitys_.erase(id);
}