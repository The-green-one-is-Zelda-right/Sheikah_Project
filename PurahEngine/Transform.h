#pragma once
#include "Component.h"
#include "../ZonaiMath/ZonaiMath.h"
#pragma comment(lib, "../x64/Debug/ZonaiMath.lib")

#include <Eigen\Dense>
#include <Eigen\Core>

#include <vector>

namespace PurahEngine
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		// ȸ�� �Լ�
		void Rotate();

		/// get
		// Local
		// ������Ʈ�� �������� �����´�.
		ZonaiMath::Vector3D GetLocalPosition() const;
		// ������Ʈ�� �����̼��� �����´�.
		ZonaiMath::Quaternion GetLocalRotation() const;
		// ������Ʈ�� �������� �����´�.
		ZonaiMath::Vector3D GetLocalScale() const;

		// World
		// ������Ʈ�� ���� �������� �����´�.
		ZonaiMath::Vector3D GetWorldPosition() const;
		// ������Ʈ�� ���� �����̼��� �����´�.
		ZonaiMath::Quaternion GetWorldRotation() const;
		// ������Ʈ�� ���� �������� �����´�.
		ZonaiMath::Vector3D GetWorldScale() const;

		/// set
		// ������ ����
		void SetLocalPosition(ZonaiMath::Vector3D setPosition);
		// �����̼� ����
		void SetLocalRotation(ZonaiMath::Quaternion setRotation);
		// ������ ����
		void SetLocalScale(ZonaiMath::Vector3D setScale);
	private:
		// ��ġ
		ZonaiMath::Vector3D position;
		// �����̼�
		ZonaiMath::Quaternion rotation;
		// ������
		ZonaiMath::Vector3D scale;

		// �θ� Transform
		Transform* parentTransform;
		std::vector<Transform*> children;
	};
}