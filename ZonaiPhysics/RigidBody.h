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
		Eigen::Vector3f GetPosition() const override;
		void SetPosition(const Eigen::Vector3f& _position) override;

		Eigen::Quaternionf GetQuaternion() const override;
		void SetQuaternion(const Eigen::Quaternionf& _quaternion) override;

		void* GetUserData() const override;
		void SetUserData(void* _userData) override;

		void WakeUp() override;
		bool IsSleeping() const override;

		DynamicLocks GetDynamicLockFlags() const override;
		void SetDynamicLockFlag(DynamicLock flag, bool) override;
		void SetDynamicLockFlags(DynamicLocks flags) override;

		float GetMass() const override;
		void SetMass(float) override;
		float GetInvMass() const override;
		Eigen::Vector3f GetInertiaTensor() const override;
		void SetInertiaTensor(const Eigen::Vector3f&) override;

		float GetLinearDamping() const override;
		void SetLinearDamping(float _damping) override;
		float GetAngularDamping() const override;
		void SetAngularDamping(float _damping) override;

		Eigen::Vector3f GetLinearVelocity() const override;
		void SetLinearVelocity(const Eigen::Vector3f& _velocity) override;
		Eigen::Vector3f GetAngularVelocity() const override;
		void SetAngularVelocity(const Eigen::Vector3f& _velocity) override;

		float GetMaxLinearVelocity() const override;
		void SetMaxLinearVelocity(const float& _limit) override;
		float GetMaxAngularVelocity() const override;
		void SetMaxAngularVelocity(const float& _limit) override;

		void SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque,
			ForceType _type) override;
		void AddForce(const Eigen::Vector3f& _force, ForceType _type) override;
		void ClearForce() override;
		void AddTorque(const Eigen::Vector3f& _torque, ForceType _type) override;
		void ClearTorque() override;



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