#include "SpringJoint.h"


namespace PurahEngine
{
	SpringJoint::~SpringJoint()
	{
	}

	void SpringJoint::Initialize()
	{
		JointT<ZonaiPhysics::ZnDistanceJoint>::Initialize();
	}

	void SpringJoint::OnDataLoadComplete()
	{
		JointT<ZonaiPhysics::ZnDistanceJoint>::OnDataLoadComplete();
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
		POSTDESERIALIZE_PTR(connectedBody);
	}
}