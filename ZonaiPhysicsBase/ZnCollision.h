#pragma once
#include <cstdint>
#include <Eigen/Dense>
#include <memory>
#include <algorithm>

namespace ZonaiPhysics
{
	struct ZnContact
	{
		Eigen::Vector3f point{ Eigen::Vector3f::Zero() };  // 접촉점
		Eigen::Vector3f normal{ Eigen::Vector3f::Zero() }; // 접촉점의 노말
		Eigen::Vector3f impulse{ Eigen::Vector3f::Zero() }; // 접촉점에서의 충격량
		float separation{ 0.f }; // 침투 깊이
	};

	struct ZnCollision
	{
		Eigen::Vector3f impulses{ Eigen::Vector3f::Zero() };
		Eigen::Vector3f thisPostLinearVelocity{ Eigen::Vector3f::Zero() };
		Eigen::Vector3f thisPostAngularVelocity{ Eigen::Vector3f::Zero() };
		Eigen::Vector3f otherPostLinearVelocity{ Eigen::Vector3f::Zero() };
		Eigen::Vector3f otherPostAngularVelocity{ Eigen::Vector3f::Zero() };
		std::unique_ptr<ZnContact[]> contacts; // 접촉점 배열 포인터
		uint32_t contactCount{ 0 }; // 접촉점 개수

		ZnCollision() = default;
		~ZnCollision() = default;
		ZnCollision(const ZnCollision& _col)
		{
			copyFrom(_col);
		}
		ZnCollision(ZnCollision&& _col) noexcept
		{
			moveFrom(std::move(_col));
		}
		ZnCollision& operator=(const ZnCollision& _col)
		{
			if (this != &_col)
			{
				copyFrom(_col);
			}
			return *this;
		}
		ZnCollision& operator=(ZnCollision&& _col) noexcept
		{
			if (this != &_col)
			{
				moveFrom(std::move(_col));
			}
			return *this;
		}

		ZnCollision Swap() const
		{
			ZnCollision swappedCollision;
			swappedCollision.impulses = -impulses;
			swappedCollision.thisPostLinearVelocity = otherPostLinearVelocity;
			swappedCollision.thisPostAngularVelocity = otherPostAngularVelocity;
			swappedCollision.otherPostLinearVelocity = thisPostLinearVelocity;
			swappedCollision.otherPostAngularVelocity = thisPostAngularVelocity;
			swappedCollision.contactCount = contactCount;

			if (contactCount)
			{
				swappedCollision.contacts = std::make_unique<ZnContact[]>(contactCount);
				for (size_t i = 0; i < contactCount; i++)
				{
					swappedCollision.contacts[i].normal = -contacts[i].normal;
					swappedCollision.contacts[i].point = contacts[i].point;
					swappedCollision.contacts[i].impulse = contacts[i].impulse;
					swappedCollision.contacts[i].separation = -contacts[i].separation;
				}
			}

			return swappedCollision;
		}

		void Release()
		{
			contacts.reset();
			contactCount = 0;
		}

	private:
		void copyFrom(const ZnCollision& _col)
		{
			impulses = _col.impulses;
			thisPostLinearVelocity = _col.thisPostLinearVelocity;
			thisPostAngularVelocity = _col.thisPostAngularVelocity;
			otherPostLinearVelocity = _col.otherPostLinearVelocity;
			otherPostAngularVelocity = _col.otherPostAngularVelocity;
			contactCount = _col.contactCount;
			if (contactCount)
			{
				contacts = std::make_unique<ZnContact[]>(contactCount);
				std::copy(_col.contacts.get(), _col.contacts.get() + contactCount, contacts.get());
			}
		}
		void moveFrom(ZnCollision&& _col)
		{
			impulses = std::move(_col.impulses);
			thisPostLinearVelocity = std::move(_col.thisPostLinearVelocity);
			thisPostAngularVelocity = std::move(_col.thisPostAngularVelocity);
			otherPostLinearVelocity = std::move(_col.otherPostLinearVelocity);
			otherPostAngularVelocity = std::move(_col.otherPostAngularVelocity);
			contacts = std::move(_col.contacts);
			contactCount = _col.contactCount;
			_col.contactCount = 0;
		}
	};
}
