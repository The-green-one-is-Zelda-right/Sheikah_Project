#pragma once

namespace ZonaiMath
{
	struct Vector3D;

	class Matrix1x3
	{
	public:
		/// <summary>
	/// Æ¯¼ö ¸â¹ö ÇÔ¼ö
	/// </summary>
		constexpr Matrix1x3() :
			e{}
		{}
		constexpr Matrix1x3(
			float _e00, float _e01, float _e02) :
			e{ _e00, _e01, _e02 }
		{}
		constexpr Matrix1x3(const Matrix1x3&) = default;
		constexpr Matrix1x3(Matrix1x3&&) = default;
		~Matrix1x3() = default;

		constexpr Matrix1x3& operator= (const Matrix1x3&) = default;
		constexpr Matrix1x3& operator= (Matrix1x3&&) = default;

		explicit operator Vector3D();

		/// <summary>
		/// ¸â¹ö º¯¼ö
		/// </summary>
		union
		{
			struct
			{
				float e00, e01, e02;
			};
			float e[3];
		};

		Matrix1x3& operator += (const Matrix1x3&);
		Matrix1x3& operator -= (const Matrix1x3&);

		Matrix1x3 operator + (const Matrix1x3&) const;
		Matrix1x3 operator - (const Matrix1x3&) const;
		Matrix1x3 operator - () const;

		Matrix1x3& operator *= (const float);
		Matrix1x3& operator /= (const float);

		Matrix1x3 operator * (const float) const;
		Matrix1x3 operator / (const float) const;
		bool operator == (const Matrix1x3&) const;
	};
}

