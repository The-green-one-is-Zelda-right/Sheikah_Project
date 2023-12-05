#pragma once
#include "ZnObject.h"

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}


namespace ZonaiPhysics
{
	class ZnCollider : public ZnObject
	{
	public:
							ZnCollider() noexcept = default;
		virtual				~ZnCollider() noexcept = default;

	public:
		/**
		��ġ
		*/
		virtual ZonaiMath::Vector3D	GetPosition() const noexcept = 0;
		virtual void		SetPosition(const ZonaiMath::Vector3D& _position) noexcept = 0;

		/**
		ȸ��
		*/
		virtual ZonaiMath::Quaternion	GetQuaternion() const noexcept = 0;
		virtual void		SetQuaternion(const ZonaiMath::Quaternion& _quaternion) noexcept = 0;

		/**
		���� ������
		*/
		virtual void*		GetUserData() const noexcept = 0;
		virtual void		SetUserData(void* _userData) noexcept = 0;

		virtual void		SetTrigger(bool) noexcept = 0;
	};
}
