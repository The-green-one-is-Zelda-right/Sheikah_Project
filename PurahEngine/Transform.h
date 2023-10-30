#pragma once
#include "Component.h"
#include "../ZonaiMath/ZonaiMath.h"
#pragma comment(lib, "../x64/Debug/ZonaiMath.lib")

namespace PurahEngine
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();


		// ������Ʈ�� �������� �����´�.
		ZonaiMath::Vector3D GetLocalPosition() const;
		// ������Ʈ�� �����̼��� �����´�.
		ZonaiMath::Quaternion GetLocalRotation() const;
		// ������Ʈ�� �������� �����´�.
		ZonaiMath::Vector3D GetLocalScale() const;

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
	};
}