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

		if (!maskTest0)
		{
			return physx::PxFilterFlag::eKILL;
		}

		// 충돌 한 것 중에 하나라도 트리거가 있으면
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;

			return physx::PxFilterFlag::eDEFAULT;
		}

		// 키네마틱 액터에 대한 이벤트를 전송하되, 접촉을 해결 안 함.
		if (physx::PxFilterObjectIsKinematic(attributes0) && physx::PxFilterObjectIsKinematic(attributes1))
		{
			return physx::PxFilterFlag::eSUPPRESS;
		}

		pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
		pairFlags |= physx::PxPairFlag::ePRE_SOLVER_VELOCITY;
		pairFlags |= physx::PxPairFlag::ePOST_SOLVER_VELOCITY;
		pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;

		return physx::PxFilterFlag::eDEFAULT;
	}
}
