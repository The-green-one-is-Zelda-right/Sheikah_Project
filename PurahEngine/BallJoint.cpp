#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "BallJoint.h"


namespace PurahEngine
{
	BallJoint::~BallJoint()
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

	void BallJoint::PostInitialize()
	{
		using namespace ZonaiPhysics;

		const auto& instance = PhysicsSystem::GetInstance();

		assert(body != nullptr);

		ZonaiPhysics::ZnRigidBody* connect = nullptr;

		if (connectedBody)
		{
			connect = connectedBody->body;
		}

		joint = instance.CreateBallJoint(
			body->body, ZnTransform(LocalAnchor, LocalAnchorRotation),
			connect, ZnTransform(connectedLocalAnchor, connectedLocalAnchorRotation)
		);

		PhysicsSystem::GetInstance().joints.push_back(this);
		joint->SetUserData(this);

		LimitEnable(useLimit);
		SetLimitAngle(swingLimitY, swingLimitZ);

		// spring
		{
			float setSpring = 0.f;
			float setDamper = 0.f;
			float setBounce = 0.f;
			if (useSpring)
			{
				setSpring = spring;
				setDamper = damping;
				setBounce = bounciness;
			}
			SetSpringArg(setSpring, setDamper);
			SetBounciness(setBounce);
		}

		JointT::PostInitialize();
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

	void BallJoint::SetLimitAngle(float _yAngle, float _zAngle) const
	{
		joint->SetLimitAngle(_yAngle, _zAngle);
	}

	void BallJoint::SetLimitRadian(float _yRadian, float _zRadian) const
	{
		joint->SetLimitRadian(_yRadian, _zRadian);
	}

	void BallJoint::SetSpringArg(float _stiffness, float _damping) const
	{
		joint->SetSpringArg(_stiffness, _damping);
	}

	void BallJoint::SetBounciness(float _bounciness) const
	{
		joint->SetRestitution(_bounciness);
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
		POSTDESERIALIZE_PTR(body);
		assert(body != nullptr);

		POSTDESERIALIZE_PTR(connectedBody);
		if (connectedBody == body)
		{
			connectedBody = nullptr;
		}
	}
}
