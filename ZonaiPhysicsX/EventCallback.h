#pragma once
#include <PxSimulationEventCallback.h>

namespace ZonaiPhysics
{
	class ZnCollider;
	class ZnSimulationCallback;

	class EventCallback : public physx::PxSimulationEventCallback
	{
		static ZnSimulationCallback* callback;

	public:
		void setInstance(ZnSimulationCallback*);

	public:
		void onConstraintBreak(
			physx::PxConstraintInfo* constraints, 
			physx::PxU32 count) override;

		void onWake(
			physx::PxActor** actors, 
			physx::PxU32 count) override;

		void onSleep(
			physx::PxActor** actors, 
			physx::PxU32 count) override;

		void onContact(
			const physx::PxContactPairHeader& pairHeader, 
			const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;

		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;

		void onAdvance(
			const physx::PxRigidBody* const* bodyBuffer,
			const physx::PxTransform* poseBuffer,
			const physx::PxU32 count) override;

	private:
		static ZnCollider* GetCollider(physx::PxShape* _shape);
	};

	class CharacterControllerCallback : public physx::PxControllerFilterCallback
	{
		static physx::PxShape* getShape(const physx::PxController& controller)
		{
			physx::PxRigidDynamic* actor = controller.getActor();

			// Early out if no actor or no shapes
			if (!actor || actor->getNbShapes() < 1)
				return nullptr;

			// Get first shape only
			physx::PxShape* shape = nullptr;
			actor->getShapes(&shape, 1);

			return shape;
		}

	public:
		bool filter(const physx::PxController& a, const physx::PxController& b) override;
	};
}


