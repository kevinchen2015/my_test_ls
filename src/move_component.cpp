#include "move_component.h"
#include "game_input.h"
#include "frame_cmd.h"
#include "game_world.h"
#include "game_entity_mgr.h"
#include "ecs_entity.h"

#include <iostream>

void move_tick(TransformD& trans, MoveType move_type, Vector3D dir, int speed, int delta_time)
{
	if (speed == 0 || delta_time == 0)
		return;

	if (move_type == MOVE_STOP)
	{
	
	}
	else
	{
		int distance = speed * delta_time;
		Vector3D  dis_vector = dir.ProductD4(distance);
		trans.position += dis_vector;
	}
}


void handle_move_cmd(FrameCmd* cmd,GameWorld* game_world)
{
	if (cmd->cmd->id == GAME_MOVE_CMD)
	{
		MoveCmd* move_cmd = static_cast<MoveCmd*>(cmd->cmd);
		ecs::Entity* entity = (game_world->GetEntityMgr())->GetEntity(move_cmd->entity_id);
		if (entity)
		{
			MoveComponent* move_component = entity->GetComponent<MoveComponent>();
			if (move_component)
			{
				move_component->trans.faceToD4 = move_cmd->face;
				move_component->move_speed = move_cmd->speed;
				move_component->move_type = move_cmd->move_type;

				move_component->move_dir = move_cmd->face;  //todo...
			}
		}
	}
}

void handle_predict_move_cmd(MoveCmd* cmd, MovePredictComponent* comp)
{
	cmd->face = comp->trans.faceToD4;
	cmd->speed = comp->move_speed;

	comp->move_type = cmd->move_type;
	comp->move_dir = comp->trans.faceToD4;
}

