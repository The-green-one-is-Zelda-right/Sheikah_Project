#pragma once
#include <algorithm>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ZnTransform
	{
	public:
		ZnTransform() :
			quaternion(Eigen::Quaternionf::Identity()), position(Eigen::Vector3f::Zero())
		{}

		ZnTransform(const Eigen::Vector3f& _vec) :
			quaternion(Eigen::Quaternionf::Identity()), position(_vec)
		{}

		ZnTransform(Eigen::Vector3f&& _vec) :
			quaternion(Eigen::Quaternionf::Identity()), position(std::move(_vec))
		{}

		ZnTransform(const Eigen::Quaternionf& _qat) :
			quaternion(_qat), position(Eigen::Vector3f::Zero())
		{}

		ZnTransform(Eigen::Quaternionf&& _qat) :
			quaternion(std::move(_qat)), position(Eigen::Vector3f::Zero())
		{}

		ZnTransform(const Eigen::Vector3f& _vec, const Eigen::Quaternionf& _qat) :
			quaternion(_qat), position(_vec)
		{}

		ZnTransform(Eigen::Vector3f&& _vec, Eigen::Quaternionf&& _qat) :
			quaternion(std::move(_qat)), position(std::move(_vec))
		{}

		ZnTransform(const ZnTransform& _tran) :
			quaternion(_tran.quaternion), position(_tran.position)
		{}

		ZnTransform(ZnTransform&& _trans) :
			quaternion(std::move(_trans.quaternion)), position(std::move(_trans.position))
		{}

		constexpr ZnTransform& operator=(const ZnTransform& _tran) = default;

		constexpr ZnTransform& operator=(ZnTransform && _tran) = default;

	public:
		Eigen::Quaternionf quaternion;
		Eigen::Vector3f position;
	};
}