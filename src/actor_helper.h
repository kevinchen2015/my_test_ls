#ifndef _ACTOR_HELPER_H_
#define _ACTOR_HELPER_H_

namespace ecs
{
	class Entity;
}

class ActorHelper
{
public:
	
	static ecs::Entity* CreateServerActor(int id);
	static void ReleaseServerActor(ecs::Entity* entity);

	static ecs::Entity* CreateClientActor(int id,bool predict);
	static void ReleaseClientActor(ecs::Entity* entity);
};


#endif