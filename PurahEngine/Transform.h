#pragma once
#include "Component.h"

#include <Eigen\Dense>
#include <vector>

namespace PurahEngine
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		// ȸ�� �Լ�
		void Rotate(Eigen::Vector3f axis, float angle);

		/// get
		// Local
		// ������Ʈ�� �������� �����´�.
		Eigen::Vector3f GetLocalPosition() const;
		// ������Ʈ�� �����̼��� �����´�.
		Eigen::Quaternionf GetLocalRotation() const;
		// ������Ʈ�� �������� �����´�.
		Eigen::Vector3f GetLocalScale() const;

		// World
		// ������Ʈ�� ���� �������� �����´�.
		Eigen::Vector3f GetWorldPosition() const;
		// ������Ʈ�� ���� �����̼��� �����´�.
		Eigen::Quaternionf GetWorldRotation() const;
		// ������Ʈ�� ���� �������� �����´�.
		Eigen::Vector3f GetWorldScale() const;

		Eigen::Matrix4f GetLocalMatrix() const;

		Eigen::Matrix4f GetWorldMatrix() const;

		/// set
		// ������ ����
		void SetLocalPosition(Eigen::Vector3f setPosition);
		// �����̼� ����
		void SetLocalRotation(Eigen::Quaternionf setRotation);
		// ������ ����
		void SetLocalScale(Eigen::Vector3f setScale);
	private:
		// ��ġ
		Eigen::Vector3f position;
		// �����̼�
		Eigen::Quaternionf rotation;
		// ������
		Eigen::Vector3f scale;
		// ���
		/// �຤�ʹ� ���� * SRT, �����ʹ� TRS * ����
		/// Eigen�� �����ʹ�.


		// �θ� Transform
		Transform* parentTransform;
		std::vector<Transform*> children;
	};
}