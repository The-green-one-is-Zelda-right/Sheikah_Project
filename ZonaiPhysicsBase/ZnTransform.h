#pragma once
#include <algorithm>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ZnTransform
	{
	public:
		ZnTransform() noexcept :
			quaternion(Eigen::Quaternionf::Identity()), position(Eigen::Vector3f::Zero())
		{}

		ZnTransform(const Eigen::Vector3f& _vec) noexcept :
			quaternion(Eigen::Quaternionf::Identity()), position(_vec)
		{}

		ZnTransform(Eigen::Vector3f&& _vec) noexcept :
			quaternion(Eigen::Quaternionf::Identity()), position(std::move(_vec))
		{}

		ZnTransform(const Eigen::Quaternionf& _qat) noexcept :
			quaternion(_qat), position(Eigen::Vector3f::Zero())
		{}

		ZnTransform(Eigen::Quaternionf&& _qat) noexcept :
			quaternion(std::move(_qat)), position(Eigen::Vector3f::Zero())
		{}

		ZnTransform(const Eigen::Vector3f& _vec, const Eigen::Quaternionf& _qat) noexcept :
			quaternion(_qat), position(_vec)
		{}

		ZnTransform(Eigen::Vector3f&& _vec, Eigen::Quaternionf&& _qat) noexcept :
			quaternion(std::move(_qat)), position(std::move(_vec))
		{}

		ZnTransform(const ZnTransform& _tran) noexcept :
			quaternion(_tran.quaternion), position(_tran.position)
		{}

		ZnTransform(ZnTransform&& _trans) noexcept :
			quaternion(std::move(_trans.quaternion)), position(std::move(_trans.position))
		{}

		constexpr ZnTransform& operator=(const ZnTransform& _tran) noexcept = default;

		constexpr ZnTransform& operator=(ZnTransform && _tran) noexcept = default;

	public:
		Eigen::Quaternionf quaternion;
		Eigen::Vector3f position;
	};
}