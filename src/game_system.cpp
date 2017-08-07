#include "game_system.h"
#include "ecs_system.h"
#include <assert.h>

GameSystem::~GameSystem()
{
	for (auto s : systems_)
	{
		delete (s);
	}
}

void GameSystem::OnFixedUpdate(unsigned int time, int delta)
{
	for (auto s : systems_)
	{
		(s)->OnFixedUpdate(time, delta);
	}
}

void GameSystem::OnUpdate(unsigned int time, int delta)
{
	for (auto s : systems_)
	{
		(s)->OnUpdate(time, delta);
	}
}

void GameSystem::RollBack(unsigned int time)
{
	for (auto s : systems_)
	{
		(s)->RollBack(time);
	}
}

void GameSystem::AddSystem(ecs::System* system)
{
	systems_.push_back(system);
}

void GameSystem::OnEntityEnter(ecs::Entity* entity)
{
	for (auto s : systems_)
	{
		(s)->OnEntityEnter(entity);
	}
}

void GameSystem::OnEntityExit(ecs::Entity* entity)
{
	for (auto s : systems_)
	{
		(s)->OnEntityExit(entity);
	}
}