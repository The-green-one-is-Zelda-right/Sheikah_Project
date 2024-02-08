#include <cmath>

#include "ZonaiMath.h"

namespace ZonaiMath
{
	/// <summary>
	/// 오일러각을 매트릭스로 반환
	/// </summary>
	Matrix4x4 ToTranslateMatrix(const Vector3D& _euler)
	{
		return Matrix4x4
		{
			1.f,		0.f,		0.f,		0.f,
			0.f,		1.f,		0.f,		0.f,
			0.f,		0.f,		1.f,		0.f,
			_euler.x,	_euler.y,	_euler.z,	1.f
		};
	}

	/// <summary>
	/// 스케일 매트릭스로 반환
	/// </summary>
	Matrix4x4 ToScaleMatrix(const Vector3D& _scl)
	{
		return Matrix4x4
		{
			_scl.x,		0.f,		0.f,		0.f,
			0.f,		_scl.y,		0.f,		0.f,
			0.f,		0.f,		_scl.z,		0.f,
			0.f,		0.f,		0.f,		1.f
		};
	}

	/// <summary>
	/// 스케일 매트릭스로 반환
	/// </summary>
	Matrix4x4 ToScaleMatrix(const float _scl)
	{
		return Matrix4x4
		{
			_scl,		0.f,		0.f,		0.f,
			0.f,		_scl,		0.f,		0.f,
			0.f,		0.f,		_scl,		0.f,
			0.f,		0.f,		0.f,		1.f
		};
	}

	/// <summary>
	/// 트랜스폼 매트릭스로 반환
	/// </summary>
	Matrix4x4 ToTransformMatrix(const Vector3D& _pos, const Quaternion& _rot, const Vector3D& _scl)
	{
		return ToScaleMatrix(_scl) * ToMatrix4(_rot) * ToTranslateMatrix(_pos);
	}

	Matrix4x4 ToTransformMatrix(const Vector3D& _pos, const Quaternion& _rot, const float _scl)
	{
		return ToScaleMatrix(_scl) * ToMatrix4(_rot) * ToTranslateMatrix(_pos);
	}


	Vector3D DecomposPosition(const Matrix4x4& _mat)
	{
		return Vector3D(_mat.e30, _mat.e31, _mat.e32);
	}

	Quaternion DecomposRotation(const Matrix4x4& _mat)
	{
		Vector3D scl{ DecomposScale(_mat) };

		Matrix1x3 row00{ _mat.e00, _mat.e01, _mat.e02 };
		row00 /= scl.x;
		Matrix1x3 row01{ _mat.e10, _mat.e11, _mat.e12 };
		row01 /= scl.y;
		Matrix1x3 row02{ _mat.e20, _mat.e21, _mat.e22 };
		row02 /= scl.z;

		return ToQuaternion({ row00 , row01, row02 });
	}

	Vector3D DecomposScale(const Matrix4x4& _mat)
	{
		return Vector3D(
			Vector3D(_mat.e00, _mat.e01, _mat.e02).Length(),
			Vector3D(_mat.e10, _mat.e11, _mat.e12).Length(),
			Vector3D(_mat.e20, _mat.e21, _mat.e22).Length()
		);
	}

	/// <summary>
	/// 트랜스폼 행렬 분해?
	/// </summary>
	void Decompose(const Matrix4x4& _matrix, Vector3D& _pos, Quaternion& _rot, Vector3D& _scl)
	{
		// 월드 행렬의 포지션 요소
		_pos.x = _matrix.r[3].e00;
		_pos.y = _matrix.r[3].e01;
		_pos.z = _matrix.r[3].e02;

		// 월드 행렬의 각 기저의 크기가 스케일 값
		_scl.x = Vector3D(_matrix.e00, _matrix.e10, _matrix.e20).Length();
		_scl.y = Vector3D(_matrix.e01, _matrix.e11, _matrix.e21).Length();
		_scl.z = Vector3D(_matrix.e02, _matrix.e12, _matrix.e22).Length();

		// 정규화해서 회전값만 남김
		Matrix4x4 rotationMatrix = _matrix;
		rotationMatrix.r[0] /= _scl.x;
		rotationMatrix.r[1] /= _scl.y;
		rotationMatrix.r[2] /= _scl.z;

		// 회전 행렬을 쿼터니언으로 변환
		_rot = ToQuaternion(rotationMatrix);
	}

	/// <summary>
	/// 사원수 선형 보간
	/// 정규화된 두 쿼터니언을 받아 보간함.
	/// t는 1.f ~ 0.f
	/// </summary>
	Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t)
	{
		/// Q(t) = start - t * start  + t * end
		/// Q(t)를 정규화까지 하면 된다.

		// Quaternion result{ q1 + (q2 - q1) * t };
		// return result.Normalized();

		return (q1 + (q2 - q1) * t).Normalized();
	}

	/// <summary>
	/// 사원수 구형 보간
	/// 정규화된 두 쿼터니언을 받아 보간함.
	/// t는 1.f ~ 0.f
	/// </summary>
	Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
	{
		/// Q(t) = 
		/// ( (sin(1 - t) * theta) / sin theta ) * start + 
		/// ( (sin(theta * t) ) / sin theta ) * end
		/// 그리고 정규화

		Quaternion q1N{ q1 };
		Quaternion q2N{ q2 };

		// 내적의 결과값은 cos theta
		float cosTheta = q1N.Dot(q2N);

		// cos값이 음수면 쿼터니언 하나를 음수화 한다는데
		// 이 이유를 모르겠지만 암튼 찾아본 사이트에서 그리 하라고 함.
		// 쿼터니언 사이 각도가 90도가 넘어가니까 음수 취하는게 더 짧은 경로라서 그런걸까
		// 진짜 뭔지 모르겠음

		/// 서칭 결과
		/// 두 쿼터니언 사이의 각도가 90도보다 크다는 건
		/// 구형 보간 시, 더 긴 대원호를 따라가게 된다.
		/// 따라서 짧은 대원호를 따라가게 하기 위해 음수를 취하는 것인데
		/// 쿼터니언에서는 q와 -q는 같은 회전을 의미하기 때문에 회전 결과에서는 아무런 영향이 없다.
		/// 하지만 구형 보간에서는 부호가 중요하다
		/// 구형 보간은 대원호 상에서 두 쿼터니언의 점을 구하고 잇는 것이기에
		/// 부호에 따라 방향이 달라진다.
		if (cosTheta < 0.f)
		{
			q2N = -q2N;
			cosTheta = -cosTheta;
		}

		// cos Theta가 1이 넘어가면 큰일난다!! acos에서 NaN을 띄우기 때문이다!!!
		// 이거 때문에 1쿼터 때 고생했다
		if (cosTheta > 0.9999f)
		{
			// cosTheta가 1이거나 -1인 경우는
			// q1 = q2거나 서로 정 반대라는 의미
			return Lerp(q1, q2, t);
		}

		// 쿼터니언 사이의 각도를 구함
		float theta = std::acos(cosTheta);

		// 어차피 쓸 sin theta 미리 계산해두자
		float invSinTheta = 1 / std::sin(theta);

		// 구형 보간 공식이다
		float factorA{ std::sin((1.0f - t) * theta) * invSinTheta };
		float factorB{ std::sin(t * theta) * invSinTheta };

		// Quaternion result{ (factorA * q1N) + (factorB * q2N) };
		// return result.Normalized();
		return ((factorA * q1N) + (factorB * q2N)).Normalized();
	}

	Vector3D VectorRotateFromQuaternion(const Vector3D& _vec, const Quaternion& _q)
	{
		Quaternion q_con{ _q.Conjugate() };
		Quaternion vec{ 0.f, _vec.x, _vec.y, _vec.z };

		Quaternion result{ _q * vec * q_con };

		return Vector3D(result.x, result.y, result.z);
	}

	/// <summary>
	/// 벡터를 목표하는 벡터로 변환하기 위한 쿼터니언을 반환
	/// </summary>
	Quaternion RotateVectorToVectorQuaternion(const Vector3D& _from, const Vector3D& _to)
	{
		Vector3D normalizedFrom = _from.Normalized();
		Vector3D normalizedTo = _to.Normalized();

		Vector3D rotationAxis = normalizedFrom.Cross(normalizedTo);

		float rotationAngle = std::acosf(normalizedFrom.Dot(normalizedTo));

		return ToQuaternion(rotationAxis, rotationAngle);
	}

	/// <summary>
	/// 오일러 각을 쿼터니언으로 바꿈
	/// </summary>
	Quaternion ToQuaternion(const Vector3D& _euler)
	{
		Vector3D eulerRad = _euler * 0.5f;

		float roll = eulerRad.x;        // 뱅크
		float pitch = eulerRad.z;       // 에티튜드
		float yaw = eulerRad.y;         // 헤딩

		// float roll = eulerRad.z;        // 
		// float pitch = eulerRad.x;
		// float yaw = eulerRad.y;

		float c1 = std::cosf(yaw);
		float s1 = std::sinf(yaw);
		float c2 = std::cosf(pitch);
		float s2 = std::sinf(pitch);
		float c3 = std::cosf(roll);
		float s3 = std::sinf(roll);

		float c1c2 = c1 * c2;
		float s1s2 = s1 * s2;

		Quaternion q;

		q.w = c1c2 * c3 - s1s2 * s3;
		q.x = c1c2 * s3 + s1s2 * c3;
		q.y = s1 * c2 * c3 + c1 * s2 * s3;
		q.z = c1 * s2 * c3 - s1 * c2 * s3;

		return q;
	}

	/// <summary>
	/// 축각을 쿼터니언으로 바꿈
	/// </summary>
	Quaternion ToQuaternion(const Vector3D& _axis, float _angle)
	{
		Vector3D axis = _axis.Normalized();
		float halfAngle = _angle * 0.5f;
		float sinhalf = std::sinf(halfAngle);
		float coshalf = std::cosf(halfAngle);

		return Quaternion
		(
			coshalf,
			axis.x * sinhalf,
			axis.y * sinhalf,
			axis.z * sinhalf
		);
	}

	/// <summary>
	/// 회전 행렬을 쿼터니언으로 바꿈
	/// </summary>
	Quaternion ToQuaternion(const Matrix3x3& _rotMatrix)
	{
		Quaternion quaternion;

		float trace = _rotMatrix.e00 + _rotMatrix.e11 + _rotMatrix.e22;

		if (trace > 0.0f) {
			float s = 0.5f / std::sqrt(trace + 1.0f);
			quaternion.w = 0.25f / s;
			quaternion.x = (_rotMatrix.e21 - _rotMatrix.e12) * s;
			quaternion.y = (_rotMatrix.e02 - _rotMatrix.e20) * s;
			quaternion.z = (_rotMatrix.e10 - _rotMatrix.e01) * s;
		}
		else
		{
			if (_rotMatrix.e00 > _rotMatrix.e11 && _rotMatrix.e00 > _rotMatrix.e22)
			{
				float s = 2.0f * std::sqrt(1.0f + _rotMatrix.e00 - _rotMatrix.e11 - _rotMatrix.e22);
				quaternion.w = (_rotMatrix.e21 - _rotMatrix.e12) / s;
				quaternion.x = 0.25f * s;
				quaternion.y = (_rotMatrix.e01 + _rotMatrix.e10) / s;
				quaternion.z = (_rotMatrix.e02 + _rotMatrix.e20) / s;
			}
			else if (_rotMatrix.e11 > _rotMatrix.e22) {
				float s = 2.0f * std::sqrt(1.0f + _rotMatrix.e11 - _rotMatrix.e00 - _rotMatrix.e22);
				quaternion.w = (_rotMatrix.e02 - _rotMatrix.e20) / s;
				quaternion.x = (_rotMatrix.e01 + _rotMatrix.e10) / s;
				quaternion.y = 0.25f * s;
				quaternion.z = (_rotMatrix.e12 + _rotMatrix.e21) / s;
			}
			else {
				float s = 2.0f * std::sqrt(1.0f + _rotMatrix.e22 - _rotMatrix.e00 - _rotMatrix.e11);
				quaternion.w = (_rotMatrix.e10 - _rotMatrix.e01) / s;
				quaternion.x = (_rotMatrix.e02 + _rotMatrix.e20) / s;
				quaternion.y = (_rotMatrix.e12 + _rotMatrix.e21) / s;
				quaternion.z = 0.25f * s;
			}
		}

		return quaternion;
	}

	Quaternion ToQuaternion(const Matrix4x4& _rotMatrix)
	{
		Quaternion q;
		float r22 = _rotMatrix.e22;
		if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
		{
			float dif10 = _rotMatrix.e11 - _rotMatrix.e00;
			float omr22 = 1.f - r22;
			if (dif10 <= 0.f)  // x^2 >= y^2
			{
				float fourXSqr = omr22 - dif10;
				float inv4x = 0.5f / sqrtf(fourXSqr);
				q.w = fourXSqr * inv4x;
				q.x = (_rotMatrix.e01 + _rotMatrix.e10) * inv4x;
				q.y = (_rotMatrix.e02 + _rotMatrix.e20) * inv4x;
				q.z = (_rotMatrix.e12 - _rotMatrix.e21) * inv4x;
			}
			else  // y^2 >= x^2
			{
				float fourYSqr = omr22 + dif10;
				float inv4y = 0.5f / sqrtf(fourYSqr);
				q.w = (_rotMatrix.e01 + _rotMatrix.e10) * inv4y;
				q.x = fourYSqr * inv4y;
				q.y = (_rotMatrix.e12 + _rotMatrix.e21) * inv4y;
				q.z = (_rotMatrix.e20 - _rotMatrix.e02) * inv4y;
			}
		}
		else  // z^2 + w^2 >= x^2 + y^2
		{
			float sum10 = _rotMatrix.e11 + _rotMatrix.e00;
			float opr22 = 1.f + r22;
			if (sum10 <= 0.f)  // z^2 >= w^2
			{
				float fourZSqr = opr22 - sum10;
				float inv4z = 0.5f / sqrtf(fourZSqr);
				q.w = (_rotMatrix.e02 + _rotMatrix.e20) * inv4z;
				q.x = (_rotMatrix.e12 + _rotMatrix.e21) * inv4z;
				q.y = fourZSqr * inv4z;
				q.z = (_rotMatrix.e01 - _rotMatrix.e10) * inv4z;
			}
			else  // w^2 >= z^2
			{
				float fourWSqr = opr22 + sum10;
				float inv4w = 0.5f / sqrtf(fourWSqr);
				q.w = (_rotMatrix.e12 - _rotMatrix.e21) * inv4w;
				q.x = (_rotMatrix.e20 - _rotMatrix.e02) * inv4w;
				q.y = (_rotMatrix.e01 - _rotMatrix.e10) * inv4w;
				q.z = fourWSqr * inv4w;
			}
		}

		return q;
	}

	/// <summary>
	/// 쿼터니언을 오일러각으로 바꿈
	/// </summary>
	Vector3D ToEuler(const Quaternion& q)
	{
		// YZX 오일러로 변환
		float roll, pitch, yaw;

		double sqw = q.w * q.w;
		double sqx = q.x * q.x;
		double sqy = q.y * q.y;
		double sqz = q.z * q.z;
		double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
		double test = q.x * q.y + q.z * q.w;

		if (test > 0.499f * unit)
		{ // singularity at north pole
			yaw = 2.f * std::atan2f(q.x, q.w);
			pitch = PI / 2.f;
			roll = 0.f;
			return  Vector3D(roll, yaw, pitch);
		}

		if (test < -0.499f * unit)
		{ // singularity at south pole
			yaw = -2.f * std::atan2f(q.x, q.w);
			pitch = -PI / 2.f;
			roll = 0.f;
			return  Vector3D(roll, yaw, pitch);
		}

		yaw = std::atan2f(2 * q.y * q.w - 2 * q.x * q.z, sqx - sqy - sqz + sqw);
		pitch = std::asinf(2 * test / unit);
		roll = std::atan2f(2 * q.x * q.w - 2 * q.y * q.z, -sqx + sqy - sqz + sqw);

		return Vector3D(roll, yaw, pitch);
	}

	/// <summary>
	/// 쿼터니언을 축각으로 바꿈
	/// </summary>
	Vector4D ToAxisAngle(const Quaternion& _q)
	{
		float squaredLength = _q.LengthSquare();

		if (squaredLength < Epsilon)
		{
			return Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
		}

		float inverseLength = 1.0f / std::sqrt(squaredLength);
		float angle = 2.0f * std::acos(_q.w);
		float sinAngle = std::sqrt(1.0f - _q.w * _q.w) * inverseLength;

		return Vector4D(_q.x * inverseLength, _q.y * inverseLength, _q.z * inverseLength, angle);
	}

	/// <summary>
	/// 쿼터니언을 행렬로 바꿈
	/// </summary>
	Matrix3x3 ToMatrix3(const Quaternion& _q)
	{
		Matrix3x3 matrix;

		const float w2 = _q.w * _q.w;
		const float x2 = _q.x * _q.x;
		const float y2 = _q.y * _q.y;
		const float z2 = _q.z * _q.z;
		const float xy = _q.x * _q.y;
		const float xz = _q.x * _q.z;
		const float yz = _q.y * _q.z;
		const float wx = _q.w * _q.x;
		const float wy = _q.w * _q.y;
		const float wz = _q.w * _q.z;

		matrix.e00 = 1.0f - 2.0f * (y2 + z2);
		matrix.e01 = 2.0f * (xy + wz);
		matrix.e02 = 2.0f * (xz - wy);

		matrix.e10 = 2.0f * (xy - wz);
		matrix.e11 = 1.0f - 2.0f * (x2 + z2);
		matrix.e12 = 2.0f * (yz + wx);

		matrix.e20 = 2.0f * (xz + wy);
		matrix.e21 = 2.0f * (yz - wx);
		matrix.e22 = 1.0f - 2.0f * (x2 + y2);

		return matrix;
	}

	Matrix4x4 ToMatrix4(const Quaternion& _q)
	{
		Matrix4x4 matrix;

		const float w2 = _q.w * _q.w;
		const float x2 = _q.x * _q.x;
		const float y2 = _q.y * _q.y;
		const float z2 = _q.z * _q.z;
		const float xy = _q.x * _q.y;
		const float xz = _q.x * _q.z;
		const float yz = _q.y * _q.z;
		const float wx = _q.w * _q.x;
		const float wy = _q.w * _q.y;
		const float wz = _q.w * _q.z;

		matrix.e00 = 1.0f - 2.0f * (y2 + z2);
		matrix.e01 = 2.0f * (xy + wz);
		matrix.e02 = 2.0f * (xz - wy);
		matrix.e03 = 0.0f;

		matrix.e10 = 2.0f * (xy - wz);
		matrix.e11 = 1.0f - 2.0f * (x2 + z2);
		matrix.e12 = 2.0f * (yz + wx);
		matrix.e13 = 0.0f;

		matrix.e20 = 2.0f * (xz + wy);
		matrix.e21 = 2.0f * (yz - wx);
		matrix.e22 = 1.0f - 2.0f * (x2 + y2);
		matrix.e23 = 0.0f;

		matrix.e30 = 0.0f;
		matrix.e31 = 0.0f;
		matrix.e32 = 0.0f;
		matrix.e33 = 1.0f;

		return matrix;
	}

	/// <summary>
	/// 축각을 행렬로 바꿈
	/// </summary>
	Matrix3x3 ToMatrix3(const Vector3D& _axis, const float _angle)
	{
		float cos = std::cos(_angle);
		float sin = std::sin(_angle);
		float x = _axis.x;
		float y = _axis.y;
		float z = _axis.z;

		return Matrix3x3(
			cos + x * x * (1 - cos),
			x * y * (1 - cos) - z * sin,
			x * z * (1 - cos) + y * sin,

			y * x * (1 - cos) + z * sin,
			cos + y * y * (1 - cos),
			y * z * (1 - cos) - x * sin,

			z * x * (1 - cos) - y * sin,
			z * y * (1 - cos) + x * sin,
			cos + z * z * (1 - cos)
		);
	}

	Matrix4x4 ToMatrix4(const Vector3D& _axis, const float _angle)
	{
		float cos = std::cos(_angle);
		float sin = std::sin(_angle);
		float x = _axis.x;
		float y = _axis.y;
		float z = _axis.z;

		return Matrix4x4(
			cos + x * x * (1 - cos),
			x * y * (1 - cos) - z * sin,
			x * z * (1 - cos) + y * sin,
			0.f,

			y * x * (1 - cos) + z * sin,
			cos + y * y * (1 - cos),
			y * z * (1 - cos) - x * sin,
			0.f,

			z * x * (1 - cos) - y * sin,
			z * y * (1 - cos) + x * sin,
			cos + z * z * (1 - cos),
			0.f,

			0.f,
			0.f,
			0.f,
			1.f
		);
	}
}