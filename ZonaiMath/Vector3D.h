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
		constexpr Vector3D() : e{}
		{}
		constexpr Vector3D(float _a) :
			e{ _a, _a, _a }
		{}
		constexpr Vector3D(float _x, float _y, float _z)
			: e{ _x, _y, _z }
		{}
		constexpr Vector3D(const Vector3D&) = default;
		constexpr Vector3D(Vector3D&&) = default;
		constexpr Vector3D(_In_reads_(3) const float* _arr) :
			e{ _arr[0], _arr[1], _arr[2] }
		{}
		~Vector3D() = default;

		constexpr Vector3D& operator= (const Vector3D&) = default;
		constexpr Vector3D& operator= (Vector3D&&) = default;

		explicit operator Matrix1x3();

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
		float Length() const;
		float LengthSquare() const;
		Vector3D Cross(const Vector3D&) const;
		float Dot(const Vector3D&) const;
		float ScalarTriple(const Vector3D&, const Vector3D&) const;
		Vector3D VectorTriple(const Vector3D&, const Vector3D&) const;
		float FastInvSqrt(float) const;
		Vector3D& Normalize();				// 벡터를 정규화하고 반환
		Vector3D Normalized() const;		// 정규화된 새로운 벡터를 반환

		/// <summary>
		/// 연산자 오버로딩
		/// </summary>
		Vector3D& operator += (const Vector3D&);
		Vector3D& operator -= (const Vector3D&);

		Vector3D operator + (const Vector3D&) const;
		Vector3D operator - (const Vector3D&) const;
		Vector3D operator - () const;

		Vector3D& operator *= (const float);
		Vector3D& operator /= (const float);

		Vector3D operator * (const float) const;
		Vector3D operator / (const float) const;

		Vector3D& operator*= (const Matrix3x3&);
		Vector3D operator* (const Matrix3x3&) const;

		Vector3D& operator*= (const Matrix4x4&);
		Vector3D operator* (const Matrix4x4&) const;

		bool operator == (const Vector3D&) const;

		friend Vector3D operator * (const Quaternion&, const Vector3D&);
	};

	Vector3D operator * (const Quaternion&, const Vector3D&);
}
