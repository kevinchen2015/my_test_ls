#ifndef _ECS_SYSTEM_H_
#define _ECS_SYSTEM_H_

#include <list>

namespace ecs
{
	class Entity;

	//只有行为,对感兴趣数据的相关行为更新或处理
	class System
	{
	public:
		virtual ~System() {}
		virtual void OnEntityEnter(Entity* entity) = 0;
		virtual void OnEntityExit(Entity* entity) = 0;
		
		virtual void OnFixedUpdate(unsigned int time, int delta) = 0;

		virtual void OnUpdate(unsigned int time, int delta) {};
		virtual void RollBack(unsigned int time) {};
		//virtual void RollForth() = 0;
	};
	
}


//可选宏

#define SYSTEM_DECLEAR_DEFAULT_FUNC()  \
public:\
	void OnEntityEnter(ecs::Entity* entity){PickMember(entity);} \
	void OnEntityExit(ecs::Entity* entity){RemoveMember(entity);} \

#define SYSTEM_DECLEAR_MEMBER(T) \
public:\
	typedef T Member;\
protected:\
	std::list<T> members_;\
	void RemoveMember(ecs::Entity* entity) \
	{\
		auto iter = members_.begin();\
		for(;iter != members_.end();++iter)\
		{\
			T& t = (*iter); \
			if (t.p1->GetEntity() == entity)\
			{\
				members_.erase(iter); \
				return; \
			}\
		}\
	}; \
	

#define SYSTEM_DECLEAR_PICK_MEMBER_1(T,P1) \
protected:\
	void PickMember(ecs::Entity* entity) \
	{\
		P1* p1 = entity->GetComponent<P1>();\
		if(p1)\
		{\
			T t;\
			t.p1 = p1;\
			members_.push_back(t);\
		}\
	}\

#define SYSTEM_DECLEAR_PICK_MEMBER_2(T,P1,P2) \
protected:\
	void PickMember(ecs::Entity* entity) \
	{\
		P1* p1 = entity->GetComponent<P1>();\
		P2* p2 = entity->GetComponent<P2>();\
		if(p1 && p2)\
		{\
			T t;\
			t.p1 = p1;\
			t.p2 = p2;\
			members_.push_back(t);\
		}\
	}\

#define SYSTEM_DECLEAR_PICK_MEMBER_3(T,P1,P2,P3) \
protected:\
	void PickMember(ecs::Entity* entity) \
	{\
		P1* p1 = entity->GetComponent<P1>();\
		P2* p2 = entity->GetComponent<P2>();\
		P3* p3 = entity->GetComponent<P3>();\
		if(p1 && p2)\
		{\
			T t;\
			t.p1 = p1;\
			t.p2 = p2;\
			t.p3 = p3;\
			members_.push_back(t);\
		}\
	}\

#define SYSTEM_DECLEAR_PICK_MEMBER_4(T,P1,P2,P3,P4) \
protected:\
	void PickMember(ecs::Entity* entity) \
	{\
		P1* p1 = entity->GetComponent<P1>();\
		P2* p2 = entity->GetComponent<P2>();\
		P3* p3 = entity->GetComponent<P3>();\
		P4* p4 = entity->GetComponent<P4>();\
		if(p1 && p2)\
		{\
			T t;\
			t.p1 = p1;\
			t.p2 = p2;\
			t.p3 = p3;\
			t.p4 = p4;\
			members_.push_back(t);\
		}\
	}\


#endif