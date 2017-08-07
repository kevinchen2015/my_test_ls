#include "ecs_component.h"

namespace ecs
{
	Component::Component()
	{
		private_data_.entity_ = nullptr;
	}

	Entity* Component::GetEntity()
	{
		return private_data_.entity_;
	}

}
