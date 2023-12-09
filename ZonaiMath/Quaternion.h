#pragma once
#include <smmintrin.h>

namespace ZonaiMath
{
	class Vector4D;

	class Quaternion
	{
	public:
		/// <summary>
		/// 특수 멤버 함수
		/// </summary>
		constexpr Quaternion() noexcept :
			m{}
		{}
		constexpr Quaternion(float _w, float _x, float _y, float _z) noexcept :
			x(_x), y(_y), z(_z), w(_w)
		{}
		constexpr Quaternion(const Quaternion&) noexcept = default;
		constexpr Quaternion(Quaternion&&) noexcept = default;
		~Quaternion() noexcept = default;

		constexpr Quaternion& operator= (const Quaternion&) noexcept = default;
		constexpr Quaternion& operator= (Quaternion&&) noexcept = default;

		explicit operator Vector4D() noexcept;

		/// <summary>
		/// 멤버 변수
		/// </summary>
		union
		{
			struct
			{
				float w, x, y, z;
			};
			float e[4];
			__m128 m;
		};

		static const Quaternion Zero;
		static const Quaternion Identity;

		/// <summary>
		/// 쿼터니언 함수
		/// </summary>
		float Length() const noexcept;
		float LengthSquare() const noexcept;
		float FastInvSqrt(float) const noexcept;
		float Dot(const Quaternion&) const noexcept;

		Quaternion Conjugate() const noexcept;
		Quaternion Inverse() const noexcept;
		Quaternion& Normalize() noexcept;
		Quaternion Normalized() const noexcept;

		/// <summary>
		/// 연산자 오버로딩
		/// </summary>
		Quaternion& operator += (const Quaternion&) noexcept;
		Quaternion& operator -= (const Quaternion&) noexcept;

		Quaternion operator + (const Quaternion&) const noexcept;
		Quaternion operator - (const Quaternion&) const noexcept;
		Quaternion operator - () const noexcept;

		Quaternion& operator *= (const Quaternion&) noexcept;
		Quaternion& operator /= (const Quaternion&) noexcept;

		Quaternion operator * (const Quaternion&) const noexcept;
		Quaternion operator / (const Quaternion&) const noexcept;

		Quaternion& operator *= (const float) noexcept;
		Quaternion& operator /= (const float) noexcept;

		Quaternion operator * (const float) const noexcept;
		Quaternion operator / (const float) const noexcept;

		friend Quaternion operator *= (const float, const Quaternion&) noexcept;
		friend Quaternion operator /= (const float, const Quaternion&) noexcept;

		friend Quaternion operator * (const float, const Quaternion&) noexcept;
		friend Quaternion operator / (const float, const Quaternion&) noexcept;

		bool operator == (const Quaternion&) const noexcept;
	};

	Quaternion operator *= (const float, const Quaternion&) noexcept;
	Quaternion operator /= (const float, const Quaternion&) noexcept;
	Quaternion operator * (const float, const Quaternion&) noexcept;
	Quaternion operator / (const float, const Quaternion&) noexcept;
}
