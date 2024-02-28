#include "Matrix2x2.h"

namespace ZonaiMath
{
	const Matrix2x2 Matrix2x2::Zero = {};
	const Matrix2x2 Matrix2x2::Identity = {
				1.f, 0.f,
				0.f, 1.f,
	};

	float Matrix2x2::Determinant()
	{
		return 0.f;
	}

	ZonaiMath::Matrix2x2 Matrix2x2::Transpose() const
	{
		return Matrix2x2
		{
			e00, e10,
			e01, e11
		};
	}

	ZonaiMath::Matrix2x2 Matrix2x2::Inverse() const
	{
		return {};
	}

	ZonaiMath::Matrix2x2 Matrix2x2::operator+(const Matrix2x2& other) const
	{
		return {
			(this->e[0][0] + other.e[0][0]),
			(this->e[0][1] + other.e[0][1]),

			(this->e[1][0] + other.e[1][0]),
			(this->e[1][1] + other.e[1][1])
		};
	}

	ZonaiMath::Matrix2x2& Matrix2x2::operator+=(const Matrix2x2& other)
	{
		this->e00 += other.e00;
		this->e01 += other.e01;

		this->e10 += other.e10;
		this->e11 += other.e11;

		return *this;
	}

	ZonaiMath::Matrix2x2 Matrix2x2::operator-(const Matrix2x2& other) const
	{
		return {
			(this->e[0][0] - other.e[0][0]),
			(this->e[0][1] - other.e[0][1]),

			(this->e[1][0] - other.e[1][0]),
			(this->e[1][1] - other.e[1][1])
		};
	}

	ZonaiMath::Matrix2x2& Matrix2x2::operator-=(const Matrix2x2& other)
	{
		this->e00 -= other.e00;
		this->e01 -= other.e01;

		this->e10 -= other.e10;
		this->e11 -= other.e11;

		return *this;
	}

	ZonaiMath::Matrix2x2 Matrix2x2::operator*(const Matrix2x2& other) const
	{
		Matrix2x2 temp(*this);

		return temp *= other;
	}

	ZonaiMath::Matrix2x2& Matrix2x2::operator*=(const Matrix2x2& other)
	{
		const Matrix2x2 temp(*this);

		this->e00 = (temp.e[0][0] * other.e[0][0] + temp.e[0][1] * other.e[1][0]);
		this->e01 = (temp.e[0][0] * other.e[0][1] + temp.e[0][1] * other.e[1][1]);

		this->e10 = (temp.e[1][0] * other.e[0][0] + temp.e[1][1] * other.e[1][0]);
		this->e11 = (temp.e[1][0] * other.e[0][1] + temp.e[1][1] * other.e[1][1]);

		return *this;
	}

	bool Matrix2x2::operator==(const Matrix2x2& other) const
	{
		return
			(this->e00 == other.e00) &&
			(this->e01 == other.e01) &&
			(this->e10 == other.e10) &&
			(this->e11 == other.e11);
	}
}