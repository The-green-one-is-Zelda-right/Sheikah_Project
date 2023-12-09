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
	constexpr float ToDegree(const float _rad) noexcept
	{
		return _rad * (180.0f / PI);
	}

	/// <summary>
	/// 각도를 라디안으로 바꿈
	/// </summary>
	constexpr float ToRadian(const float _deg) noexcept
	{
		return _deg * (PI / 180.f);
	}

#pragma region Transform
	Matrix4x4 ToTranslateMatrix(const Vector3D& _euler) noexcept;

	Matrix4x4 ToScaleMatrix(const Vector3D& _scl) noexcept;

	Matrix4x4 ToScaleMatrix(const float _scl) noexcept;

	Matrix4x4 ToTransformMatrix(const Vector3D& _pos, const Quaternion& _rot, const Vector3D& _scl) noexcept;

	Matrix4x4 ToTransformMatrix(const Vector3D& _pos, const Quaternion& _rot, const float _scl) noexcept;
#pragma endregion Transform

#pragma region Decompose
	Vector3D DecomposPosition(const Matrix4x4& _mat) noexcept;

	Quaternion DecomposRotation(const Matrix4x4& _mat) noexcept;

	Vector3D DecomposScale(const Matrix4x4& _mat) noexcept;

	void Decompose(const Matrix4x4& _matrix, Vector3D& _pos, Quaternion& _rot, Vector3D& _scl) noexcept;
#pragma endregion Decompose

#pragma region Quaternion
	Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

	Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;
#pragma endregion Quaternion


	Vector3D VectorRotateFromQuaternion(const Vector3D& _vec, const Quaternion& _q) noexcept;

	Quaternion RotateVectorToVectorQuaternion(const Vector3D& _from, const Vector3D& _to) noexcept;


#pragma region ToQuaternion
	Quaternion ToQuaternion(const Vector3D& _euler) noexcept;

	Quaternion ToQuaternion(const Vector3D& _axis, float _angle) noexcept;

	Quaternion ToQuaternion(const Matrix3x3& _rotMatrix) noexcept;

	Quaternion ToQuaternion(const Matrix4x4& _rotMatrix) noexcept;
#pragma endregion ToQuaternion

	Vector3D ToEuler(const Quaternion& _q) noexcept;


	Vector4D ToAxisAngle(const Quaternion& _q) noexcept;

#pragma region ToMatrix
	Matrix3x3 ToMatrix3(const Quaternion& _q) noexcept;

	Matrix4x4 ToMatrix4(const Quaternion& _q) noexcept;

	Matrix3x3 ToMatrix3(const Vector3D& _axis, const float _angle) noexcept;

	Matrix4x4 ToMatrix4(const Vector3D& _axis, const float _angle) noexcept;
#pragma endregion ToMatrix
}