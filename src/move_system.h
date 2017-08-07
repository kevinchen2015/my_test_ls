#ifndef _MOVE_SYSTEM_H_
#define _MOVE_SYSTEM_H_


#include "ecs_entity.h"
#include "ecs_system.h"
#include "move_component.h"


struct MoveParam
{
	MoveComponent* p1;
};
class MoveSystem : public ecs::System
{
public:
	
	void OnFixedUpdate(unsigned int time, int delta);

	SYSTEM_DECLEAR_DEFAULT_FUNC();


protected:
	SYSTEM_DECLEAR_MEMBER(MoveParam);
	SYSTEM_DECLEAR_PICK_MEMBER_1(MoveParam, MoveComponent);
};

//--------------------------------------------------------------

struct MovePredictParam
{
	MoveComponent* p1;
	MovePredictComponent* p2;
};
class MovePredictSystem : public ecs::System
{
public:

	void OnFixedUpdate(unsigned int time, int delta);
	void OnUpdate(unsigned int time, int delta);
	void RollBack(unsigned int time);

	SYSTEM_DECLEAR_DEFAULT_FUNC();
	
protected:
	SYSTEM_DECLEAR_MEMBER(MovePredictParam);
	SYSTEM_DECLEAR_PICK_MEMBER_2(MovePredictParam, MoveComponent, MovePredictComponent);
};


#endif