#ifndef _ECS_ENTITY_H_
#define _ECS_ENTITY_H_


namespace ecs
{
	typedef unsigned short ComponentID;
	typedef unsigned int EntityID;
	typedef unsigned short TagID;

	struct Component;

	//对象实体，聚合数据
	class Entity
	{
	public:
		Entity(EntityID id,int component_num = 128);
		~Entity();

		template<typename T>
		T* GetComponent()
		{
			if (T::ID >= component_num_)
			{
				//assert(0);
				return nullptr;
			}
			return static_cast<T*>(components_[T::ID]);
		}

		template<typename T>
		void AddComponent(T* component)
		{
			if (T::ID >= component_num_)
			{
				//assert(0);
				return;
			}
			//assert(components_[T::ID] == nullptr);
			//assert(component->GetEntity() == nullptr);

			components_[T::ID] = component;
			component->private_data_.entity_ = this;
		}

		Component* GetComponent(ComponentID id)
		{
			if (id >= component_num_)
			{
				//assert(0);
				return nullptr;
			}
			return components_[id];
		}

		EntityID GetEntityID() {return entity_id_;}
		void     SetTagID(TagID tag_id) { tag_id_ = tag_id; }
		TagID    GetTagID() { return tag_id_; }
		int      GetComponentMaxNum() {return component_num_;}

		void* user_data_;
		void* user_data2_;

	private:
		int		    component_num_;
		Component** components_;
		EntityID    entity_id_;
		TagID	    tag_id_;
	};
}


#endif