#include "game_world.h"
#include "frame_time.h"
#include "game_system.h"
#include "game_entity_mgr.h"
#include "frame_cmd.h"

GameWorld::GameWorld(int cmd_max)
{
	cmd_max_ = cmd_max;
	systems_ = new GameSystem();
	entity_mgr_ = new GameEntityMgr();

	cmd_handlers_ = new std::function<void(FrameCmd*, GameWorld*)>[cmd_max_];
	for (int i = 0; i < cmd_max_; ++i)
	{
		cmd_handlers_[i] = nullptr;
	}
}

GameWorld::~GameWorld()
{
	delete[] cmd_handlers_;
	delete systems_;
	delete entity_mgr_;
}

void GameWorld::OnFixedUpdate(FrameTime* game_time)
{
	systems_->OnFixedUpdate(game_time->GetTime(),game_time->GetDelta());
}

void GameWorld::AddEntity(ecs::Entity* entity)
{
	entity_mgr_->AddEntity(entity);
	systems_->OnEntityEnter(entity);
}

void GameWorld::RemoveEntity(ecs::Entity* entity)
{
	systems_->OnEntityExit(entity);
	entity_mgr_->RemoveEntity(entity->GetEntityID());
}

void GameWorld::HandleCmd(FrameCmd* cmd)
{
	if (cmd_handlers_[cmd->cmd->id])
	{
		cmd_handlers_[cmd->cmd->id](cmd, this);
	}
}

