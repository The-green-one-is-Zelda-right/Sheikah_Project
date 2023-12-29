#include "ZnSimulationCallbackX.h"

#include <PxConstraint.h>
#include <PxShape.h>

#include "ZnCollider.h"
#include "ZnCollision.h"
#include "ZnRigidBody.h"


namespace ZonaiPhysics
{
	// 웨이크 콜백
	void ZnSimulationCallbackX::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		for (auto i = 0; i < count; i++)
			callback->OnWake(static_cast<ZnRigidBody*>(actors[i]->userData));
	}

	// 슬리프 콜백
	void ZnSimulationCallbackX::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		for (auto i = 0; i < count; i++)
			callback->OnSleep(static_cast<ZnRigidBody*>(actors[i]->userData));
	}

	// 트리거 콜백
	void ZnSimulationCallbackX::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		using namespace physx;

		for (auto i = 0; i < count; i++)
		{
			const PxTriggerPair& tp = pairs[i];

			const auto trigger = static_cast<ZnCollider*>(tp.triggerShape->userData);
			const auto other = static_cast<ZnCollider*>(tp.otherShape->userData);

			// auto itr = triggerMap.find(std::make_pair(trigger, other));


			if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				// 충돌한 친구랑 안한 친구 정보를 넘겨줘야 함.
				callback->OnTriggerEnter(trigger, other);
			}
			//else if(tp.status & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			//{
			//	
			//}
			else if(tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				callback->OnTriggerExit(trigger, other);
			}
		}
	}

	// 제약 파괴 콜백
	void ZnSimulationCallbackX::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		// 파괴된 제약의 정보를 넘겨줌
		for (auto i = 0; i < count; i++)
		{
			callback->OnConstraintBreak(static_cast<ZnJoint*>(constraints[i].constraint->userData));
		}
	}

	// 충돌 콜벡
	// Header : 
	// Pair	  : 
	// Pairs  : 
	void ZnSimulationCallbackX::onContact(const physx::PxContactPairHeader& pairHeader,
		const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		using namespace physx;

		for (auto i = 0; i < nbPairs; i++)
		{
			const PxContactPair& cp = pairs[i];

			PxContactPairPoint buffer;
			cp.extractContacts(&buffer, cp.contactCount);
			const auto* collision = reinterpret_cast<ZnCollision*>(&buffer);

			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				callback->OnCollisionEnter(
					static_cast<ZnCollider*>(cp.shapes[0]->userData), 
					static_cast<ZnCollider*>(cp.shapes[1]->userData),
					*collision
				);
			}
			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				callback->OnCollisionStay(
					static_cast<ZnCollider*>(cp.shapes[0]->userData),
					static_cast<ZnCollider*>(cp.shapes[1]->userData),
					*collision
				);
			}
			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				callback->OnCollisionExit(
					static_cast<ZnCollider*>(cp.shapes[0]->userData),
					static_cast<ZnCollider*>(cp.shapes[1]->userData),
					*collision
				);
			}
		}
	}

	void ZnSimulationCallbackX::onAdvance(const physx::PxRigidBody* const* bodyBuffer,
		const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		// 구현 안함.
	}
}
