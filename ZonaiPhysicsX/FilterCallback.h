#pragma once
#include <PxFiltering.h>

namespace ZonaiPhysics
{
	class FilterCallback : public physx::PxSimulationFilterCallback
	{
	public:
		physx::PxFilterFlags pairFound(
			physx::PxU64 pairID, 
			physx::PxFilterObjectAttributes attributes0,physx::PxFilterData filterData0, const physx::PxActor* a0, const physx::PxShape* s0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, const physx::PxActor* a1,const physx::PxShape* s1, 
			physx::PxPairFlags& pairFlags) override;

		void pairLost(
			physx::PxU64 pairID, 
			physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, 
			bool objectRemoved) override;

		bool statusChange(
			physx::PxU64& pairID, 
			physx::PxPairFlags& pairFlags,
			physx::PxFilterFlags& filterFlags) override;
	};

    physx::PxFilterFlags FilterShader(
        physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
        physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);
}

