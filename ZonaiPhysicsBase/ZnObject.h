#pragma once

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}

namespace ZonaiPhysics
{
	using namespace ZonaiMath;

	class ZnObject
	{
	public:
							ZnObject() noexcept = default;
		virtual				~ZnObject() noexcept = 0;

	public:
		/**
		위치
		*/
		virtual Vector3D	GetPosition() const noexcept = 0;
		virtual void		SetPosition(const Vector3D& _position) noexcept = 0;

		/**
		회전
		*/
		virtual Quaternion	GetQuaternion() const noexcept = 0;
		virtual void		SetQuaternion(const Quaternion& _quaternion) noexcept = 0;

		/**
		유저 데이터
		*/
		virtual void*		GetUserData() const noexcept = 0;
		virtual void		SetUserData(void* _userData) noexcept = 0;
	};
}

/*
오브젝트가 PxRigidDynamic을 들고 있고
RigidBody, Collider가 그걸 받아서 쓰는 형태로 하는데.
*/