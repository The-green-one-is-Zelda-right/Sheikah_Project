#include <cmath>
#include <utility>

#include "Vector4D.h"


#include "Quaternion.h"


namespace ZonaiMath
{
	const Quaternion Quaternion::Zero = { 0.f, 0.f, 0.f, 0.f };
	const Quaternion Quaternion::Identity = { 1.f, 0.f, 0.f, 0.f };

	Quaternion::operator Vector4D() noexcept
	{
		return Vector4D{ x, y, z, w };
	}

	float Quaternion::Length() const noexcept
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(m, m, 0xff)));
	}

	float Quaternion::LengthSquare() const noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(m, m, 0xff));
	}

	float Quaternion::FastInvSqrt(float number) const noexcept
	{
		return _mm_cvtss_f32(_mm_rsqrt_ps(_mm_set_ps1(number)));
	}

	/// <summary>
	/// »ç¿ø¼ö ³»Àû
	/// </summary>
	float Quaternion::Dot(const Quaternion& other) const noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(this->m, other.m, 0xff));
	}

	/// <summary>
	/// »ç¿ø¼ö ÄÓ·¹
	/// </summary>
	Quaternion Quaternion::Conjugate() const noexcept
	{
		return Quaternion{ w, -x, -y, -z };
	}

	/// <summary>
	/// »ç¿ø¼ö ¿ª¼ö
	/// </summary>
	Quaternion Quaternion::Inverse() const noexcept
	{
		float temp = LengthSquare();

		if (temp == 0.0f)
		{
			return *this;
		}

		float invSqrt = FastInvSqrt(temp);

		return Quaternion
		(
			w * invSqrt,
			-x * invSqrt,
			-y * invSqrt,
			-z * invSqrt
		);
	}

	Quaternion& Quaternion::Normalize() noexcept
	{
		float temp = LengthSquare();

		if (temp == 0.0f)
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

	Quaternion Quaternion::Normalized() const noexcept
	{
		float temp = LengthSquare();

		if (temp == 0)
		{
			return Quaternion(*this);
		}

		float invSqrt = FastInvSqrt(temp);

		Quaternion q{ *this };

		q.m = _mm_mul_ps(
			this->m,
			_mm_set_ps1(invSqrt)
		);

		return q;
	}

	/// <summary>
	/// »ç¿ø¼ö µ¡¼À
	/// </summary>
	Quaternion& Quaternion::operator+=(const Quaternion& other) noexcept
	{
		this->m = _mm_add_ps(this->m, other.m);
		return *this;
	}

	/// <summary>
	/// »ç¿ø¼ö »¬¼À
	/// </summary>
	Quaternion& Quaternion::operator-=(const Quaternion& other) noexcept
	{
		this->m = _mm_sub_ps(this->m, other.m);
		return *this;
	}

	Quaternion Quaternion::operator+(const Quaternion& other) const noexcept
	{
		Quaternion temp(*this);
		return temp += other;
	}

	Quaternion Quaternion::operator-(const Quaternion& other) const noexcept
	{
		Quaternion temp(*this);
		return temp -= other;
	}

	Quaternion Quaternion::operator-() const noexcept
	{
		return Quaternion
		(
			-this->w,
			-this->x,
			-this->y,
			-this->z
		);
	}

	/// <summary>
	/// »ç¿ø¼ö °ö¼À
	/// </summary>
	Quaternion& Quaternion::operator*=(const Quaternion& other) noexcept
	{
		// ÇØ¹ÐÅÏ °ö
		float newW =
			this->w * other.w -
			this->x * other.x -
			this->y * other.y -
			this->z * other.z;

		float newX =
			this->w * other.x +
			this->x * other.w +
			this->y * other.z -
			this->z * other.y;

		float newY =
			this->w * other.y -
			this->x * other.z +
			this->y * other.w +
			this->z * other.x;

		float newZ =
			this->w * other.z +
			this->x * other.y -
			this->y * other.x +
			this->z * other.w;

		this->w = newW;
		this->x = newX;
		this->y = newY;
		this->z = newZ;

		return *this;
	}

	/// <summary>
	/// »ç¿ø¼ö ³ª´°¼À
	/// </summary>
	Quaternion& Quaternion::operator/=(const Quaternion& other) noexcept
	{
		Quaternion conjugate = other.Conjugate();
		float temp = other.Length();

		/// »ç¿ø¼ö ³ª´°¼ÀÀº ÇØ´ç »ç¿ø¼öÀÇ ÄÓ·¹¸¦ °öÇÏ°í
		*this *= conjugate;

		/// Å©±âÀÇ Á¦°öÀ» ³ª´« °Í°ú °°´Ù.
		*this /= temp;

		return *this;
	}

	Quaternion Quaternion::operator*(const Quaternion& other) const noexcept
	{
		Quaternion result = *this;
		result *= other;
		return result;
	}

	Quaternion Quaternion::operator/(const Quaternion& other) const noexcept
	{
		Quaternion result = *this;
		result /= other;
		return result;
	}

	/// <summary>
	/// »ç¿ø¼ö ½ºÄ®¶ó °ö¼À
	/// </summary>
	Quaternion& Quaternion::operator*=(const float n) noexcept
	{
		this->m = _mm_mul_ps(this->m, _mm_set_ps1(n));
		return *this;
	}

	/// <summary>
	/// »ç¿ø¼ö ½ºÄ®¶ó ³ª´°¼À
	/// </summary>
	Quaternion& Quaternion::operator/=(const float n) noexcept
	{
		this->m = _mm_div_ps(this->m, _mm_set_ps1(n));
		return *this;
	}

	Quaternion Quaternion::operator*(const float n) const noexcept
	{
		Quaternion result(*this);

		return result *= n;
	}

	Quaternion Quaternion::operator/(const float n) const noexcept
	{
		Quaternion result(*this);

		return result /= n;
	}

	bool Quaternion::operator==(const Quaternion& other) const noexcept
	{
		return (this->w == other.w && this->x == other.x && this->y == other.y && this->z == other.z);
	}


	Quaternion operator*=(const float n, Quaternion& other) noexcept
	{
		return other *= n;
	}

	Quaternion operator/=(const float n, Quaternion& other) noexcept
	{
		return other /= n;
	}

	Quaternion operator*(const float n, const Quaternion& other) noexcept
	{
		Quaternion result(other);

		return result *= n;
	}

	Quaternion operator/(const float n, const Quaternion& other) noexcept
	{
		Quaternion result(other);

		return result /= n;
	}
}