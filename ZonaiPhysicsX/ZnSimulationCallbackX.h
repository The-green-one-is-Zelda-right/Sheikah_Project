#pragma once
#include <PxSimulationEventCallback.h>
#include "ZnSimulationCallback.h"
#include <map>
#include <unordered_map>

namespace ZonaiPhysics
{
	class ZnSimulationCallback;

	class ZnSimulationCallbackX : public physx::PxSimulationEventCallback
	{
	public:
		void SetCallbackInstace(ZnSimulationCallback*);

	private:
		// std::unordered_map<std::pair<ZnCollider*, ZnCollider*>, bool> triggerMap;

	public:
		void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;

	private:
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

	private:
		static ZnSimulationCallback* callback;
		static ZnCollider* GetCollider(physx::PxShape* _shape);
	};
}


