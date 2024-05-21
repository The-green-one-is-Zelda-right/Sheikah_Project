#include <PxShape.h>

#include "QueryFilterCallback.h"

#include <vector>

#include "ZnLayer.h"

namespace ZonaiPhysics
{
	physx::PxQueryHitType::Enum QueryFilter::preFilter(
        const physx::PxFilterData& filterData, const physx::PxShape* shape, 
        const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags)
	{
        // shape�� ������ X
        if (!shape)
            return physx::PxQueryHitType::eNONE;

        // Ʈ���Ŵ� X
        if (PxFilterObjectIsTrigger(shape->getFlags()))
            return physx::PxQueryHitType::eNONE;

        // ���̾ �ٸ��� X
        const physx::PxFilterData shapeFilter = shape->getQueryFilterData();

        const bool test = ZnLayer::IsCollide(filterData.word0, shapeFilter.word0);

        if (!test)
            return physx::PxQueryHitType::eNONE;

        return physx::PxQueryHitType::eBLOCK;

        // Check if skip triggers
        //const bool hitTriggers = filterData.word2 != 0;
        //if (!hitTriggers && PxFilterObjectIsTrigger(shape->getFlags()))
        //    return physx::PxQueryHitType::eNONE;

        //const bool blockSingle = filterData.word1 != 0;
        // return blockSingle ? physx::PxQueryHitType::eBLOCK : physx::PxQueryHitType::eTOUCH;
	}

	physx::PxQueryHitType::Enum QueryFilter::postFilter(
        const physx::PxFilterData& filterData, const physx::PxQueryHit& hit, 
        const physx::PxShape* shape, const physx::PxRigidActor* actor)
	{
        // Not used
        return physx::PxQueryHitType::eNONE;
	}


	physx::PxQueryHitType::Enum CharacterQueryFilter::preFilter(
        const physx::PxFilterData& filterData, const physx::PxShape* shape, 
        const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags)
	{
        // Early out to avoid crashing
        if (!shape)
            return physx::PxQueryHitType::eNONE;

        // Let triggers through
        if (PxFilterObjectIsTrigger(shape->getFlags()))
            return physx::PxQueryHitType::eNONE;

        // Trigger the contact callback for pairs (A,B) where the filtermask of A contains the ID of B and vice versa
        const physx::PxFilterData shapeFilter = shape->getQueryFilterData();
        if (filterData.word0 & shapeFilter.word1)
            return physx::PxQueryHitType::eBLOCK;

        return physx::PxQueryHitType::eNONE;
	}

	physx::PxQueryHitType::Enum CharacterQueryFilter::postFilter(
        const physx::PxFilterData& filterData, const physx::PxQueryHit& hit, 
        const physx::PxShape* shape, const physx::PxRigidActor* actor)
	{
        // Not used
        return physx::PxQueryHitType::eNONE;
	}
}
