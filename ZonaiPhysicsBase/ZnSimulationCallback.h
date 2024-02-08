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

		// ÀÏ¾î³²
		virtual void OnWake(ZnRigidBody const*) = 0;

		// ¼ö¸é
		virtual void OnSleep(ZnRigidBody const*) = 0;

		// Á¦¾à ÆÄ±«
		virtual void OnConstraintBreak(ZnJoint const*) = 0;

		virtual void OnTriggerEnter(ZnCollider const*, ZnCollider const*) = 0;
		virtual void OnTriggerStay(ZnCollider const*, ZnCollider const*) = 0;
		virtual void OnTriggerExit(ZnCollider const*, ZnCollider const*) = 0;

		virtual void OnCollisionEnter(ZnCollider const*, ZnCollider const*, const ZnCollision&) = 0;
		virtual void OnCollisionStay(ZnCollider const*, ZnCollider const*, const ZnCollision&) = 0;
		virtual void OnCollisionExit(ZnCollider const*, ZnCollider const*, const ZnCollision&) = 0;
	};
}

/**
 * Èì
 * 
 * 
 * 
 */