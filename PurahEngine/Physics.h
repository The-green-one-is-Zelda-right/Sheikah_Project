#pragma once
#include "PurahEngineAPI.h"
#include <Eigen/Dense>

#include "MaterialEnum.h"
#include "ZnQueryInfo.h"

namespace ZonaiPhysics
{
	struct ZnQueryInfo;
}

namespace PurahEngine
{
	class PURAHENGINE_API Physics
	{
	public:
		static bool Raycast(const Eigen::Vector3f& _from,
			const Eigen::Vector3f& _to, float _distace,
			const std::vector<int>& _layers,
			ZonaiPhysics::ZnQueryInfo& _info);

		static bool Raycast(const Eigen::Vector3f& _from,
			const Eigen::Vector3f& _to, float _distace,
			unsigned int _layers,
			ZonaiPhysics::ZnQueryInfo& _info);

		static bool Boxcast(const Eigen::Vector3f& _extend, 
			const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
			const Eigen::Vector3f& _dir, float _distance,
			const std::vector<int>& _layers,
			ZonaiPhysics::ZnQueryInfo& _info
			);

		static bool Boxcast(const Eigen::Vector3f& _extend,
			const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
			const Eigen::Vector3f& _dir, float _distance,
			unsigned int _layers,
			ZonaiPhysics::ZnQueryInfo& _info
		);

		static bool Spherecast(float radius,
			const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
			const Eigen::Vector3f& _dir, float _distance,
			const std::vector<int>& _layers,
			ZonaiPhysics::ZnQueryInfo& _info
		);

		static bool Spherecast(float radius,
			const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
			const Eigen::Vector3f& _dir, float _distance,
			unsigned int _layers,
			ZonaiPhysics::ZnQueryInfo& _info
		);

		static bool Capsulecast(float height, float radius,
			const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
			const Eigen::Vector3f& _dir, float _distance,
			const std::vector<int>& _layers,
			ZonaiPhysics::ZnQueryInfo& _info
		);

		static bool Capsulecast(float height, float radius,
			const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
			const Eigen::Vector3f& _dir, float _distance,
			unsigned int _layers,
			ZonaiPhysics::ZnQueryInfo& _info
		);

		static bool SphereOverlap(float _radius,
			const Eigen::Vector3f& _pos, const Eigen::Quaternionf _rot,
			unsigned int _layers,
			ZonaiPhysics::ZnQueryInfo& _info
		);

		static int GetLayerID(const std::wstring& _name);

		static Eigen::Vector3f GetGravity();

		static void SetGravity(const Eigen::Vector3f& _gravity);
	};
}
