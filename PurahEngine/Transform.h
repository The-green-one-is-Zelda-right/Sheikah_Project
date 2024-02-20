#pragma once
#include "Component.h"
#include "PurahEngineAPI.h"

#include <Eigen\Dense>
#include <vector>

namespace PurahEngine
{
	class RigidBody;

	class PURAHENGINE_API Transform : public Component
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

		Eigen::Matrix4f GetLocalMatrix() const;

		//Eigen::Vector3f GetFront() const;

		// World
		// ������Ʈ�� ���� �������� �����´�.
		Eigen::Vector3f GetWorldPosition() const;
		// ������Ʈ�� ���� �����̼��� �����´�.
		Eigen::Quaternionf GetWorldRotation() const;
		// ������Ʈ�� ���� �������� �����´�.
		Eigen::Vector3f GetWorldScale() const;

		Eigen::Matrix4f GetWorldMatrix() const;

		Transform* GetParent();

		std::vector<PurahEngine::Transform*> GetChildren() const;

		/// set
		// Local
		// ������ ����
		void SetLocalPosition(Eigen::Vector3f setPosition);
		// �����̼� ����
		void SetLocalRotation(Eigen::Quaternionf setRotation);
		// ������ ����
		void SetLocalScale(Eigen::Vector3f setScale);

		// World
		void SetWorldPosition(Eigen::Vector3f setPosition);

		void SetWorldRotation(Eigen::Quaternionf setRotation);
		
		void SetParent(PurahEngine::Transform* parentObject);

		void SetWorldMatrix(Eigen::Matrix4f matrix);

		Eigen::Vector3f up = Eigen::Vector3f::UnitY();
		Eigen::Vector3f right = Eigen::Vector3f::UnitX();
		Eigen::Vector3f front = Eigen::Vector3f::UnitZ();

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

		RigidBody* rigidbody;
		void SetRigidBody(RigidBody* rigid);
		// �θ� Transform
		Transform* parentTransform;
		std::vector<Transform*> children;

		friend class RigidBody;
	};
}