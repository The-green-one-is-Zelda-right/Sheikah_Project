#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "FixedJoint.h"

namespace PurahEngine
{
	FixedJoint::~FixedJoint()
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

	void FixedJoint::PostInitialize()
	{
		using namespace ZonaiPhysics;

		const auto& instance = PhysicsSystem::GetInstance();

		assert(body != nullptr);

		ZonaiPhysics::ZnRigidBody* connect = nullptr;

		if (connectedBody)
		{
			connect = connectedBody->body;
		}

		joint = instance.CreateFixedJoint(
			body->body, ZnTransform(LocalAnchor, LocalAnchorRotation),
			connect, ZnTransform(connectedLocalAnchor, connectedLocalAnchorRotation)
		);

		PhysicsSystem::GetInstance().joints.push_back(this);
		joint->SetUserData(this);

		JointT::PostInitialize();
	}

	void FixedJoint::PreSerialize(json& jsonData) const
	{

	}

	void FixedJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);
		PREDESERIALIZE_QUATERNIONF(LocalAnchorRotation);
		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);
		PREDESERIALIZE_QUATERNIONF(connectedLocalAnchorRotation);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void FixedJoint::PostSerialize(json& jsonData) const
	{

	}

	void FixedJoint::PostDeserialize(const json& jsonData)
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
