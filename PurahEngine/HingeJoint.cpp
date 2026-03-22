#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "HingeJoint.h"

namespace PurahEngine
{
	HingeJoint::~HingeJoint()
	{
		auto& instance = PhysicsSystem::GetInstance();

		if (joint)
		{
			instance.FreeObject(joint, GetGameObject());
			joint = nullptr;
		}

		auto iter = std::ranges::find(instance.joints, this);
		if (iter != instance.joints.end())
		{
			instance.joints.erase(std::ranges::find(instance.joints, this));
		}
	}

	void HingeJoint::PostInitialize()
	{
		using namespace ZonaiPhysics;

		const auto& instance = PhysicsSystem::GetInstance();

		assert(body != nullptr);

		ZonaiPhysics::ZnRigidBody* connect = nullptr;

		if (connectedBody)
		{
			connect = connectedBody->body;
		}

		joint = instance.CreateHingeJoint(
			body->body, ZnTransform(LocalAnchor, LocalAnchorRotation),
			connect, ZnTransform(connectedLocalAnchor, connectedLocalAnchorRotation)
		);

		PhysicsSystem::GetInstance().joints.push_back(this);
		joint->SetUserData(this);

		if (useSpring)
		{
			SetSpringArg(spring, damper);
		}

		SetLimitEnable(useLimit);
		SetLimit(angleLower, angleUpper);

		SetDriveEnable(setMotor);
		SetDriveVelocity(velocity);
		SetDriveFreespin(freeSpin);

		JointT::PostInitialize();
	}

	float HingeJoint::GetAngle() const
	{
		return joint->GetAngle();
	}

	float HingeJoint::GetVelocity() const
	{
		return joint->GetVelocity();
	}

	void HingeJoint::SetLimit(float _lower, float _upper) const
	{
		joint->SetLimit(_lower, _upper);
	}

	void HingeJoint::GetLimit(float& _upper, float& _lower) const
	{
		joint->GetLimit(_upper, _lower);
	}

	float HingeJoint::GetRestitution() const
	{
		return joint->GetRestitution();
	}

	void HingeJoint::SetRestitution(float _restitution) const
	{
		joint->SetRestitution(_restitution);
	}

	float HingeJoint::GetBounceThreshold() const
	{
		return joint->GetBounceThreshold();
	}

	void HingeJoint::SetBounceThreshold(float _bounceThreshold) const
	{
		joint->SetBounceThreshold(_bounceThreshold);
	}

	void HingeJoint::SetSpringArg(float _stiffness, float _damping) const
	{
		joint->SetSpringArg(_stiffness, _damping);
	}

	void HingeJoint::GetSpringArg(float& _stiffness, float& _damping) const
	{
		joint->GetSpringArg(_stiffness, _damping);
	}

	void HingeJoint::SetDriveVelocity(float _velocity) const
	{
		joint->SetDriveVelocity(_velocity);
	}

	float HingeJoint::GetDriveVelocity() const
	{
		return joint->GetDriveVelocity();
	}

	void HingeJoint::SetDriveForceLimit(float _forceLimit) const
	{
		joint->SetDriveForceLimit(_forceLimit);
	}

	float HingeJoint::GetDriveForceLimit() const
	{
		return joint->GetDriveForceLimit();
	}

	void HingeJoint::SetDriveGearRatio(float _gearRatio) const
	{
		joint->SetDriveGearRatio(_gearRatio);
	}

	float HingeJoint::GetDriveGearRatio() const
	{
		return joint->GetDriveGearRatio();
	}

	void HingeJoint::SetLimitEnable(bool _value) const
	{
		joint->SetLimitEnable(_value);
	}

	void HingeJoint::SetDriveEnable(bool _value) const
	{
		joint->SetDriveEnable(_value);
	}

	void HingeJoint::SetDriveFreespin(bool _value) const
	{
		joint->SetDriveFreespin(_value);
	}

	void HingeJoint::PreSerialize(json& jsonData) const
	{

	}

	void HingeJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);
		PREDESERIALIZE_QUATERNIONF(LocalAnchorRotation);
		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);
		PREDESERIALIZE_QUATERNIONF(connectedLocalAnchorRotation);

		PREDESERIALIZE_VALUE(useSpring);
		PREDESERIALIZE_VALUE(spring);
		PREDESERIALIZE_VALUE(damper);
		PREDESERIALIZE_VALUE(targetPosition);

		PREDESERIALIZE_VALUE(setMotor);
		PREDESERIALIZE_VALUE(velocity);
		PREDESERIALIZE_VALUE(force);
		PREDESERIALIZE_VALUE(freeSpin);

		PREDESERIALIZE_VALUE(useLimit);
		PREDESERIALIZE_VALUE(angleLower);
		PREDESERIALIZE_VALUE(angleUpper);
		PREDESERIALIZE_VALUE(bounciness);
		PREDESERIALIZE_VALUE(bounceThreshold);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void HingeJoint::PostSerialize(json& jsonData) const
	{

	}

	void HingeJoint::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(body);
		assert(body != nullptr);

		POSTDESERIALIZE_PTR(connectedBody);
		if (connectedBody == body)
		{
			connectedBody = nullptr;
		}
	}
}