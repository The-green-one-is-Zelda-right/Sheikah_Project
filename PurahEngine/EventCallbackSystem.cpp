#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnRigidBody.h"
#include "ZnCollider.h"
#include "ZnJoint.h"

#include "Joint.h"
#include "Collider.h"

#include "EventCallbackSystem.h"

namespace PurahEngine
{
	class Joint;
	class Collider;

	void EventCallbackSystem::OnWake(const ZonaiPhysics::ZnRigidBody* _body)
	{
		auto user0 = _body->GetUserData();
		const auto pBody = static_cast<PurahEngine::RigidBody*>(user0);

		// 

		return;
	}

	void EventCallbackSystem::OnSleep(const ZonaiPhysics::ZnRigidBody* _body)
	{
		auto user0 = _body->GetUserData();
		const auto pBody = static_cast<PurahEngine::RigidBody*>(user0);

		// 잠에서 깨어났으니

		return;
	}

	void EventCallbackSystem::OnConstraintBreak(ZonaiPhysics::ZnJoint* _joint)
	{
		void* joint = _joint->GetUserData();
		const auto pJoint = static_cast<PurahEngine::Joint*>(joint);

		pJoint->BreakCallback();

		// 컴포넌트 파괴 해야함.
		pJoint->GetGameObject()->DeleteComponent(pJoint);
	}

	void EventCallbackSystem::OnTriggerEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		purahCollider0->GetGameObject()->OnTriggerEnter(purahCollider1);
		purahCollider1->GetGameObject()->OnTriggerEnter(purahCollider0);
	}

	void EventCallbackSystem::OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		purahCollider0->GetGameObject()->OnTriggerExit(purahCollider1);
		purahCollider1->GetGameObject()->OnTriggerExit(purahCollider0);
	}

	void EventCallbackSystem::OnCollisionEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision, const ZonaiPhysics::ZnCollision& _otherCol)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		purahCollider0->GetGameObject()->OnCollisionEnter(_collision, purahCollider1);
		purahCollider1->GetGameObject()->OnCollisionEnter(_otherCol, purahCollider0);
	}

	void EventCallbackSystem::OnCollisionStay(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision, const ZonaiPhysics::ZnCollision& _otherCol)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		purahCollider0->GetGameObject()->OnCollisionStay(_collision, purahCollider1);
		purahCollider1->GetGameObject()->OnCollisionStay(_otherCol, purahCollider0);
	}

	void EventCallbackSystem::OnCollisionExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision, const ZonaiPhysics::ZnCollision& _otherCol)
	{
		const auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		const auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		purahCollider0->GetGameObject()->OnCollisionExit(_collision, purahCollider1);
		purahCollider1->GetGameObject()->OnCollisionExit(_otherCol, purahCollider0);
	}
}
