#include "Vector2D.h"
#include <cmath>

namespace ZonaiMath
{
	const Vector2D Vector2D::Zero = {};

	float Vector2D::Length() const noexcept
	{
		return std::sqrtf((x * x) + (y * y));
	}

	float Vector2D::LengthSquare() const noexcept
	{
		return (x * x) + (y * y);
	}

	float Vector2D::Cross(const Vector2D& _other) const noexcept
	{
		return x * _other.y - y * _other.x;
	}

	float Vector2D::Dot(const Vector2D& _other) const noexcept
	{
		return x * _other.x + y * _other.y;
	}

	float Vector2D::FastInvSqrt(float number) const noexcept
	{
		return 1 / std::sqrtf(number);
	}

	Vector2D& Vector2D::Normalize() noexcept
	{
		auto temp = this->LengthSquare();

		if (temp == 0.f)
		{
			return *this;
		}

		float invSqrt = FastInvSqrt(temp);
		this->x *= invSqrt;
		this->y *= invSqrt;

		return *this;
	}

	Vector2D Vector2D::Normalized() const noexcept
	{
		float temp = LengthSquare();

		if (temp == 0)
		{
			return Vector2D(*this);
		}

		float invSqrt = FastInvSqrt(temp);

		return Vector2D(x * invSqrt, y * invSqrt);
	}

	Vector2D& Vector2D::operator+=(const Vector2D& _other) noexcept
	{
		this->x += _other.x;
		this->y += _other.y;

		return *this;
	}

	Vector2D& Vector2D::operator-=(const Vector2D& _other) noexcept
	{
		this->x -= _other.x;
		this->y -= _other.y;

		return *this;
	}

	Vector2D Vector2D::operator+(const Vector2D& _other) const noexcept
	{
		Vector2D temp(*this);
		return temp += _other;
	}

	Vector2D Vector2D::operator-(const Vector2D& _other) const noexcept
	{
		Vector2D temp(*this);
		return temp -= _other;
	}

	Vector2D Vector2D::operator-() const noexcept
	{
		Vector2D temp(*this);
		return temp *= -1;
	}

	Vector2D& Vector2D::operator*=(float n) noexcept
	{
		this->x *= n;
		this->y *= n;

		return *this;
	}

	Vector2D& Vector2D::operator/=(float n) noexcept
	{
		auto temp = 1 / n;
		this->x *= temp;
		this->y *= temp;

		return *this;
	}

	Vector2D Vector2D::operator*(float n) noexcept
	{
		Vector2D temp(*this);
		return temp *= n;
	}

	Vector2D Vector2D::operator/(float n) noexcept
	{
		Vector2D temp(*this);
		return temp /= n;
	}

	Vector2D operator*(float n, const Vector2D& vec) noexcept
	{
		return Vector2D(vec.x * n, vec.y * n);
	}

	bool Vector2D::operator==(const Vector2D& _other) const noexcept
	{
		return this->x == _other.x && this->y == _other.y;
	}
}
