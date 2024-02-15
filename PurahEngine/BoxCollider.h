#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API BoxCollider final : public Collider
	{
	public:
		BoxCollider() = default;
		~BoxCollider() override = default;

		void Awake() override
		{
			Collider::Awake();

			this->znCollider = PhysicsSystem::GetInstance().CreateBoxCollider(
				this->GetGameObject(),
				size.x(), size.y(), size.z()
			);

			this->awake = true;

			this->znCollider->SetLocalPosition(this->positionOffset);
			this->znCollider->SetLocalQuaternion(this->rotationOffset);
		}

	public:
		void SetSize(const Eigen::Vector3f& _size)
		{
			this->size = _size;
		}

	public:
		Eigen::Vector3f size{ 1.f, 1.f, 1.f };
	};
}
