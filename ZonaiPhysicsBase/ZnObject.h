#pragma once
#include <Eigen/Dense>

#include "ZnBase.h"


namespace ZonaiPhysics
{
	class ZnObject : public ZnBase
	{
	public:
							ZnObject() : userData()
		{}
		virtual				~ZnObject() = default;

	public:
		/**
		��ġ
		*/
		virtual Eigen::Vector3f	GetPosition() const = 0;
		virtual void		SetPosition(const Eigen::Vector3f& _position) = 0;

		/**
		ȸ��
		*/
		virtual Eigen::Quaternionf	GetQuaternion() const = 0;
		virtual void		 SetQuaternion(const Eigen::Quaternionf& _quaternion) = 0;

		/**
		���� ������
		*/
		virtual void*		GetUserData() const = 0;
		virtual void		SetUserData(void* _userData) = 0;

	protected:
		void* userData;
	};
}
