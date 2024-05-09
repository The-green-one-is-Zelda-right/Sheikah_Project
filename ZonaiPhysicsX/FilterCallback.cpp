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
		// Ʈ���Ÿ� �����ŵ�ϴ�.
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			if (maskTest0)
			{
				// Notify trigger if masks specify it
				// ����ũ�� �׷��� �����ϸ� Ʈ���ſ� �˸�.
				pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
				pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			}
			pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
			pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

			return physx::PxFilterFlag::eDEFAULT;
		}

		// Send events for the kinematic actors but don't solve the contact
		// Ű�׸�ƽ ���Ϳ� ���� �̺�Ʈ�� �����ϵ�, ������ �ذ����� ������.
		if (physx::PxFilterObjectIsKinematic(attributes0) && physx::PxFilterObjectIsKinematic(attributes1))
		{
			//pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			//pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			//pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;

			//pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
			//pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

			//pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;

			//return physx::PxFilterFlag::eSUPPRESS;

			return physx::PxFilterFlag::eKILL;
		}

		// Trigger the contact callback for pairs (A,B) where the filtermask of A contains the ID of B and vice versa
		// A�� ���͸���ũ�� B�� ID�� �����ϰ� B�� ���͸���ũ�� A�� ID�� �����ϴ� �� (A, B)�� ���� contact �ݹ��� Ʈ�����մϴ�.
		if (maskTest0)
		{
			pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT;

			pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
			pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;

			pairFlags |= physx::PxPairFlag::ePRE_SOLVER_VELOCITY;
			pairFlags |= physx::PxPairFlag::ePOST_SOLVER_VELOCITY;

			pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;

			return physx::PxFilterFlag::eDEFAULT;
		}

		// Ignore pair (no collisions nor events)
		return physx::PxFilterFlag::eKILL;
	}
}
