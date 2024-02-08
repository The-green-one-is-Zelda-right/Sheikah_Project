#pragma once
#include <cstdint>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ColliderHelper
	{
	public:
		static void					Release(void* _shape);

		static void					SetTrigger(void* _shape, bool _value);
		static void					SetLayer(void* _shape, uint32_t _layer);

		static Eigen::Vector3f		GetLocalPosition(void* _shape);
		static void					SetLocalPosition(void* _shape, const Eigen::Vector3f& _pos);

		static Eigen::Quaternionf	GetLocalQuaternion(void* _shape);
		static void					SetLocalQuaternion(void* _shape, const Eigen::Quaternionf& _quat);

		static Eigen::Vector3f		GetGlobalPosition(void* _shape);
		static void					SetGlobalPosition(void* _shape, const Eigen::Vector3f& _pos);

		static Eigen::Quaternionf	GetGlobalQuaternion(void* _shape);
		static void					SetGlobalQuaternion(void* _shape, const Eigen::Quaternionf& _quat);
	};
}
