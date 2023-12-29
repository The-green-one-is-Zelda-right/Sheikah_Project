#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API BoxCollider : public Collider
	{
	public:
		BoxCollider();
		~BoxCollider() override;

	public:
		void Awake() override;

	public:
		void SetSize(const Eigen::Vector3f& _size);

		void SetOffsetPosition(const Eigen::Vector3f& _position);
		Eigen::Vector3f GetOffsetPosition();
		void SetOffsetQuaternion(const Eigen::Quaternionf& _quaternion);
		Eigen::Quaternionf GetOffsetQuaternion();

	public:
		bool awake;
		Eigen::Vector3f size;
		Eigen::Vector3f positionOffset;
		Eigen::Quaternionf rotationOffset;
	};
}

