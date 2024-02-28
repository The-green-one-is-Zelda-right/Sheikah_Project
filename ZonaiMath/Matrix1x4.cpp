#include "Vector4D.h"

#include "Matrix1x4.h"

namespace ZonaiMath
{
	Matrix1x4::operator Vector4D() const
	{
		return Vector4D{ this->e00, this->e01, this->e02, this->e03 };
	}

	Matrix1x4& Matrix1x4::operator+=(const Matrix1x4& other)
	{
		this->m = _mm_add_ps(this->m, other.m);

		return *this;
	}

	Matrix1x4& Matrix1x4::operator-=(const Matrix1x4& other)
	{
		this->m = _mm_sub_ps(this->m, other.m);

		return *this;
	}

	Matrix1x4 Matrix1x4::operator+(const Matrix1x4& other) const
	{
		return Matrix1x4(*this) += other;
	}

	Matrix1x4 Matrix1x4::operator-(const Matrix1x4& other) const
	{
		return Matrix1x4(*this) -= other;
	}

	Matrix1x4 Matrix1x4::operator-() const
	{
		return Matrix1x4
		{
			-this->e00,
			-this->e01,
			-this->e02,
			-this->e03
		};
	}

	Matrix1x4& Matrix1x4::operator*=(const float n)
	{
		this->m = _mm_mul_ps(this->m, _mm_set_ps1(n));

		return *this;
	}

	Matrix1x4& Matrix1x4::operator/=(const float n)
	{
		this->m = _mm_div_ps(this->m, _mm_set_ps1(n));

		return *this;
	}

	Matrix1x4 Matrix1x4::operator*(const float n) const
	{
		Matrix1x4 temp(*this);

		return temp *= n;
	}

	Matrix1x4 Matrix1x4::operator/(const float n) const
	{
		Matrix1x4 temp(*this);

		return temp /= n;
	}

	bool Matrix1x4::operator==(const Matrix1x4& _mat) const
	{
		return
			(e00 == _mat.e00) && 
			(e01 == _mat.e01) && 
			(e02 == _mat.e02) && 
			(e03 == _mat.e03);
	}
}