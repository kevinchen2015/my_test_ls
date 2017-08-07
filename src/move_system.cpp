#include "move_system.h"

#include <iostream>

void MoveSystem::OnFixedUpdate(unsigned int time, int delta)
{
	for (Member& m : members_)
	{
		//std::cout << "move tick ,entity id:" <<m.p1->GetEntity()->GetEntityID();
		//std::cout << "  time:" << time;
		//if (m.p1->move_type == MOVE_STOP)
		//std::cout << " move stop " << std::endl;
		move_tick(m.p1->trans, m.p1->move_type, m.p1->move_dir, m.p1->move_speed, delta);
	}
}

//--------------------------------------------------------------


void MovePredictSystem::OnFixedUpdate(unsigned int time, int delta)
{
	for (Member& m : members_)
	{
		//std::cout << "move predict tick ,entity id:" << m.p1->GetEntity()->GetEntityID();
		//std::cout << "  time:" << time;
		//if(m.p1->move_type == MOVE_STOP)
		//	std::cout << " move stop " << std::endl;
		move_tick(m.p1->trans,m.p1->move_type, m.p1->move_dir, m.p1->move_speed, delta);
	}
}

void MovePredictSystem::OnUpdate(unsigned int time, int delta)
{
	//todo...
}

void MovePredictSystem::RollBack(unsigned int time)
{
	std::cout << "move RollBack  "<< time << std::endl;
	for (Member& m : members_)
	{
		m.p2->trans = m.p1->trans;
		m.p2->move_dir = m.p1->move_dir;
		m.p2->move_speed = m.p1->move_speed;
		m.p2->move_type = m.p1->move_type;
	}
}
