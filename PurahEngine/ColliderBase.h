#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ZnCollider;
}

namespace PurahEngine
{
	enum class ColliderType
	{
		DYNAMIC,
		STATIC,
	};

	class PURAHENGINE_API ColliderBase : public Component
	{
	public:
		ColliderBase() {}
		virtual ~ColliderBase() {}

	public:
		virtual void Awake() = 0;

	public:
		virtual void PreStep() = 0;

	protected:
		bool awake = false;
		ZonaiPhysics::ZnCollider* znCollider = nullptr;
		Eigen::Vector3f positionOffset{ Eigen::Vector3f::Zero() };
		Eigen::Quaternionf rotationOffset{ Eigen::Quaternionf::Identity() };

	public:
		void SetOffsetPosition(const Eigen::Vector3f& _position);
		Eigen::Vector3f GetOffsetPosition() const;

		void SetOffsetQuaternion(const Eigen::Quaternionf& _quaternion);
		Eigen::Quaternionf GetOffsetQuaternion() const;
	};
}
