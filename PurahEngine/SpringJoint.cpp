#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "SpringJoint.h"


namespace PurahEngine
{
	SpringJoint::~SpringJoint()
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

	void SpringJoint::PostInitialize()
	{
		using namespace ZonaiPhysics;

		const auto& instance = PhysicsSystem::GetInstance();

		assert(body != nullptr);

		ZnRigidBody* connect = nullptr;

		if (connectedBody)
		{
			connect = connectedBody->body;
		}

		joint = instance.CreateDistanceJoint(
			body->body, ZnTransform(LocalAnchor),
			connect, ZnTransform(connectedLocalAnchor)
		);

		PhysicsSystem::GetInstance().joints.push_back(this);
		joint->SetUserData(this);

		SetSpringEnable(useSpring);
		SetSpringArg(spring, damper);

		SetMinDistanceEnable(useMinDistance);
		SetMinDistance(minDistance);
		SetMaxDistanceEnable(useMaxDistance);
		SetMaxDistance(maxDistance);
		SetTolerance(Tolerance);

		JointT::PostInitialize();
	}

	float SpringJoint::GetDistance() const
	{
		return joint->GetDistance();
	}

	void SpringJoint::SetMinDistance(float _distance) const
	{
		joint->SetMinDistance(_distance);
	}

	float SpringJoint::GetMinDistance() const
	{
		return joint->GetMinDistance();
	}

	void SpringJoint::SetMaxDistance(float _distance) const
	{
		joint->SetMaxDistance(_distance);
	}

	float SpringJoint::GetMaxDistance() const
	{
		return joint->GetMaxDistance();
	}

	void SpringJoint::SetTolerance(float _tolerance) const
	{
		joint->SetTolerance(_tolerance);
	}

	float SpringJoint::GetTolerance() const
	{
		return joint->GetTolerance();
	}

	void SpringJoint::SetStiffness(float _stiffness) const
	{
		joint->SetStiffness(_stiffness);
	}

	float SpringJoint::GetStiffness() const
	{
		return joint->GetStiffness();
	}

	void SpringJoint::SetDamping(float _damping) const
	{
		joint->SetDamping(_damping);
	}

	float SpringJoint::GetDamping() const
	{
		return joint->GetDamping();
	}

	void SpringJoint::SetSpringArg(float _stiffness, float _damping) const
	{
		joint->SetSpringArg(_stiffness, _damping);
	}

	void SpringJoint::SetMinDistanceEnable(bool _value) const
	{
		joint->SetMinDistanceEnable(_value);
	}

	void SpringJoint::SetMaxDistanceEnable(bool _value) const
	{
		joint->SetMaxDistanceEnable(_value);
	}

	void SpringJoint::SetSpringEnable(bool _value) const
	{
		joint->SetSpringEnable(_value);
	}

	void SpringJoint::PreSerialize(json& jsonData) const
	{

	}

	void SpringJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);

		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);

		PREDESERIALIZE_VALUE(useSpring);
		PREDESERIALIZE_VALUE(spring);
		PREDESERIALIZE_VALUE(damper);

		PREDESERIALIZE_VALUE(useMinDistance);
		PREDESERIALIZE_VALUE(minDistance);
		PREDESERIALIZE_VALUE(useMaxDistance);
		PREDESERIALIZE_VALUE(maxDistance);
		PREDESERIALIZE_VALUE(Tolerance);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void SpringJoint::PostSerialize(json& jsonData) const
	{

	}

	void SpringJoint::PostDeserialize(const json& jsonData)
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