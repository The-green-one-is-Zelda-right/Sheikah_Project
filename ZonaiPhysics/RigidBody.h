#pragma once
#include "ZnRigidBody.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class Collider;

	struct MassData
	{
		float mass;
		float invMass;
		Eigen::Matrix3f inerita;
		Eigen::Matrix3f invInerita;
		Eigen::Vector3f centerOfMass;
	};

	struct Material
	{
		float linearDamping;
		float angularDamping;
		float resoultion;
	};

	class RigidBody : public ZnRigidBody
	{
	public:
		Eigen::Vector3f GetPosition() const noexcept override;
		void SetPosition(const Eigen::Vector3f& _position) noexcept override;

		Eigen::Quaternionf GetQuaternion() const noexcept override;
		void SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept override;

		void* GetUserData() const noexcept override;
		void SetUserData(void* _userData) noexcept override;

		void WakeUp() noexcept override;
		bool IsSleeping() const noexcept override;

		DynamicLocks GetDynamicLockFlags() const noexcept override;
		void SetDynamicLockFlag(DynamicLock flag, bool) noexcept override;
		void SetDynamicLockFlags(DynamicLocks flags) noexcept override;

		float GetMass() const noexcept override;
		void SetMass(float) noexcept override;
		float GetInvMass() const noexcept override;
		Eigen::Vector3f GetInertiaTensor() const noexcept override;
		void SetInertiaTensor(const Eigen::Vector3f&) noexcept override;

		float GetLinearDamping() const noexcept override;
		void SetLinearDamping(float _damping) noexcept override;
		float GetAngularDamping() const noexcept override;
		void SetAngularDamping(float _damping) noexcept override;

		Eigen::Vector3f GetLinearVelocity() const noexcept override;
		void SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept override;
		Eigen::Vector3f GetAngularVelocity() const noexcept override;
		void SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept override;

		float GetMaxLinearVelocity() const noexcept override;
		void SetMaxLinearVelocity(const float& _limit) noexcept override;
		float GetMaxAngularVelocity() const noexcept override;
		void SetMaxAngularVelocity(const float& _limit) noexcept override;

		void SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque,
			ForceType _type) noexcept override;
		void AddForce(const Eigen::Vector3f& _force, ForceType _type) noexcept override;
		void ClearForce() noexcept override;
		void AddTorque(const Eigen::Vector3f& _torque, ForceType _type) noexcept override;
		void ClearTorque() noexcept override;



		std::vector<Collider*> colliders;

		MassData massData;
		Material material;

		Eigen::Vector3f position;
		Eigen::Quaternionf rotation;

		float limitLinearVelocity;
		float limitAngularVelocity;

		Eigen::Vector3f linearVelocity;
		Eigen::Vector3f angularVelocity;

		Eigen::Vector3f linearAcceleration;
		Eigen::Vector3f angularAcceleration;

		void* userData;

		bool active;
		DynamicLocks lockFlags;
	};
}