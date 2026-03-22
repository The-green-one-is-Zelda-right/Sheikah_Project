#pragma once

namespace ZonaiPhysics
{
	class ZnRigidBody;
	class ZnCollider;
	class ZnJoint;
	struct ZnCollision;
}

namespace ZonaiPhysics
{
	class ZnSimulationCallback
	{
	public:
		virtual ~ZnSimulationCallback() = default;

		// 일어남
		virtual void OnWake(ZnRigidBody const*) = 0;

		// 수면
		virtual void OnSleep(ZnRigidBody const*) = 0;

		// 제약 파괴
		virtual void OnConstraintBreak(ZnJoint*) = 0;

		virtual void OnTriggerEnter(ZnCollider const*, ZnCollider const*) = 0;
		virtual void OnTriggerStay(ZnCollider const*, ZnCollider const*) = 0;
		virtual void OnTriggerExit(ZnCollider const*, ZnCollider const*) = 0;

		virtual void OnCollisionEnter(ZnCollider const*, ZnCollider const*, const ZnCollision&, const ZnCollision&) = 0;
		virtual void OnCollisionStay(ZnCollider const*, ZnCollider const*, const ZnCollision&, const ZnCollision&) = 0;
		virtual void OnCollisionExit(ZnCollider const*, ZnCollider const*, const ZnCollision&, const ZnCollision&) = 0;
	};
}

/**
 * 흠
 * 
 * 
 * 
 */