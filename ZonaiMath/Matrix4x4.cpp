#include "Matrix4x4.h"


namespace ZonaiMath
{
	const Matrix4x4 Matrix4x4::Zero = {};
	const Matrix4x4 Matrix4x4::Identity = {
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f,
	};

	Matrix4x4 Matrix4x4::Transpose() const
	{
		return Matrix4x4
		{
			e00, e10, e20, e30,
			e01, e11, e21, e31,
			e02, e12, e22, e32,
			e03, e13, e23, e33
		};

		/*
		__m128 vTemp1 = _mm_shuffle_ps(m[0].m, m[1].m, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 vTemp3 = _mm_shuffle_ps(m[0].m, m[1].m, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 vTemp2 = _mm_shuffle_ps(m[2].m, m[3].m, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 vTemp4 = _mm_shuffle_ps(m[2].m, m[3].m, _MM_SHUFFLE(3, 2, 3, 2));

		Matrix4x4 MT;
		MT.m[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
		MT.m[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
		MT.m[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
		MT.m[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
		*/
	}

	Matrix4x4 Matrix4x4::Inverse() const
	{
		Matrix4x4 result{};

		const float det = Determinant();

		if (det == 0.0f)
		{
			// 행렬식이 0인 경우엔 역행렬이 존재하지 않음.
			return result;
		}

		const float invDet = 1.0f / det;

		/*
		Matrix4x4 MT = (*this).Transpose();

		__m128 V00 = _mm_shuffle_ps(MT.m[2].m, MT.m[2].m, _MM_SHUFFLE(1, 1, 0, 0));
		__m128 V10 = _mm_shuffle_ps(MT.m[3].m, MT.m[3].m, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 V01 = _mm_shuffle_ps(MT.m[0].m, MT.m[0].m, _MM_SHUFFLE(1, 1, 0, 0));
		__m128 V11 = _mm_shuffle_ps(MT.m[1].m, MT.m[1].m, _MM_SHUFFLE(3, 2, 3, 2));

		__m128 V02 = _mm_shuffle_ps(MT.m[2].m, MT.m[0].m, _MM_SHUFFLE(2, 0, 2, 0));
		__m128 V12 = _mm_shuffle_ps(MT.m[3].m, MT.m[1].m, _MM_SHUFFLE(3, 1, 3, 1));

		__m128 D0 = _mm_mul_ps(V00, V10);
		__m128 D1 = _mm_mul_ps(V01, V11);
		__m128 D2 = _mm_mul_ps(V02, V12);

		V00 = _mm_shuffle_ps(MT.m[2].m, MT.m[2].m, _MM_SHUFFLE(3, 2, 3, 2));
		V10 = _mm_shuffle_ps(MT.m[3].m, MT.m[3].m, _MM_SHUFFLE(1, 1, 0, 0));
		V01 = _mm_shuffle_ps(MT.m[0].m, MT.m[0].m, _MM_SHUFFLE(3, 2, 3, 2));
		V11 = _mm_shuffle_ps(MT.m[1].m, MT.m[1].m, _MM_SHUFFLE(1, 1, 0, 0));

		V02 = _mm_shuffle_ps(MT.m[2].m, MT.m[0].m, _MM_SHUFFLE(3, 1, 3, 1));
		V12 = _mm_shuffle_ps(MT.m[3].m, MT.m[1].m, _MM_SHUFFLE(2, 0, 2, 0));

		D0 = _mm_sub_ps((D0), _mm_mul_ps((V00), (V10)));
		D1 = _mm_sub_ps((D1), _mm_mul_ps((V01), (V11)));
		D2 = _mm_sub_ps((D2), _mm_mul_ps((V02), (V12)));

		// V11 = D0Y,D0W,D2Y,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
		V00 = _mm_shuffle_ps(MT.m[1].m, MT.m[1].m, _MM_SHUFFLE(1, 0, 2, 1));
		V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
		V01 = _mm_shuffle_ps(MT.m[0].m, MT.m[0].m, _MM_SHUFFLE(0, 1, 0, 2));
		V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));

		// V13 = D1Y,D1W,D2W,D2W
		__m128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
		V02 = _mm_shuffle_ps(MT.m[3].m, MT.m[3].m, _MM_SHUFFLE(1, 0, 2, 1));
		V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
		__m128 V03 = _mm_shuffle_ps(MT.m[2].m, MT.m[2].m, _MM_SHUFFLE(0, 1, 0, 2));
		V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

		__m128 C0 = _mm_mul_ps(V00, V10);
		__m128 C2 = _mm_mul_ps(V01, V11);
		__m128 C4 = _mm_mul_ps(V02, V12);
		__m128 C6 = _mm_mul_ps(V03, V13);

		// V11 = D0X,D0Y,D2X,D2X
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
		V00 = _mm_shuffle_ps(MT.m[1].m, MT.m[1].m, _MM_SHUFFLE(2, 1, 3, 2));
		V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V01 = _mm_shuffle_ps(MT.m[0].m, MT.m[0].m, _MM_SHUFFLE(1, 3, 2, 3));
		V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));

		// V13 = D1X,D1Y,D2Z,D2Z
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
		V02 = _mm_shuffle_ps(MT.m[3].m, MT.m[3].m, _MM_SHUFFLE(2, 1, 3, 2));
		V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
		V03 = _mm_shuffle_ps(MT.m[2].m, MT.m[2].m, _MM_SHUFFLE(1, 3, 2, 3));
		V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

		//_mm_sub_ps((D0), _mm_mul_ps((V00), (V10)));
		C0 = _mm_sub_ps((C0), _mm_mul_ps((V00), (V10)));
		C2 = _mm_sub_ps((C2), _mm_mul_ps((V01), (V11)));
		C4 = _mm_sub_ps((C4), _mm_mul_ps((V02), (V12)));
		C6 = _mm_sub_ps((C6), _mm_mul_ps((V03), (V13)));

		V00 = _mm_shuffle_ps(MT.m[1].m, MT.m[1].m, _MM_SHUFFLE(0, 3, 0, 3));
		// V10 = D0Z,D0Z,D2X,D2Y
		V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
		V10 = _mm_shuffle_ps(V10, V10, _MM_SHUFFLE(0, 2, 3, 0));
		V01 = _mm_shuffle_ps(MT.m[0].m, MT.m[0].m, _MM_SHUFFLE(2, 0, 3, 1));
		// V11 = D0X,D0W,D2X,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
		V11 = _mm_shuffle_ps(V11, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V02 = _mm_shuffle_ps(MT.m[3].m, MT.m[3].m, _MM_SHUFFLE(0, 3, 0, 3));
		// V12 = D1Z,D1Z,D2Z,D2W
		V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
		V12 = _mm_shuffle_ps(V12, V12, _MM_SHUFFLE(0, 2, 3, 0));
		V03 = _mm_shuffle_ps(MT.m[2].m, MT.m[2].m, _MM_SHUFFLE(2, 0, 3, 1));
		// V13 = D1X,D1W,D2Z,D2W
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
		V13 = _mm_shuffle_ps(V13, V13, _MM_SHUFFLE(2, 1, 0, 3));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		__m128 C1 = _mm_sub_ps(C0, V00);
		C0 = _mm_add_ps(C0, V00);
		__m128 C3 = _mm_add_ps(C2, V01);
		C2 = _mm_sub_ps(C2, V01);
		__m128 C5 = _mm_sub_ps(C4, V02);
		C4 = _mm_add_ps(C4, V02);
		__m128 C7 = _mm_add_ps(C6, V03);
		C6 = _mm_sub_ps(C6, V03);

		C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
		C0 = _mm_shuffle_ps(C0, C0, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C2, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C4, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C6, _MM_SHUFFLE(3, 1, 2, 0));

		// Get the determinant
		__m128 vTemp = _mm_dp_ps(C0, MT.m[0].m, 0xFF);
		vTemp = _mm_div_ps({1.f, 1.f, 1.f, 1.f}, vTemp);

		Matrix4x4 mResult;
		mResult.m[0] = _mm_mul_ps(C0, vTemp);
		mResult.m[1] = _mm_mul_ps(C2, vTemp);
		mResult.m[2] = _mm_mul_ps(C4, vTemp);
		mResult.m[3] = _mm_mul_ps(C6, vTemp);
		return mResult;
		*/
		/// 찾긴 했는데 성능이 과연 좋을까? 싶어서 안하기로 함.

		result.e00 = (e12 * e23 * e31 - e13 * e22 * e31 + e13 * e21 * e32 - e11 * e23 * e32 - e12 * e21 * e33 + e11 * e22 * e33) * invDet;
		result.e01 = (e03 * e22 * e31 - e02 * e23 * e31 - e03 * e21 * e32 + e01 * e23 * e32 + e02 * e21 * e33 - e01 * e22 * e33) * invDet;
		result.e02 = (e02 * e13 * e31 - e03 * e12 * e31 + e03 * e11 * e32 - e01 * e13 * e32 - e02 * e11 * e33 + e01 * e12 * e33) * invDet;
		result.e03 = (e03 * e12 * e21 - e02 * e13 * e21 - e03 * e11 * e22 + e01 * e13 * e22 + e02 * e11 * e23 - e01 * e12 * e23) * invDet;
		result.e10 = (e13 * e22 * e30 - e12 * e23 * e30 - e13 * e20 * e32 + e10 * e23 * e32 + e12 * e20 * e33 - e10 * e22 * e33) * invDet;
		result.e11 = (e02 * e23 * e30 - e03 * e22 * e30 + e03 * e20 * e32 - e00 * e23 * e32 - e02 * e20 * e33 + e00 * e22 * e33) * invDet;
		result.e12 = (e03 * e12 * e30 - e02 * e13 * e30 - e03 * e10 * e32 + e00 * e13 * e32 + e02 * e10 * e33 - e00 * e12 * e33) * invDet;
		result.e13 = (e02 * e13 * e20 - e03 * e12 * e20 + e03 * e10 * e22 - e00 * e13 * e22 - e02 * e10 * e23 + e00 * e12 * e23) * invDet;
		result.e20 = (e11 * e23 * e30 - e13 * e21 * e30 + e13 * e20 * e31 - e10 * e23 * e31 - e11 * e20 * e33 + e10 * e21 * e33) * invDet;
		result.e21 = (e03 * e21 * e30 - e01 * e23 * e30 - e03 * e20 * e31 + e00 * e23 * e31 + e01 * e20 * e33 - e00 * e21 * e33) * invDet;
		result.e22 = (e01 * e13 * e30 - e03 * e11 * e30 + e03 * e10 * e31 - e00 * e13 * e31 - e01 * e10 * e33 + e00 * e11 * e33) * invDet;
		result.e23 = (e03 * e11 * e20 - e01 * e13 * e20 - e03 * e10 * e21 + e00 * e13 * e21 + e01 * e10 * e23 - e00 * e11 * e23) * invDet;
		result.e30 = (e12 * e21 * e30 - e11 * e22 * e30 - e12 * e20 * e31 + e10 * e22 * e31 + e11 * e20 * e32 - e10 * e21 * e32) * invDet;
		result.e31 = (e01 * e22 * e30 - e02 * e21 * e30 + e02 * e20 * e31 - e00 * e22 * e31 - e01 * e20 * e32 + e00 * e21 * e32) * invDet;
		result.e32 = (e02 * e11 * e30 - e01 * e12 * e30 - e02 * e10 * e31 + e00 * e12 * e31 + e01 * e10 * e32 - e00 * e11 * e32) * invDet;
		result.e33 = (e01 * e12 * e20 - e02 * e11 * e20 + e02 * e10 * e21 - e00 * e12 * e21 - e01 * e10 * e22 + e00 * e11 * e22) * invDet;

		return result;
	}

	float Matrix4x4::Determinant() const
	{
		const float det =
			e00 * e11 * e22 * e33 +
			e00 * e12 * e23 * e31 +
			e00 * e13 * e21 * e32 +
			e01 * e10 * e23 * e32 +
			e01 * e12 * e20 * e33 +
			e01 * e13 * e22 * e30 +
			e02 * e10 * e21 * e33 +
			e02 * e11 * e23 * e30 +
			e02 * e13 * e20 * e31 +
			e03 * e10 * e22 * e31 +
			e03 * e11 * e20 * e32 +
			e03 * e12 * e21 * e30 -
			e00 * e11 * e23 * e32 -
			e00 * e12 * e21 * e33 -
			e00 * e13 * e22 * e31 -
			e01 * e10 * e22 * e33 -
			e01 * e12 * e23 * e30 -
			e01 * e13 * e20 * e32 -
			e02 * e10 * e23 * e31 -
			e02 * e11 * e20 * e33 -
			e02 * e13 * e21 * e30 -
			e03 * e10 * e21 * e32 -
			e03 * e11 * e22 * e30 -
			e03 * e12 * e20 * e31;


		return det;
	}

	float Matrix4x4::Determinant3x3() const
	{
		return
			e00 * (e11 * e22 - e12 * e21) -
			e01 * (e10 * e22 - e12 * e20) +
			e02 * (e10 * e21 - e11 * e20);
	}

	Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other)
	{
		this->r[0] += other.r[0];
		this->r[1] += other.r[1];
		this->r[2] += other.r[2];
		this->r[3] += other.r[3];

		return *this;
	}

	Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other)
	{
		this->r[0] -= other.r[0];
		this->r[1] -= other.r[1];
		this->r[2] -= other.r[2];
		this->r[3] -= other.r[3];

		return *this;
	}

	Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
	{
		Matrix4x4 temp(*this);

		return temp += other;
	}

	Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const
	{
		Matrix4x4 temp(*this);

		return temp -= other;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
	{
		Matrix4x4 temp(*this);

		return temp *= other;
	}

	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other)
	{
		const Matrix4x4 temp
		(
			other.e00, other.e10, other.e20, other.e30,
			other.e01, other.e11, other.e21, other.e31,
			other.e02, other.e12, other.e22, other.e32,
			other.e03, other.e13, other.e23, other.e33
		);

		const __m128 m0{ this->m[0] };
		const __m128 m1{ this->m[1] };
		const __m128 m2{ this->m[2] };
		const __m128 m3{ this->m[3] };

		// 행렬 연산은 내적과 비슷하다...
		this->e00 = _mm_cvtss_f32(_mm_dp_ps(m0, temp.m[0], 0xff));
		this->e01 = _mm_cvtss_f32(_mm_dp_ps(m0, temp.m[1], 0xff));
		this->e02 = _mm_cvtss_f32(_mm_dp_ps(m0, temp.m[2], 0xff));
		this->e03 = _mm_cvtss_f32(_mm_dp_ps(m0, temp.m[3], 0xff));

		this->e10 = _mm_cvtss_f32(_mm_dp_ps(m1, temp.m[0], 0xff));
		this->e11 = _mm_cvtss_f32(_mm_dp_ps(m1, temp.m[1], 0xff));
		this->e12 = _mm_cvtss_f32(_mm_dp_ps(m1, temp.m[2], 0xff));
		this->e13 = _mm_cvtss_f32(_mm_dp_ps(m1, temp.m[3], 0xff));

		this->e20 = _mm_cvtss_f32(_mm_dp_ps(m2, temp.m[0], 0xff));
		this->e21 = _mm_cvtss_f32(_mm_dp_ps(m2, temp.m[1], 0xff));
		this->e22 = _mm_cvtss_f32(_mm_dp_ps(m2, temp.m[2], 0xff));
		this->e23 = _mm_cvtss_f32(_mm_dp_ps(m2, temp.m[3], 0xff));

		this->e30 = _mm_cvtss_f32(_mm_dp_ps(m3, temp.m[0], 0xff));
		this->e31 = _mm_cvtss_f32(_mm_dp_ps(m3, temp.m[1], 0xff));
		this->e32 = _mm_cvtss_f32(_mm_dp_ps(m3, temp.m[2], 0xff));
		this->e33 = _mm_cvtss_f32(_mm_dp_ps(m3, temp.m[3], 0xff));

		return *this;
	}

	Matrix4x4 Matrix4x4::operator/(const Matrix4x4& _mat) const
	{
		Matrix4x4 result{ *this };
		return result /= _mat;
	}

	Matrix4x4& Matrix4x4::operator/=(const Matrix4x4& _mat)
	{
		*this *= _mat.Inverse();

		return *this;
	}

	bool Matrix4x4::operator==(const Matrix4x4& _mat) const
	{
		return 
			(this->r[0] == _mat.r[0]) && 
			(this->r[1] == _mat.r[1]) && 
			(this->r[2] == _mat.r[2]) && 
			(this->r[3] == _mat.r[3]);
	}
}