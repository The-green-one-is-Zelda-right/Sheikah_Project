#pragma once
#include "ZnDistanceJoint.h"
#include "TemplateJoint.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

namespace physx
{
	class PxPhysics;
	class PxDistanceJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class DistanceJoint : public TemplateJoint<ZnDistanceJoint, physx::PxDistanceJoint>
	{
	public:
		DistanceJoint() = delete;
		DistanceJoint(physx::PxDistanceJoint*, RigidBody*, RigidBody*);
		//DistanceJoint(
		//	physx::PxPhysics*& _factory,
		//	RigidBody* _object0, const ZnTransform& _transform0,
		//	RigidBody* _object1, const ZnTransform& _transform1);
		~DistanceJoint() override;

	public:
		float GetDistance() const override;

		void SetMinDistance(float _distance) override;
		float GetMinDistance() const override;

		void SetMaxDistance(float _distance) override;
		float GetMaxDistance() const override;

		void SetTolerance(float _tolerance) override;
		float GetTolerance() const override;

		void SetStiffness(float _stiffness) override;
		float GetStiffness() const override;

		void SetDamping(float _damping) override;
		float GetDamping() const override;

		void SetSpringArg(float _stiffness, float _damping) override;

		void SetMinDistanceEnable(bool _value) override;
		void SetMaxDistanceEnable(bool _value) override;
		void SetSpringEnable(bool _value) override;
	};
}
