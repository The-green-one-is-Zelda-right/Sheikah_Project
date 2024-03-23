#include "SlideJoint.h"


namespace PurahEngine
{
	SlideJoint::~SlideJoint()
	{
	}

	void SlideJoint::Initialize()
	{
		JointT<ZonaiPhysics::ZnPrismaticJoint>::Initialize();
	}

	void SlideJoint::OnDataLoadComplete()
	{
		JointT<ZonaiPhysics::ZnPrismaticJoint>::OnDataLoadComplete();
	}

	void SlideJoint::PreSerialize(json& jsonData) const
	{

	}

	void SlideJoint::PreDeserialize(const json& jsonData)
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

	void SlideJoint::PostSerialize(json& jsonData) const
	{

	}

	void SlideJoint::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(connectedBody);
	}
}