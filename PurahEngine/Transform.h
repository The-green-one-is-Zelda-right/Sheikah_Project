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

		// 회전 함수
		void Rotate(Eigen::Vector3f axis, float angle);

		/// get
		// Local
		// 오브젝트의 포지션을 가져온다.
		Eigen::Vector3f GetLocalPosition() const;
		// 오브젝트의 로테이션을 가져온다.
		Eigen::Quaternionf GetLocalRotation() const;
		// 오브젝트의 스케일을 가져온다.
		Eigen::Vector3f GetLocalScale() const;

		Eigen::Matrix4f GetLocalMatrix() const;

		//Eigen::Vector3f GetFront() const;

		// World
		// 오브젝트의 월드 포지션을 가져온다.
		Eigen::Vector3f GetWorldPosition() const;
		// 오브젝트의 월드 로테이션을 가져온다.
		Eigen::Quaternionf GetWorldRotation() const;
		// 오브젝트의 월드 스케일을 가져온다.
		Eigen::Vector3f GetWorldScale() const;

		Eigen::Matrix4f GetWorldMatrix() const;

		Transform* GetParent();

		std::vector<PurahEngine::Transform*> GetChildren() const;

		/// set
		// Local
		// 포지션 세팅
		void SetLocalPosition(Eigen::Vector3f setPosition);
		// 로테이션 세팅
		void SetLocalRotation(Eigen::Quaternionf setRotation);
		// 스케일 세팅
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
		// 위치
		Eigen::Vector3f position;
		// 로테이션
		Eigen::Quaternionf rotation;
		// 스케일
		Eigen::Vector3f scale;
		// 행렬
		/// 행벡터는 벡터 * SRT, 열벡터는 TRS * 벡터
		/// Eigen은 열벡터다.

		RigidBody* rigidbody;
		void SetRigidBody(RigidBody* rigid);
		// 부모 Transform
		Transform* parentTransform;
		std::vector<Transform*> children;

		friend class RigidBody;
	};
}