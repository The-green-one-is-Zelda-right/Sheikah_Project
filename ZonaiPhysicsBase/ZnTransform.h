#pragma once
#include "../ZonaiMath/ZonaiMath.h"

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}

namespace ZonaiPhysics
{
	class ZnTransform
	{
	public:
		ZnTransform() noexcept = default;

		ZnTransform(const ZonaiMath::Vector3D& _vec) noexcept : position(_vec), quaternion()
		{}

		ZnTransform(const ZonaiMath::Quaternion& _qat) noexcept : position(), quaternion(_qat)
		{}

		ZnTransform(const ZonaiMath::Vector3D& _vec, const ZonaiMath::Quaternion& _qat) noexcept : position(_vec), quaternion(_qat)
		{}

		ZnTransform(const ZnTransform& _tran) noexcept : position(_tran.position), quaternion(_tran.quaternion)
		{}

		void operator=(const ZnTransform& _tran) noexcept
		{
			position = _tran.position;
			quaternion = _tran.quaternion;
		}

	public:
		ZonaiMath::Vector3D position;
		ZonaiMath::Quaternion quaternion;
	};
}