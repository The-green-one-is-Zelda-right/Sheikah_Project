#pragma once
#include <cstdint>
#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#pragma warning (pop)

#include "ZnResourceID.h"

namespace ZonaiPhysics
{
	struct ZnBound3;
	struct ZnTransform;

	class ColliderHelper
	{
	public:
		static void					Release(void* _shape);

		static void					DisableQuery(void* _shape, bool _value);
		static void					DisableSimulation(void* _shape, bool _value);
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

		static ZnBound3				GetBoundingBox(void* _shape, const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot);
	
		static void					SetMaterial(void* _shape, const ZnMaterialID& _id);
		static void					SetMaterials(void* _shape, ZnMaterialID* _pointer, int size);
	};
}
