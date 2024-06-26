#pragma once
#include "ZnSimulationCallback.h"

namespace ZonaiPhysics
{
	class ZnCollider;
	class ZnCollision;
}

namespace PurahEngine
{
	class EventCallbackSystem : public ZonaiPhysics::ZnSimulationCallback
	{
	public:
		void OnWake(const ZonaiPhysics::ZnRigidBody*) override;

		void OnSleep(const ZonaiPhysics::ZnRigidBody*) override;

		void OnConstraintBreak(ZonaiPhysics::ZnJoint*) override;

		void OnTriggerEnter(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*) override;

		void OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*) override;

		void OnTriggerExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*) override;

		void OnCollisionEnter(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*, 
			const ZonaiPhysics::ZnCollision&, const ZonaiPhysics::ZnCollision&) override;

		void OnCollisionStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*, 
			const ZonaiPhysics::ZnCollision&, const ZonaiPhysics::ZnCollision&) override;

		void OnCollisionExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*, 
			const ZonaiPhysics::ZnCollision&, const ZonaiPhysics::ZnCollision&) override;
	};
}

