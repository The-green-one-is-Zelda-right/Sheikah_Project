﻿#pragma once

#include "../../FloaterMath/include/Matrix4f.h"
#include "../../FloaterMath/include/Quaternion.h"
#include "../../FloaterMath/include/Vector3f.h"
#include "../../FloaterMath/include/Vector4f.h"
#include <vector>


namespace flt
{
	class Transform
	{
	public:
		Transform() : _position(), _scale(1.0f, 1.0f, 1.0f, 0.0f), _rotation(), _pParent(nullptr), _children(), _worldMatrix(), _isDirty(true) {}
		~Transform();

		void SetMatrix(const Matrix4f& worldMatrix);

		Vector4f GetPosition() const noexcept { return _position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const Vector4f& position);

		Quaternion GetRotation() const noexcept { return _rotation; }
		void SetRotation(float degreeX, float degreeY, float degreeZ);
		void SetRotation(const Vector3f& axis, float radian);
		void SetRotation(const Quaternion& q);

		Vector4f GetScale() const noexcept { return _scale; }
		void SetScale(float x, float y, float z);
		void SetScale(const Vector4f& scale);

		void AddPosition(float x, float y, float z);
		void AddPosition(const Vector4f& position);
		void AddRotation(const Vector3f& axis, float radian);
		void AddRotation(const Quaternion& q);
		void AddScale(float x, float y, float z);

		Matrix4f GetTransformMatrix4f() const noexcept;
		Matrix4f GetRotationMatrix4f() const noexcept;
		Matrix4f GetScaleMatrix4f() const noexcept;
		Matrix4f GetLocalMatrix4f() const noexcept;
		Matrix4f GetWorldMatrix4f() noexcept;

		void LookAt(Vector4f target);

		Transform* GetParent() const noexcept { return _pParent; }
		bool SetParent(Transform* pParent);

		Transform* GetChild(size_t index) const noexcept { return _children[index]; }
		const std::vector<Transform*>& GetChildren() const noexcept { return _children; }

	private:
		void MakeDirtyRecursive() noexcept;
		void CalcWorldMatrixRecursive() noexcept;


	private:
		Vector4f _position;
		Vector4f _scale;
		Quaternion _rotation;

		Transform* _pParent;
		std::vector<Transform*> _children;

		Matrix4f _worldMatrix;
		bool _isDirty;
	};
}
