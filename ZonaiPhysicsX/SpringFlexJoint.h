#pragma once
#include "D1SpringFlexJoint.h"
#include "ZnSpringFlexJoint.h"
#include "TemplateJoint.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

namespace ZonaiPhysics
{
	class SpringFlexJoint : public TemplateJoint<ZnSpringFlexJoint, D1SpringFlexJoint>
	{
	public:
		SpringFlexJoint() = delete;
		SpringFlexJoint(D1SpringFlexJoint*, RigidBody*, RigidBody*);
		~SpringFlexJoint();

		void SetSpringStiffness(float stiffness) override;
		float GetSpringStiffness() const override;

		void SetSpringDamping(float damping) override;
		float GetSpringDamping() const override;

		void SetLinearLimit(float limit) override;
		float GetLinearLimit() const override;
	};
}

