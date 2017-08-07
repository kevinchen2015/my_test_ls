#include "actor_helper.h"
#include "move_component.h"
#include "component_id_define.h"

#include <assert.h>
#include "ecs_entity.h"
#include "ecs_component.h"

#include "game_cmd.h"

#define GAME_COMPONENT_MAX 64

static void ReleaseComponent(ecs::Component* p,int id)
{
	switch (id)
	{
	case MOVE_COMPONENT:
		delete ((MoveComponent*)p);
		break;

	case MOVE_PREDICT_COMPONENT:
		delete ((MovePredictComponent*)p);
		break;

	default:
		assert(0);
		break;
	}
}

ecs::Entity* ActorHelper::CreateServerActor(int id)
{
	ecs::Entity* entity = new ecs::Entity(id, GAME_COMPONENT_MAX);
	entity->AddComponent<MoveComponent>(new MoveComponent);
	return entity;
}

void ActorHelper::ReleaseServerActor(ecs::Entity* entity)
{
	for (int i = 0; i < entity->GetComponentMaxNum(); ++i)
	{
		ecs::Component* p = entity->GetComponent(i);
		ReleaseComponent(p, i);
	}
	delete entity;
}

ecs::Entity* ActorHelper::CreateClientActor(int id,bool predict)
{
	ecs::Entity* entity = new ecs::Entity(id, GAME_COMPONENT_MAX);
	entity->AddComponent<MoveComponent>(new MoveComponent);

	if (predict)
	{
		entity->AddComponent<MovePredictComponent>(new MovePredictComponent);
		ecs::TagID tag_id = entity->GetTagID();
		tag_id |= PREDICT_TAG;
		entity->SetTagID(tag_id);
	}
	return entity;
}

void ActorHelper::ReleaseClientActor(ecs::Entity* entity)
{
	for (int i = 0; i < entity->GetComponentMaxNum(); ++i)
	{
		ecs::Component* p = entity->GetComponent(i);
		ReleaseComponent(p, i);
	}
	delete entity;
}
