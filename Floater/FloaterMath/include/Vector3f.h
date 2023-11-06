#pragma once

#include <cmath>


namespace flt
{
	struct __declspec(dllexport) Vector3f
	{
		constexpr Vector3f() noexcept : m{ 0.0f, 0.0f, 0.0f } {}
		constexpr Vector3f(float x, float y, float z) noexcept : m{ x, y, z } {}
		constexpr Vector3f(Vector3f&&) noexcept = default;
		constexpr Vector3f(const Vector3f&) noexcept = default;
		constexpr Vector3f& operator=(Vector3f&&) noexcept = default;
		constexpr Vector3f& operator=(const Vector3f&) noexcept = default;
		~Vector3f() noexcept = default;

		//operator Vector4f() const noexcept;

		constexpr Vector3f& operator+=(const Vector3f& rhs) noexcept
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		constexpr Vector3f& operator-=(const Vector3f& rhs) noexcept
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		constexpr Vector3f& operator*=(const float rhs) noexcept
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}
		constexpr Vector3f& operator/=(const float rhs) noexcept
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}
		constexpr Vector3f operator+(const Vector3f& rhs) const noexcept
		{
			return Vector3f(*this) += rhs;
		}
		constexpr Vector3f operator-(const Vector3f& rhs) const noexcept
		{
			return Vector3f(*this) -= rhs;
		}
		constexpr Vector3f operator*(const float rhs) const noexcept
		{
			return Vector3f(*this) *= rhs;
		}
		constexpr Vector3f operator/(const float rhs) const noexcept
		{
			return Vector3f(*this) /= rhs;
		}
		constexpr Vector3f operator-() const noexcept
		{
			return Vector3f(-x, -y, -z);
		}

		float Norm() const noexcept
		{
			return sqrtf(x * x + y * y + z * z);
		}

		Vector3f& Normalize() noexcept
		{
			float norm = Norm();
			if (norm == 0)
			{
				return *this;
			}

			*this /= norm;
			return *this;
		}

		Vector3f Normalized() const noexcept
		{
			return Vector3f(*this).Normalize();
		}

		float Dot(const Vector3f& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		Vector3f Cross(const Vector3f& rhs) const noexcept
		{
			return Vector3f(y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x);
		}

	public:
		union
		{
			float m[3];
			struct { float x, y, z; };
			struct { float r, g, b; };
		};

		static Vector3f Lerp(const Vector3f& v0, const Vector3f& v1, float t) noexcept;
	};

}
