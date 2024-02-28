#include <cmath>
#include <utility>

#include "Vector3D.h"
#include "Matrix1x4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

#include "Vector4D.h"

namespace ZonaiMath
{
	const Vector4D Vector4D::Zero = {};

	Vector4D::Vector4D(const Vector3D& _other, const float _w)
		: x{ _other.x }, y{ _other.y }, z{ _other.z }, w{ _w }
	{

	}

	Vector4D::operator Matrix1x4()
	{
		return Matrix1x4{ e[0], e[1] , e[2] , e[3] };
	}

	Vector4D::operator Quaternion()
	{
		return Quaternion{ w, x, y, z };
	}

	float Vector4D::Length() const
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(m, m, 0xff)));
	}

	float Vector4D::LengthSquare() const
	{
		return _mm_cvtss_f32(_mm_dp_ps(m, m, 0xff));
	}

	Vector4D Vector4D::Cross(const Vector4D& other) const
	{
		__m128 m1 = _mm_shuffle_ps(
			this->m,
			this->m,
			_MM_SHUFFLE(3, 0, 2, 1)		// Y Z X 0
		);

		__m128 m2 = _mm_shuffle_ps(
			other.m,
			other.m,
			_MM_SHUFFLE(3, 0, 2, 1)
		);

		m1 = _mm_mul_ps(m2, this->m);
		m2 = _mm_mul_ps(m1, other.m);
		m1 = _mm_sub_ps(m1, m2);

		Vector4D temp;
		temp.m = _mm_shuffle_ps(m1, m1, _MM_SHUFFLE(3, 0, 2, 1));

		return temp;
	}

	float Vector4D::Dot(const Vector4D& other) const
	{
		// _mm_dp_ps는 각 요소를 곱해서 더해주는 기능
		// _mm_cvtss_f32는 스칼라만 뽑아주는 것.
		return _mm_cvtss_f32(_mm_dp_ps(this->m, other.m, 0xFF));
	}

	float Vector4D::FastInvSqrt(float number) const
	{
		return _mm_cvtss_f32(_mm_rsqrt_ps(_mm_set_ps1(number)));
	}

	Vector4D& Vector4D::Normalize()
	{
		float temp = LengthSquare();

		if (temp == 0)
		{
			return *this;
		}

		float invSqrt = FastInvSqrt(temp);

		this->m = _mm_mul_ps(
			this->m,
			_mm_set_ps1(invSqrt)
		);

		return *this;
	}

	Vector4D Vector4D::Normalized() const
	{
		float temp = LengthSquare();

		if (temp == 0)
		{
			// 원래는 영벡터를 반환했었지만
			// Normalize가 정규화 되지 않은 자기 자신을 반환하는 경우가 있으니
			// 자신을 복사해서 반환하는 것으로 함.
			return Vector4D(*this);
		}

		float invSqrt = FastInvSqrt(temp);

		Vector4D vec(*this);

		vec.m = _mm_mul_ps(
			this->m,
			_mm_set_ps1(invSqrt)
		);

		return vec;
	}

	float Vector4D::Dot3(const Vector4D& other) const
	{
		Vector4D temp(*this);
		temp.w = 0.f;
		temp.m = _mm_dp_ps(temp.m, other.m, 0xFF);

		return temp.m.m128_f32[0];
	}

	Vector4D& Vector4D::Normalize3()
	{
		float temp = LengthSquare();

		if (temp == 0)
		{
			return *this;
		}

		float invSqrt = FastInvSqrt(temp);
		this->m = _mm_mul_ps(this->m, _mm_set_ps1(invSqrt));
		w = 0.f;

		return *this;
	}

	Vector4D Vector4D::Normalized3() const
	{
		float temp = LengthSquare();

		if (temp == 0)
		{
			// 원래는 영벡터를 반환했었지만
			// Normalize가 정규화 되지 않은 자기 자신을 반환하는 경우가 있으니
			// 자신을 복사해서 반환하는 것으로 함.
			auto v = Vector4D(*this);
			v.w = 0.f;

			return v;
		}

		float invSqrt = FastInvSqrt(temp);

		Vector4D vec(*this);

		vec.m = _mm_mul_ps(this->m, _mm_set_ps1(invSqrt));

		return vec;
	}

	Vector4D& Vector4D::operator*=(const Matrix4x4& _mat)
	{
		Matrix4x4 temp{ _mat.Transpose() };
		__m128 m128 = m;

		this->x = _mm_cvtss_f32(_mm_dp_ps(m128, temp.m[0], 0xFF));
		this->y = _mm_cvtss_f32(_mm_dp_ps(m128, temp.m[1], 0xFF));
		this->z = _mm_cvtss_f32(_mm_dp_ps(m128, temp.m[2], 0xFF));
		this->w = _mm_cvtss_f32(_mm_dp_ps(m128, temp.m[3], 0xFF));

		return *this;
	}

	Vector4D Vector4D::operator*(const Matrix4x4& _mat) const
	{
		Vector4D temp(*this);

		return temp *= _mat;
	}

	Vector4D& Vector4D::operator+=(const Vector4D& other)
	{
		this->m = _mm_add_ps(this->m, other.m);

		return *this;
	}

	Vector4D& Vector4D::operator-=(const Vector4D& other)
	{
		this->m = _mm_sub_ps(this->m, other.m);

		return *this;
	}

	Vector4D Vector4D::operator+(const Vector4D& other) const
	{
		Vector4D temp(*this);
		return temp += other;
	}

	Vector4D Vector4D::operator-(const Vector4D& other) const
	{
		Vector4D temp(*this);
		return temp -= other;
	}

	Vector4D Vector4D::operator-() const
	{
		return Vector4D
		(
			-this->w,
			-this->x,
			-this->y,
			-this->z
		);
	}

	Vector4D& Vector4D::operator*=(const float n)
	{
		this->m = _mm_mul_ps(this->m, _mm_set_ps1(n));
		return *this;
	}

	Vector4D& Vector4D::operator/=(const float n)
	{
		this->m = _mm_div_ps(this->m, _mm_set_ps1(n));
		return *this;
	}

	Vector4D Vector4D::operator*(const float n) const
	{
		Vector4D temp(*this);
		return temp *= n;
	}

	Vector4D Vector4D::operator/(const float n) const
	{
		Vector4D temp(*this);
		return temp /= n;
	}

	bool Vector4D::operator==(const Vector4D& other) const
	{
		return (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w);
	}
}