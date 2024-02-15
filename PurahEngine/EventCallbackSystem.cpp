#include "EventCallbackSystem.h"

#include "GameObject.h"
#include "ZnCollider.h"

namespace PurahEngine
{
	void EventCallbackSystem::OnWake(const ZonaiPhysics::ZnRigidBody*)
	{
		// 구현 안 함
		return;
	}

	void EventCallbackSystem::OnSleep(const ZonaiPhysics::ZnRigidBody*)
	{
		// 구현 안 함
		return;
	}

	void EventCallbackSystem::OnConstraintBreak(const ZonaiPhysics::ZnJoint*)
	{
		// 구현 안 함
		return;
	}

	void EventCallbackSystem::OnTriggerEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1)
	{
		auto user0 =  _c0->GetUserData();
		const auto g0 = static_cast<GameObject*>(user0);

		auto user1 = _c1->GetUserData();
		const auto g1 = static_cast<GameObject*>(user1);

		// g0->OnTriggerEnter(user1);
		// g1->OnTriggerEnter(user0);
	}

	void EventCallbackSystem::OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnCollisionEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		//

	}

	void EventCallbackSystem::OnCollisionStay(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		// 
	}

	void EventCallbackSystem::OnCollisionExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		// 
	}
}
