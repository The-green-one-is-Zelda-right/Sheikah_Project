#pragma once

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#pragma warning (pop)

class D1SpringFlexJoint : public physx::PxConstraintConnector
{
public:
	static const physx::PxU32 TYPE_ID = physx::PxConcreteType::eFIRST_USER_EXTENSION;

	D1SpringFlexJoint(physx::PxPhysics& physics,
					  physx::PxRigidBody& body0, const physx::PxTransform& localFrame0,
					  physx::PxRigidBody& body1, const physx::PxTransform& localFrame1);

	void release();

	// attribute accessor and mutators

	void setConstraintFlag(physx::PxConstraintFlag::Enum, bool);
	void setConstraintFlags(physx::PxConstraintFlags);

	physx::PxTransform getLocalPose(physx::PxU32);
	void setLocalPose(physx::PxU32, const physx::PxTransform& _trans);

	physx::PxVec3 getRelativeLinearVelocity();
	physx::PxVec3 getRelativeAngularVelocity();

	void setBreakForce(float, float);
	void getBreakForce(float&, float&);

	void setSpringStiffness(physx::PxReal stiffness);
	physx::PxReal getSpringStiffness() const;

	void setSpringDamping(physx::PxReal damping);
	physx::PxReal getSpringDamping() const;

	void setLinearLimit(physx::PxReal limit);
	physx::PxReal getLinearLimit() const;

	// PxConstraintConnector boilerplate

	void* prepareData() override;
	void onConstraintRelease() override;
	void onComShift(physx::PxU32 actor) override;
	void onOriginShift(const physx::PxVec3& shift) override;
	void* getExternalReference(physx::PxU32& typeID) override;

	bool updatePvdProperties(physx::pvdsdk::PvdDataStream&,
		const physx::PxConstraint*,
		physx::PxPvdUpdateType::Enum) const override {
		return true;
	}
	void updateOmniPvdProperties() const override { }
	physx::PxBase* getSerializable() override { return NULL; }

	physx::PxConstraintSolverPrep getPrep() const override;
	const void* getConstantBlock() const override { return &mData; }

	struct CustomSpringJointData
	{
		physx::PxTransform c2b[2];

		physx::PxReal stiffness;
		physx::PxReal damping;
		
		physx::PxReal linearLimit;
	};

	physx::PxRigidBody* mBody[2];
	physx::PxTransform mLocalPose[2];

	physx::PxConstraint* mConstraint;
	CustomSpringJointData mData;

	void* userData = nullptr;

	~D1SpringFlexJoint() override {}
};
