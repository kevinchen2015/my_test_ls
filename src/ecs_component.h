#ifndef _ECS_COMPONENT_H_
#define _ECS_COMPONENT_H_

namespace ecs
{
	class Entity;
	struct PrivateData
	{
		Entity*		entity_;
	};
	
	//ֻ�����ݣ�û����Ϊ,û���麯��,�������ݻع�����
	struct Component
	{
		Component();
		PrivateData private_data_;

		Entity* GetEntity();

		//public data
	};
}


#define COMPONENT_DECLEAR_ID() static int ID; 
#define COMPONENT_DEFINE_ID(T,id) int T::ID = id; 

#define COMPONENT_PUBLIC_DATA_COPY(src,des,T) memcpy(des+sizeof(PrivateData),src+sizeof(PrivateData),sizeof(T));

#endif