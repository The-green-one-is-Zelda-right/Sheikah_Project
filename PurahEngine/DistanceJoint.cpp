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

	}

	void DistanceJoint::OnDataLoadComplete()
	{
		auto& instance = PhysicsSystem::GetInstance();

		const RigidBody* body0 = GetGameObject()->GetComponent<RigidBody>();

		assert(body0 != nullptr);

		joint = instance.CreateDistanceJoint(
            body0->body, { LocalAnchor }, 
            connectedBody->body, { connectedLocalAnchor }
        );

        SetMinDistanceEnable(true);
        SetMinDistance(minDistance);
        SetMaxDistanceEnable(true);
        SetMaxDistance(maxDistance);
        SetTolerance(Tolerance);
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
        POSTDESERIALIZE_PTR(connectedBody);
	}
}
