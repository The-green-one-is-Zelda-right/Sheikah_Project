#pragma once
#include <PxQueryFiltering.h>

namespace ZonaiPhysics
{
	class QueryFilter : public physx::PxQueryFilterCallback
	{
	public:
		physx::PxQueryHitType::Enum preFilter(
			const physx::PxFilterData& filterData, const physx::PxShape* shape,
			const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) override;

		physx::PxQueryHitType::Enum postFilter(
			const physx::PxFilterData& filterData, const physx::PxQueryHit& hit,
			const physx::PxShape* shape, const physx::PxRigidActor* actor) override;
	};

	class CharacterQueryFilter : public physx::PxQueryFilterCallback
	{
	public:
		physx::PxQueryHitType::Enum preFilter(
			const physx::PxFilterData& filterData, const physx::PxShape* shape,
			const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) override;

		physx::PxQueryHitType::Enum postFilter(
			const physx::PxFilterData& filterData, const physx::PxQueryHit& hit,
			const physx::PxShape* shape, const physx::PxRigidActor* actor) override;
	};
}
