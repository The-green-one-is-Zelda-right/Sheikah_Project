#pragma once
#include <sal.h>

namespace ZonaiMath
{
	class Vector2D
	{
	public:
		constexpr Vector2D() : e{}
		{}
		constexpr Vector2D(float _a) :
			e{ _a, _a }
		{}
		constexpr Vector2D(float _x, float _y) :
			e{ _x, _y }
		{}
		constexpr Vector2D(const Vector2D& other) = default;
		constexpr Vector2D(Vector2D&& other) = default;
		constexpr Vector2D(_In_reads_(2) const float* arr) :
			e{ arr[0], arr[1] }
		{}
		~Vector2D() = default;

		constexpr Vector2D& operator = (const Vector2D&) = default;
		constexpr Vector2D& operator = (Vector2D&&) = default;

		union
		{
			struct
			{
				float x, y;
			};
			float e[2];
		};

		static const Vector2D Zero;

		/// º¤ÅÍ ÇÔ¼ö
		float Length() const;
		float LengthSquare() const;
		float Cross(const Vector2D&) const;
		float Dot(const Vector2D&) const;
		float FastInvSqrt(float number) const;
		Vector2D& Normalize();
		Vector2D Normalized() const;


		/// ¿¬»êÀÚ ¿À¹ö·Îµù
		Vector2D& operator += (const Vector2D&);
		Vector2D& operator -= (const Vector2D&);

		Vector2D operator + (const Vector2D&) const;
		Vector2D operator - () const;
		Vector2D operator - (const Vector2D&) const;

		Vector2D& operator*= (float);
		Vector2D& operator /= (float);

		Vector2D operator * (float);
		Vector2D operator / (float);

		friend Vector2D operator* (float, const Vector2D&);	// º¤ÅÍ ½ºÄ®¶ó °ö

		bool operator == (const Vector2D&) const;			// °°Àº º¤ÅÍÀÎ°¡?
	};

	Vector2D operator* (float, const Vector2D&);
}