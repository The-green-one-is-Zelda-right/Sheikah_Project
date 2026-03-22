#pragma once
#include "ZnJoint.h"

namespace ZonaiPhysics
{
	class ZnSpringFlexJoint : public ZnJoint
	{
	public:
		ZnSpringFlexJoint() = default;
		~ZnSpringFlexJoint() override = default;

	public:
		virtual void SetSpringStiffness(float stiffness) = 0;
		virtual float GetSpringStiffness() const = 0;

		virtual void SetSpringDamping(float damping) = 0;
		virtual float GetSpringDamping() const = 0;

		virtual void SetLinearLimit(float limit) = 0;
		virtual float GetLinearLimit() const = 0;
	};
}