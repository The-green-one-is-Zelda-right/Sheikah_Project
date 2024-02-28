#include "EventCallbackSystem.h"

#include "GameObject.h"
#include "ZnCollider.h"

namespace PurahEngine
{
	class Collider;

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
		assert(user0 != nullptr);
		const auto g0 = static_cast<GameObject*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<GameObject*>(user1);

		g0->OnTriggerEnter(_c1);
		g1->OnTriggerEnter(_c0);
	}

	void EventCallbackSystem::OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<GameObject*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<GameObject*>(user1);

		g0->OnTriggerExit(_c1);
		g1->OnTriggerExit(_c0);
	}

	void EventCallbackSystem::OnCollisionEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<GameObject*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<GameObject*>(user1);

		g0->OnCollisionEnter(_collision, _c1);
		g1->OnCollisionEnter(_collision, _c0);
	}

	void EventCallbackSystem::OnCollisionStay(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<GameObject*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<GameObject*>(user1);

		g0->OnCollisionStay(_collision, _c1);
		g1->OnCollisionStay(_collision, _c0);
	}

	void EventCallbackSystem::OnCollisionExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<GameObject*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<GameObject*>(user1);

		g0->OnCollisionExit(_collision, _c1);
		g1->OnCollisionExit(_collision, _c0);
	}
}
