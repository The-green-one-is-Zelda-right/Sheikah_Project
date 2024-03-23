#include "BallJoint.h"

#include "PhysicsSystem.h"


namespace PurahEngine
{
	BallJoint::~BallJoint()
	{

	}

	void BallJoint::Initialize()
	{

	}

	void BallJoint::OnDataLoadComplete()
	{

	}

	void BallJoint::GetLimitAngle(float& _outY, float& _outZ) const
	{
		joint->GetLimitAngle(_outY, _outZ);
	}

	float BallJoint::GetYAngle() const
	{
		return joint->GetYAngle();
	}

	float BallJoint::GetZAngle() const
	{
		return joint->GetZAngle();
	}

	void BallJoint::LimitEnable(bool _value) const
	{
		joint->LimitEnable(_value);
	}

	void BallJoint::SetLimitCone(float _yAngle, float _zAngle) const
	{
		joint->SetLimitCone(_yAngle, _zAngle);
	}

	void BallJoint::SetSpringArg(float _stiffness, float _damping) const
	{
		joint->SetSpringArg(_stiffness, _damping);
	}

	void BallJoint::PreSerialize(json& jsonData) const
	{

	}

	void BallJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);
		PREDESERIALIZE_QUATERNIONF(LocalAnchorRotation);
		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);
		PREDESERIALIZE_QUATERNIONF(connectedLocalAnchorRotation);

		PREDESERIALIZE_VALUE(useSpring);
		PREDESERIALIZE_VALUE(spring);
		PREDESERIALIZE_VALUE(damping);

		PREDESERIALIZE_VALUE(useLimit);
		PREDESERIALIZE_VALUE(swingLimitY);
		PREDESERIALIZE_VALUE(swingLimitZ);
		PREDESERIALIZE_VALUE(bounciness);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void BallJoint::PostSerialize(json& jsonData) const
	{

	}

	void BallJoint::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(connectedBody);
	}
}
