#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "DistanceJoint.h"

namespace PurahEngine
{
	DistanceJoint::~DistanceJoint()
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

	void DistanceJoint::PostInitialize()
	{
		using namespace ZonaiPhysics;

		const auto& instance = PhysicsSystem::GetInstance();

		assert(body != nullptr);

        ZonaiPhysics::ZnRigidBody* connect = nullptr;

        if (connectedBody)
        {
            connect = connectedBody->body;
        }

        joint = instance.CreateDistanceJoint(
			body->body, ZnTransform(LocalAnchor, LocalAnchorRotation),
			connect, ZnTransform(connectedLocalAnchor, connectedLocalAnchorRotation)
        );

        PhysicsSystem::GetInstance().joints.push_back(this);
        joint->SetUserData(this);

        joint->SetSpringEnable(false);

        SetMinDistanceEnable(true);
        SetMinDistance(minDistance);
        SetMaxDistanceEnable(true);
        SetMaxDistance(maxDistance);
        SetTolerance(Tolerance);

        JointT::PostInitialize();
	}

    float DistanceJoint::GetDistance() const
    {
        return joint->GetDistance();
    }

    void DistanceJoint::SetMinDistance(float _distance) const
    {
        joint->SetMinDistance(_distance);
    }

    float DistanceJoint::GetMinDistance() const
    {
        return joint->GetMinDistance();
    }

    void DistanceJoint::SetMaxDistance(float _distance) const
    {
        joint->SetMaxDistance(_distance);
    }

    float DistanceJoint::GetMaxDistance() const
    {
        return joint->GetMaxDistance();
    }

    void DistanceJoint::SetTolerance(float _tolerance) const
    {
        joint->SetTolerance(_tolerance);
    }

    float DistanceJoint::GetTolerance() const
    {
        return joint->GetTolerance();
    }

    void DistanceJoint::SetMinDistanceEnable(bool _value) const
    {
        joint->SetMinDistanceEnable(_value);
    }

    void DistanceJoint::SetMaxDistanceEnable(bool _value) const
    {
        joint->SetMaxDistanceEnable(_value);
    }

	void DistanceJoint::PreSerialize(json& jsonData) const
	{

	}

	void DistanceJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);

		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);

		PREDESERIALIZE_VALUE(useMinDistance);
		PREDESERIALIZE_VALUE(minDistance);
		PREDESERIALIZE_VALUE(maxDistance);
		PREDESERIALIZE_VALUE(Tolerance);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void DistanceJoint::PostSerialize(json& jsonData) const
	{

	}

	void DistanceJoint::PostDeserialize(const json& jsonData)
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
