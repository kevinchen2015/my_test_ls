#include "ecs_entity.h"
#include "ecs_component.h"
#include <assert.h>
namespace ecs
{

	Entity::Entity(EntityID id,int component_num)
	{
		user_data_ = nullptr;
		user_data2_ = nullptr;

		component_num_ = component_num;
		entity_id_ = id;
		tag_id_ = 0;

		components_ = new Component*[component_num_];
		for (int i = 0; i < component_num_; ++i)
		{
			components_[i] = nullptr;
		}
	}
	
	Entity::~Entity()
	{
		user_data_ = nullptr;
		user_data2_ = nullptr;
		delete[] components_;
	}

	
}
