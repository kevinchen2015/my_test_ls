#ifndef _MOVE_COMPONENT_H_
#define _MOVE_COMPONENT_H_

#include "ecs_component.h"
#include "TransformD.h"
#include "game_cmd.h"

struct MoveComponent : public  ecs::Component
{
	COMPONENT_DECLEAR_ID();

	TransformD trans;
	Vector3D   move_dir;
	int		   move_speed;
	MoveType   move_type;

	MoveComponent()
	{
		trans.faceToD4 = Vector3D::FORWARD;
		move_dir = trans.faceToD4;
		move_type = MOVE_STOP;
		move_speed = 10;
	}
};

struct MovePredictComponent : public  ecs::Component
{
	COMPONENT_DECLEAR_ID();

	TransformD trans;
	Vector3D   move_dir;
	int		   move_speed;
	MoveType   move_type;

	MovePredictComponent()
	{
		trans.faceToD4 = Vector3D::FORWARD;
		move_dir = trans.faceToD4;
		move_type = MOVE_STOP;
		move_speed = 10;
	}
};


//move utilit func

void move_tick(TransformD& trans, MoveType move_type, Vector3D dir, int speed, int delta_time);

class FrameCmd;
class GameWorld;
void handle_move_cmd(FrameCmd* cmd, GameWorld* game_world);

struct MoveCmd;
void handle_predict_move_cmd(MoveCmd* cmd, MovePredictComponent* comp);

#endif