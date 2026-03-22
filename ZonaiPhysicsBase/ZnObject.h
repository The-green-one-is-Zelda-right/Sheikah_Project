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
		위치
		*/
		virtual Eigen::Vector3f	GetPosition() const = 0;
		virtual void		SetPosition(const Eigen::Vector3f& _position, bool _wakeup = false) = 0;

		/**
		회전
		*/
		virtual Eigen::Quaternionf	GetQuaternion() const = 0;
		virtual void		 SetQuaternion(const Eigen::Quaternionf& _quaternion, bool _wakeup = false) = 0;

		/**
		유저 데이터
		*/
		virtual void*		GetUserData() const = 0;
		virtual void		SetUserData(void* _userData) = 0;

	protected:
		void* userData;
	};
}
