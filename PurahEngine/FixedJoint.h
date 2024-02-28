#pragma once
#include "Joint.h"

namespace ZonaiPhysics
{
	class ZnFixedJoint;
}

namespace PurahEngine
{
	class FixedJoint : public Joint
	{
	public:
		FixedJoint();
		~FixedJoint() override;

	public:
		void SetLocalPosition(int _index, const Eigen::Vector3f&) noexcept override;
		Eigen::Vector3f GetLocalPosition(int _index) const noexcept override;
		void SetLocalQuaternion(int _index, const Eigen::Quaternionf&) noexcept override;
		Eigen::Quaternionf GetLocalQuaternion(int _index) const noexcept override;
		Eigen::Vector3f GetRelativeLinearVelocity() const noexcept override;
		Eigen::Vector3f GetRelativeAngularVelocity() const noexcept override;
		void SetBreakForce(float _force, float _torque) noexcept override;
		void GetBreakForce(float& _force, float& _torque) const noexcept override;

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		ZonaiPhysics::ZnFixedJoint* joint;
	};
}

