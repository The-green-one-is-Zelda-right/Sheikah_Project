#pragma once
#include "ZonaiMath.h"

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}

namespace ZonaiPhysics
{
	using namespace ZonaiMath;

	class ZnTransform
	{
	public:
		ZnTransform() noexcept = default;

		ZnTransform(const Vector3D& _vec) noexcept : position(_vec), quaternion()
		{}

		ZnTransform(const Quaternion& _qat) noexcept : position(), quaternion(_qat)
		{}

		ZnTransform(const Vector3D& _vec, const Quaternion& _qat) noexcept : position(_vec), quaternion(_qat)
		{}

		ZnTransform(const ZnTransform& _tran) noexcept : position(_tran.position), quaternion(_tran.quaternion)
		{}

		void operator=(const ZnTransform& _tran) noexcept
		{
			position = _tran.position;
			quaternion = _tran.quaternion;
		}

	public:
		Vector3D position;
		Quaternion quaternion;
	};
}