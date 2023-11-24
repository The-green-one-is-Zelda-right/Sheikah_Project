#pragma once

#include <cmath>

namespace flt
{
	struct __declspec(dllexport) Vector2f
	{
		constexpr Vector2f() noexcept : m{ 0.0f, 0.0f } {}
		constexpr Vector2f(float x, float y) noexcept : m{ x, y } {}
		constexpr Vector2f(double x, double y) noexcept : m{ static_cast<float>(x), static_cast<float>(y) } {}
		constexpr Vector2f(Vector2f&&) noexcept = default;
		constexpr Vector2f(const Vector2f&) noexcept = default;
		constexpr Vector2f& operator=(Vector2f&&) noexcept = default;
		constexpr Vector2f& operator=(const Vector2f&) noexcept = default;
		~Vector2f() noexcept = default;

		Vector2f& operator+=(const Vector2f& rhs) noexcept
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		Vector2f& operator-=(const Vector2f& rhs) noexcept
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		Vector2f& operator*=(const float rhs) noexcept
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}
		Vector2f& operator/=(const float rhs) noexcept
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}
		Vector2f operator+(const Vector2f& rhs) const noexcept
		{
			return Vector2f(*this) += rhs;
		}
		Vector2f operator-(const Vector2f& rhs) const noexcept
		{
			return Vector2f(*this) -= rhs;
		}
		Vector2f operator*(const float rhs) const noexcept
		{
			return Vector2f(*this) *= rhs;
		}
		Vector2f operator/(const float rhs) const noexcept
		{
			return Vector2f(*this) /= rhs;
		}
		Vector2f operator-() const noexcept
		{
			return Vector2f(-x, -y);
		}

		float Norm() const noexcept
		{
			return sqrtf(x * x + y * y);
		}

		Vector2f& Normalize() noexcept
		{
			float norm = Norm();
			if (norm == 0)
			{
				return *this;
			}

			*this /= norm;
			return *this;
		}

		Vector2f Normalized() const noexcept
		{
			return Vector2f(*this).Normalize();
		}

		float Dot(const Vector2f& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y;
		}

	public:
		union
		{
			float m[2];
			struct { float x, y; };
			struct { float u, v; };
		};
	};
}
