#pragma once
#include <sal.h>

namespace ZonaiMath
{
	struct Matrix1x3;
	struct Matrix3x3;
	struct Matrix4x4;
	struct Quaternion;

	class Vector3D
	{
	public:
		/// <summary>
		/// 특수 멤버 함수
		/// </summary>
		constexpr Vector3D() noexcept : e{}
		{}
		constexpr Vector3D(float _a) noexcept :
			e{ _a, _a, _a }
		{}
		constexpr Vector3D(float _x, float _y, float _z) noexcept
			: e{ _x, _y, _z }
		{}
		constexpr Vector3D(const Vector3D&) noexcept = default;
		constexpr Vector3D(Vector3D&&) noexcept = default;
		constexpr Vector3D(_In_reads_(3) const float* _arr) noexcept :
			e{ _arr[0], _arr[1], _arr[2] }
		{}
		~Vector3D() noexcept = default;

		constexpr Vector3D& operator= (const Vector3D&) noexcept = default;
		constexpr Vector3D& operator= (Vector3D&&) noexcept = default;

		explicit operator Matrix1x3() noexcept;

		/// <summary>
		/// 멤버 변수
		/// </summary>
		union
		{
			struct
			{
				float x, y, z;
			};
			float e[3];
		};

		static const Vector3D Zero;

		static const Vector3D Right;
		static const Vector3D Up;
		static const Vector3D Forward;

		/// <summary>
		/// 벡터 함수
		/// </summary>
		float Length() const noexcept;
		float LengthSquare() const noexcept;
		Vector3D Cross(const Vector3D&) const noexcept;
		float Dot(const Vector3D&) const noexcept;
		float ScalarTriple(const Vector3D&, const Vector3D&) const noexcept;
		Vector3D VectorTriple(const Vector3D&, const Vector3D&) const noexcept;
		float FastInvSqrt(float) const noexcept;
		Vector3D& Normalize() noexcept;				// 벡터를 정규화하고 반환
		Vector3D Normalized() const noexcept;		// 정규화된 새로운 벡터를 반환

		/// <summary>
		/// 연산자 오버로딩
		/// </summary>
		Vector3D& operator += (const Vector3D&) noexcept;
		Vector3D& operator -= (const Vector3D&) noexcept;

		Vector3D operator + (const Vector3D&) const noexcept;
		Vector3D operator - (const Vector3D&) const noexcept;
		Vector3D operator - () const noexcept;

		Vector3D& operator *= (const float) noexcept;
		Vector3D& operator /= (const float) noexcept;

		Vector3D operator * (const float) const noexcept;
		Vector3D operator / (const float) const noexcept;

		Vector3D& operator*= (const Matrix3x3&) noexcept;
		Vector3D operator* (const Matrix3x3&) const noexcept;

		Vector3D& operator*= (const Matrix4x4&) noexcept;
		Vector3D operator* (const Matrix4x4&) const noexcept;

		bool operator == (const Vector3D&) const noexcept;

		friend Vector3D operator * (const Quaternion&, const Vector3D&) noexcept;
	};

	Vector3D operator * (const Quaternion&, const Vector3D&) noexcept;
}
