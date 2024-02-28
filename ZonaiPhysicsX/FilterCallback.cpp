#include "FilterCallback.h"

#include "ZnLayer.h"
#include "ZnPhysicsX.h"


namespace ZonaiPhysics
{
	physx::PxFilterFlags FilterCallback::pairFound(
		physx::PxU64 pairID,
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, const physx::PxActor* a0, const physx::PxShape* s0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, const physx::PxActor* a1, const physx::PxShape* s1,
		physx::PxPairFlags& pairFlags)
	{
		using namespace physx;

		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;

			return PxFilterFlag::eDEFAULT;
		}

		if (PxFilterObjectIsKinematic(attributes0) || PxFilterObjectIsKinematic(attributes1))
		{
			pairFlags = PxPairFlag::eCONTACT_DEFAULT;

			return PxFilterFlag::eDEFAULT;
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		}

		return PxFilterFlag::eDEFAULT;
	}

	void FilterCallback::pairLost(
		physx::PxU64 pairID, 
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		bool objectRemoved)
	{

	}

	bool FilterCallback::statusChange(
		physx::PxU64& pairID, 
		physx::PxPairFlags& pairFlags,
		physx::PxFilterFlags& filterFlags)
	{
		return false;
	}

	physx::PxFilterFlags FilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, 
		physx::PxPairFlags& pairFlags,
		const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		const bool maskTest0 = ZnLayer::CanCollide(filterData0.word0, filterData1.word0);
		// const bool maskTest0 = (filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1);

		// Let triggers through
		// 트리거를 통과시킵니다.
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			if (maskTest0)
			{
				// Notify trigger if masks specify it
				// 마스크가 그렇게 지정하면 트리거에 알림.
				pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
				pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			}
			pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		// Send events for the kinematic actors but don't solve the contact
		// 키네마틱 액터에 대한 이벤트를 전송하되, 접촉을 해결하지 마세요.
		if (physx::PxFilterObjectIsKinematic(attributes0) && physx::PxFilterObjectIsKinematic(attributes1))
		{
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
			return physx::PxFilterFlag::eSUPPRESS;
		}

		// Trigger the contact callback for pairs (A,B) where the filtermask of A contains the ID of B and vice versa
		// A의 필터마스크가 B의 ID를 포함하고 B의 필터마스크가 A의 ID를 포함하는 쌍 (A, B)에 대한 contact 콜백을 트리거합니다.
		if (maskTest0)
		{
			pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT;
			pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			pairFlags |= physx::PxPairFlag::ePOST_SOLVER_VELOCITY;
			pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
			return physx::PxFilterFlag::eDEFAULT;
		}

		// Ignore pair (no collisions nor events)
		return physx::PxFilterFlag::eKILL;
	}
}
