#include "Vector2D.h"

#include "Matrix1x2.h"

namespace ZonaiMath
{
	Matrix1x2::operator Vector2D()
	{
		return Vector2D(e00, e01);
	}

	ZonaiMath::Matrix1x2& Matrix1x2::operator+=(const Matrix1x2& other)
	{
		this->e00 += other.e00;
		this->e01 += other.e01;

		return *this;
	}

	ZonaiMath::Matrix1x2& Matrix1x2::operator-=(const Matrix1x2& other)
	{
		this->e00 -= other.e00;
		this->e01 -= other.e01;

		return *this;
	}

	ZonaiMath::Matrix1x2 Matrix1x2::operator+(const Matrix1x2& other) const
	{
		return Matrix1x2
		{
			this->e00 + other.e00,
			this->e01 + other.e01
		};
	}

	ZonaiMath::Matrix1x2 Matrix1x2::operator-(const Matrix1x2& other) const
	{
		return Matrix1x2
		{
			this->e00 - other.e00,
			this->e01 - other.e01
		};
	}

	ZonaiMath::Matrix1x2 Matrix1x2::operator-() const
	{
		return Matrix1x2
		{
			-this->e00,
			-this->e01
		};
	}

	ZonaiMath::Matrix1x2& Matrix1x2::operator*=(const float n)
	{
		this->e00 *= n;
		this->e01 *= n;

		return *this;
	}

	ZonaiMath::Matrix1x2& Matrix1x2::operator/=(const float n)
	{
		float invN = 1.f / n;
		this->e00 *= invN;
		this->e01 *= invN;

		return *this;
	}

	ZonaiMath::Matrix1x2 Matrix1x2::operator*(const float n) const
	{
		Matrix1x2 temp;
		temp.e00 = this->e00 * n;
		temp.e01 = this->e01 * n;

		return temp;
	}

	ZonaiMath::Matrix1x2 Matrix1x2::operator/(const float n) const
	{
		float invN = 1.f / n;

		Matrix1x2 temp;
		temp.e00 = this->e00 * invN;
		temp.e01 = this->e01 * invN;

		return temp;
	}

	bool Matrix1x2::operator==(const Matrix1x2& other) const
	{
		return (this->e00 == other.e00) && (this->e01 == other.e01);
	}
}