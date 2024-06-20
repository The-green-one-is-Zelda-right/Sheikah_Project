#pragma once
#include "Component.h"
#include "PurahEngineAPI.h"

#include <Eigen\Dense>
#include <vector>

namespace PurahEngine
{
	class RigidBody;
	class GameObject;

	class PURAHENGINE_API Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		// ȸ�� �Լ�
		void Rotate(const Eigen::Vector3f& axis, float angle);

		/// get
		// Local
		// ������Ʈ�� �������� �����´�.
		Eigen::Vector3f GetLocalPosition() const;
		// ������Ʈ�� �����̼��� �����´�.
		Eigen::Quaternionf GetLocalRotation() const;
		// ������Ʈ�� �������� �����´�.
		Eigen::Vector3f GetLocalScale() const;

		Eigen::Matrix4f GetLocalMatrix() const;



		// World
		// ������Ʈ�� ���� �������� �����´�.
		Eigen::Vector3f GetWorldPosition() const;
		// ������Ʈ�� ���� �����̼��� �����´�.
		Eigen::Quaternionf GetWorldRotation() const;
		// ������Ʈ�� ���� �������� �����´�.
		Eigen::Vector3f GetWorldScale() const;

		Eigen::Matrix4f GetWorldMatrix() const;

		Transform* GetParent();

		std::vector<PurahEngine::Transform*> GetChildren();

		/// set
		// Local
		// ������ ����
		void SetLocalPosition(const Eigen::Vector3f& setPosition);
		// �����̼� ����
		void SetLocalRotation(const Eigen::Quaternionf& setRotation);
		// ������ ����
		void SetLocalScale(const Eigen::Vector3f& setScale);

		// World
		void SetWorldPosition(const Eigen::Vector3f& setPosition);

		void SetWorldRotation(const Eigen::Quaternionf& setRotation);
		
		void SetParent(PurahEngine::Transform* parentObject);

		void SetWorldMatrix(const Eigen::Matrix4f& matrix);

		void DeleteChildTrans(std::vector<PurahEngine::Transform*>::iterator childIter);

	public:

		Eigen::Vector3f GetFront() const;
		Eigen::Vector3f GetUp() const;
		Eigen::Vector3f GetRight() const;

		Eigen::Vector3f up = Eigen::Vector3f::UnitY();
		Eigen::Vector3f right = Eigen::Vector3f::UnitX();
		Eigen::Vector3f front = Eigen::Vector3f::UnitZ();

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

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
		friend class GameObject;
	};
}