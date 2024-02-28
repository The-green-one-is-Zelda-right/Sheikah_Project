#pragma once
#include <smmintrin.h>

namespace ZonaiMath
{
	class Vector4D;

	class Matrix1x4
	{
	public:
		/// <summary>
		/// Æ¯¼ö ¸â¹ö ÇÔ¼ö
		/// </summary>
		constexpr Matrix1x4() :
			e{}
		{}
		constexpr Matrix1x4(
			__m128 _m) :
			m{ _m }
		{}
		constexpr Matrix1x4(
			float _e00, float _e01, float _e02, float _e03) :
			e{ _e00, _e01, _e02, _e03 }
		{}
		constexpr Matrix1x4(const Matrix1x4&) = default;
		constexpr Matrix1x4(Matrix1x4&&) = default;
		~Matrix1x4() = default;

		constexpr Matrix1x4& operator= (const Matrix1x4&) = default;
		constexpr Matrix1x4& operator= (Matrix1x4&&) = default;

		explicit operator Vector4D() const;

		/// <summary>
		/// ¸â¹ö º¯¼ö
		/// </summary>
		union
		{
			struct
			{
				float e00, e01, e02, e03;
			};
			float e[4];
			__m128 m;
		};

		Matrix1x4& operator += (const Matrix1x4&);
		Matrix1x4& operator -= (const Matrix1x4&);

		Matrix1x4 operator + (const Matrix1x4&) const;
		Matrix1x4 operator - (const Matrix1x4&) const;
		Matrix1x4 operator - () const;

		Matrix1x4& operator *= (const float);
		Matrix1x4& operator /= (const float);

		Matrix1x4 operator * (const float) const;
		Matrix1x4 operator / (const float) const;

		bool operator == (const Matrix1x4&) const;
	};
}
