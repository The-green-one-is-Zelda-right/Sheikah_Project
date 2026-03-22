#include "D1SpringFlexJoint.h"
#include <assert.h>
#include "PxConstraint.h"

// #include "../physx/source/physxextensions/src/ExtConstraintHelper.h"

using namespace physx;


void setupLinearSpring(Px1DConstraint* c, 
						const PxVec3& direction,
						const PxTransform& cA2w, 
						const PxTransform& bA2w,
						const PxTransform& cB2w,
						const PxTransform& bB2w,
						const PxReal stiffness,
						const PxReal damping,
						const PxReal linearError)
{
	c->flags = Px1DConstraintFlag::eOUTPUT_FORCE | Px1DConstraintFlag::eSPRING;
	c->linear0 = -direction;
	// c->angular0 = (cA2w.p - bA2w.p).cross(c->linear0);
	c->angular0 = PxVec3(0.0f);
	c->linear1 = direction;
	// c->angular1 = (cB2w.p - bB2w.p).cross(c->linear1);
	c->angular1 = PxVec3(0.0f);
	c->geometricError = linearError;
	c->mods.spring.stiffness = stiffness;
	c->mods.spring.damping = damping;
	c->minImpulse = -PX_MAX_F32;
	c->maxImpulse = PX_MAX_F32;
}

void setupAngularSpring(Px1DConstraint* c, 
						const PxVec3& angularAxis, 
						const PxReal stiffness,
						const PxReal damping, 
						const PxReal angularError) 
{
	c->flags = Px1DConstraintFlag::eOUTPUT_FORCE | Px1DConstraintFlag::eSPRING;
	c->linear0 = PxVec3(0.0f);
	c->angular0 = angularAxis;
	c->linear1 = PxVec3(0.0f);
	c->angular1 = -angularAxis;
	c->geometricError = angularError;
	c->mods.spring.stiffness = stiffness;
	c->mods.spring.damping = damping;
	c->minImpulse = -PX_MAX_F32;
	c->maxImpulse = PX_MAX_F32;
}

void setupHardConstraint(Px1DConstraint* c,
						const PxVec3& direction, 
						const PxTransform& cA2w, 
						const PxTransform& bA2w, 
						const PxTransform& cB2w, 
						const PxTransform& bB2w, 
						const PxReal geometricError) 
{
	c->flags = Px1DConstraintFlag::eOUTPUT_FORCE;
	c->linear0 = direction;
	c->angular0 = (cA2w.p - bA2w.p).cross(c->linear0);
	c->linear1 = -direction;
	c->angular1 = (cB2w.p - bB2w.p).cross(c->linear1);
	c->geometricError = geometricError;
	c->minImpulse = 0.0f;
	c->maxImpulse = PX_MAX_F32;
}

bool isFinite(PxReal value) 
{
	return PxIsFinite(value);
}

static PxU32 solverPrep(Px1DConstraint* constraints,
						PxVec3p& body0WorldOffset,
						PxU32 maxConstraints,
						PxConstraintInvMassScale& invMassScale,
						const void* constantBlock,
						const PxTransform& bA2w,
						const PxTransform& bB2w,
						bool useExtendedLimits,
						PxVec3p& cA2wOut, PxVec3p& cB2wOut)
{
	PX_UNUSED(maxConstraints);

	const D1SpringFlexJoint::CustomSpringJointData& data = *reinterpret_cast<const D1SpringFlexJoint::CustomSpringJointData*>(constantBlock);

	// 안전 검사
	// if (!isFinite(data.stiffness) || !isFinite(data.damping) || !isFinite(data.minDistance) || !isFinite(data.maxDistance)) {
	// 	return 0;
	// }

	// physx::Ext::joint::ConstraintHelper ch(constraints, invMassScale, cA2w, cB2w, body0WorldOffset, data, bA2w, bB2w);

	// 두 물체의 로컬 프레임을 월드 프레임으로 변환
	PxTransform cA2w = bA2w.transform(data.c2b[0]);
	PxTransform cB2w = bB2w.transform(data.c2b[1]);

	// 결과를 출력 변수에 저장
	cA2wOut = cA2w.p;
	cB2wOut = cB2w.p;

	// 월드 오프셋 계산
	body0WorldOffset = cB2w.p - bA2w.p;

	// 선형 방향 및 거리 계산
	PxVec3 linearDirection = cB2w.p - cA2w.p;
	const PxReal linearDistance = linearDirection.normalize();

	// 회전 차이 계산
	PxQuat relativeRotation = cB2w.q.getConjugate() * cA2w.q;
	PxVec3 angularAxis;
	PxReal angularAngle;
	relativeRotation.toRadiansAndUnitAxis(angularAngle, angularAxis);

	// 오류 값 설정
	PxReal linearError = -linearDistance;
	PxReal angularError = -angularAngle;

	PxU32 constraintCount = 0;
	Px1DConstraint* c = constraints;

	// 선형 스프링 설정
	setupLinearSpring(c, linearDirection, cA2w, bA2w, cB2w, bB2w, data.stiffness, data.damping, linearError);
	++c;
	++constraintCount;

	// 최대 거리 제약 설정
	// if (linearDistance > data.maxDistance) 
	// {
	// 	setupHardConstraint(c, linearDirection, cA2w, bA2w, cB2w, bB2w, linearDistance - data.maxDistance);
	// 	++c;
	// 	++constraintCount;
	// }
	// 
	// // 최소 거리 제약 설정
	// if (linearDistance < data.minDistanwce) 
	// {
	// 	setupHardConstraint(c, -linearDirection, cA2w, bA2w, cB2w, bB2w, data.minDistance - linearDistance);
	// 	++c;
	// 	++constraintCount;
	// }

	// 회전 스프링 설정
	setupAngularSpring(c, angularAxis, data.stiffness, data.damping, angularError);
	++constraintCount;

	return constraintCount; // 생성된 제약 조건의 수 반환
}

static void visualize(PxConstraintVisualizer& viz,
	const void* constantBlock,
	const PxTransform& body0Transform,
	const PxTransform& body1Transform,
	PxU32 flags)
{
	PX_UNUSED(flags);
	const D1SpringFlexJoint::CustomSpringJointData& data = *reinterpret_cast<const D1SpringFlexJoint::CustomSpringJointData*>(constantBlock);

	PxTransform cA2w = body0Transform * data.c2b[0];
	PxTransform cB2w = body1Transform * data.c2b[1];
	viz.visualizeJointFrames(cA2w, cB2w);
}

static PxConstraintShaderTable sShaderTable = { solverPrep, visualize, PxConstraintFlag::Enum(0) };

PxConstraintSolverPrep D1SpringFlexJoint::getPrep() const { return solverPrep; }

D1SpringFlexJoint::D1SpringFlexJoint(physx::PxPhysics& physics, PxRigidBody& body0, const PxTransform& localFrame0,
	PxRigidBody& body1, const PxTransform& localFrame1)
{
	mConstraint = physics.createConstraint(&body0, &body1, *this, sShaderTable, sizeof(CustomSpringJointData));

	mBody[0] = &body0;
	mBody[1] = &body1;

	mLocalPose[0] = localFrame0.getNormalized();
	mLocalPose[1] = localFrame1.getNormalized();

	mData.stiffness = 1000.0f;
	mData.damping = 1000.0f;
	mData.linearLimit = 1.0f;
	mData.c2b[0] = body0.getCMassLocalPose().transformInv(mLocalPose[0]);
	mData.c2b[1] = body1.getCMassLocalPose().transformInv(mLocalPose[1]);
}

void D1SpringFlexJoint::release()
{
	mConstraint->release();
}

void D1SpringFlexJoint::setConstraintFlag(physx::PxConstraintFlag::Enum flag, bool value)
{
	mConstraint->setFlag(flag, value);
	mConstraint->markDirty();
}

void D1SpringFlexJoint::setConstraintFlags(physx::PxConstraintFlags flags)
{
	mConstraint->setFlags(flags);
	mConstraint->markDirty();
}

physx::PxTransform D1SpringFlexJoint::getLocalPose(physx::PxU32 index)
{
	return mLocalPose[index];
}

void D1SpringFlexJoint::setLocalPose(physx::PxU32 index, const physx::PxTransform& _trans)
{
	return mLocalPose[index] = _trans;
	mConstraint->markDirty();
}

physx::PxVec3 D1SpringFlexJoint::getRelativeLinearVelocity()
{
	return {};
}

physx::PxVec3 D1SpringFlexJoint::getRelativeAngularVelocity()
{
	return {};
}

void D1SpringFlexJoint::setBreakForce(float force, float torque)
{
	mConstraint->setBreakForce(force, torque);
	mConstraint->markDirty();
}

void D1SpringFlexJoint::getBreakForce(float& force, float& torque)
{
	mConstraint->getBreakForce(force, torque);
	mConstraint->markDirty();
}

void D1SpringFlexJoint::setSpringStiffness(physx::PxReal stiffness)
{
	mData.stiffness = stiffness;
	mConstraint->markDirty();
}

physx::PxReal D1SpringFlexJoint::getSpringStiffness() const
{
	return mData.stiffness;
}

void D1SpringFlexJoint::setSpringDamping(physx::PxReal damping)
{
	mData.damping = damping;
	mConstraint->markDirty();
}

physx::PxReal D1SpringFlexJoint::getSpringDamping() const
{
	return mData.damping;
}

void D1SpringFlexJoint::setLinearLimit(physx::PxReal limit)
{
	mData.linearLimit = limit;
	mConstraint->markDirty();
}

physx::PxReal D1SpringFlexJoint::getLinearLimit() const
{
	return mData.linearLimit;
}

void* D1SpringFlexJoint::prepareData()
{
	return &mData;
}

void D1SpringFlexJoint::onConstraintRelease()
{
	delete this;
}

void D1SpringFlexJoint::onComShift(PxU32 actor)
{
	mData.c2b[actor] = mBody[actor]->getCMassLocalPose().transformInv(mLocalPose[actor]);
	mConstraint->markDirty();
}

void D1SpringFlexJoint::onOriginShift(const PxVec3& shift)
{
	mLocalPose[0].p -= shift;
	mLocalPose[1].p -= shift;

	mData.c2b[0] = mBody[0]->getCMassLocalPose().transformInv(mLocalPose[0]);
	mData.c2b[1] = mBody[1]->getCMassLocalPose().transformInv(mLocalPose[1]);

	mConstraint->markDirty();
}

void* D1SpringFlexJoint::getExternalReference(PxU32& typeID)
{
	typeID = TYPE_ID;
	return this;
}
