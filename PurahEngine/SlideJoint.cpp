#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "SlideJoint.h"

#include <numbers>


namespace PurahEngine
{
	SlideJoint::~SlideJoint()
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

	void SlideJoint::PostInitialize()
	{
		using namespace ZonaiPhysics;

		const auto& instance = PhysicsSystem::GetInstance();

		assert(body != nullptr);

		ZonaiPhysics::ZnRigidBody* connect = nullptr;

		if (connectedBody)
		{
			connect = connectedBody->body;
		}

		joint = instance.CreateSlideJoint(
			body->body, ZnTransform(LocalAnchor, LocalAnchorRotation),
			connect, ZnTransform(connectedLocalAnchor, connectedLocalAnchorRotation)
		);

		PhysicsSystem::GetInstance().joints.push_back(this);
		joint->SetUserData(this);

		// spring
		if (useSpring)
		{
			float setSpring = 0.f;
			float setDamper = 0.f;

			if (useSpring)
			{
				setSpring = spring;
				setDamper = damper;
			}

			SetSpringArg(setSpring, setDamper);
		}

		// limit
		SetLimitEnable(useLimit);
		//if (useLimit)
		//{
		//	float setMinLimit = -FLT_MAX / 3.f;
		//	float setMaxLimit = FLT_MAX / 3.f;
		//	float bounce = 0.f;

		//	if (useMinDistance)
		//	{
		//		setMinLimit = minDistance;
		//	}
		//	if (useMaxDistance)
		//	{
		//		setMaxLimit = maxDistance;
		//	}

		//	if (bounciness != 0.f)
		//	{
		//		bounce = bounciness;
		//	}
			
			// SetLimit(setMinLimit, setMaxLimit);
		SetLimit(minDistance, maxDistance);
			// SetBounciness(bounce);
		SetBounciness(bounciness);
		//}

		JointT::PostInitialize();
	}

	float SlideJoint::GetPosition() const
	{
		return joint->GetPosition();
	}

	float SlideJoint::GetVelocity() const
	{
		return joint->GetVelocity();
	}

	void SlideJoint::SetLimit(float _lower, float _upper) const
	{
		joint->SetLimit(_lower, _upper);
	}

	void SlideJoint::SetMinDistance(float _lower) const
	{
		joint->SetMinDistance(_lower);
	}

	void SlideJoint::SetMaxDistance(float _upper) const
	{
		joint->SetMinDistance(_upper);
	}

	void SlideJoint::SetSpringArg(float _stiffness, float _damping) const
	{
		joint->SetSpringArg(_stiffness, _damping);
	}

	void SlideJoint::SetBounciness(float _bounciness) const
	{
		joint->SetRestitution(_bounciness);
	}

	void SlideJoint::SetLimitEnable(bool _value) const
	{
		joint->SetLimitEnable(_value);
	}

	bool SlideJoint::IsLimitEnabled() const
	{
		return joint->IsLimitEnabled();
	}

	void SlideJoint::PreSerialize(json& jsonData) const
	{

	}

	void SlideJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);
		PREDESERIALIZE_QUATERNIONF(LocalAnchorRotation);

		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);
		PREDESERIALIZE_QUATERNIONF(connectedLocalAnchorRotation);

		PREDESERIALIZE_VALUE(useSpring);
		PREDESERIALIZE_VALUE(spring);
		PREDESERIALIZE_VALUE(damper);

		PREDESERIALIZE_VALUE(useLimit);
		PREDESERIALIZE_VALUE(useMinDistance);
		PREDESERIALIZE_VALUE(minDistance);
		PREDESERIALIZE_VALUE(useMaxDistance);
		PREDESERIALIZE_VALUE(maxDistance);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void SlideJoint::PostSerialize(json& jsonData) const
	{

	}

	void SlideJoint::PostDeserialize(const json& jsonData)
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
