#pragma once
// #include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

// #include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

#include "Quaternion.h"

namespace ZonaiMath
{
	static constexpr float PI = 3.14159265358979323846f;
	static constexpr float Epsilon = 0.0001f;

	/// <summary>
	/// 라디안을 각도로 바꿈
	/// </summary>
	constexpr float ToDegree(const float _rad)
	{
		return _rad * (180.0f / PI);
	}

	/// <summary>
	/// 각도를 라디안으로 바꿈
	/// </summary>
	constexpr float ToRadian(const float _deg)
	{
		return _deg * (PI / 180.f);
	}

#pragma region Transform
	Matrix4x4 ToTranslateMatrix(const Vector3D& _euler);

	Matrix4x4 ToScaleMatrix(const Vector3D& _scl);

	Matrix4x4 ToScaleMatrix(const float _scl);

	Matrix4x4 ToTransformMatrix(const Vector3D& _pos, const Quaternion& _rot, const Vector3D& _scl);

	Matrix4x4 ToTransformMatrix(const Vector3D& _pos, const Quaternion& _rot, const float _scl);
#pragma endregion Transform

#pragma region Decompose
	Vector3D DecomposPosition(const Matrix4x4& _mat);

	Quaternion DecomposRotation(const Matrix4x4& _mat);

	Vector3D DecomposScale(const Matrix4x4& _mat);

	void Decompose(const Matrix4x4& _matrix, Vector3D& _pos, Quaternion& _rot, Vector3D& _scl);
#pragma endregion Decompose

#pragma region Quaternion
	Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);

	Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
#pragma endregion Quaternion


	Vector3D VectorRotateFromQuaternion(const Vector3D& _vec, const Quaternion& _q);

	Quaternion RotateVectorToVectorQuaternion(const Vector3D& _from, const Vector3D& _to);


#pragma region ToQuaternion
	Quaternion ToQuaternion(const Vector3D& _euler);

	Quaternion ToQuaternion(const Vector3D& _axis, float _angle);

	Quaternion ToQuaternion(const Matrix3x3& _rotMatrix);

	Quaternion ToQuaternion(const Matrix4x4& _rotMatrix);
#pragma endregion ToQuaternion

	Vector3D ToEuler(const Quaternion& _q);


	Vector4D ToAxisAngle(const Quaternion& _q);

#pragma region ToMatrix
	Matrix3x3 ToMatrix3(const Quaternion& _q);

	Matrix4x4 ToMatrix4(const Quaternion& _q);

	Matrix3x3 ToMatrix3(const Vector3D& _axis, const float _angle);

	Matrix4x4 ToMatrix4(const Vector3D& _axis, const float _angle);
#pragma endregion ToMatrix
}