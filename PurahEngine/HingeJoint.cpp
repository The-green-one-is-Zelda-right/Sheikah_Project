#include "HingeJoint.h"

namespace PurahEngine
{
	HingeJoint::~HingeJoint()
	{
	}

	void HingeJoint::Initialize()
	{
		JointT<ZonaiPhysics::ZnHingeJoint>::Initialize();
	}

	void HingeJoint::OnDataLoadComplete()
	{
		JointT<ZonaiPhysics::ZnHingeJoint>::OnDataLoadComplete();
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
		POSTDESERIALIZE_PTR(connectedBody);
	}
}