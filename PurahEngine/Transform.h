#pragma once
#include "Component.h"
#include "../ZonaiMath/ZonaiMath.h"
#pragma comment(lib, "../x64/Debug/ZonaiMath.lib")

#include <Eigen\Dense>
#include <vector>

namespace PurahEngine
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		// 회전 함수
		void Rotate();

		/// get
		// Local
		// 오브젝트의 포지션을 가져온다.
		Eigen::Vector3f GetLocalPosition() const;
		// 오브젝트의 로테이션을 가져온다.
		Eigen::Quaternionf GetLocalRotation() const;
		// 오브젝트의 스케일을 가져온다.
		Eigen::Vector3f GetLocalScale() const;

		// World
		// 오브젝트의 월드 포지션을 가져온다.
		Eigen::Vector3f GetWorldPosition() const;
		// 오브젝트의 월드 로테이션을 가져온다.
		Eigen::Quaternionf GetWorldRotation() const;
		// 오브젝트의 월드 스케일을 가져온다.
		Eigen::Vector3f GetWorldScale() const;

		/// set
		// 포지션 세팅
		void SetLocalPosition(Eigen::Vector3f setPosition);
		// 로테이션 세팅
		void SetLocalRotation(Eigen::Quaternionf setRotation);
		// 스케일 세팅
		void SetLocalScale(Eigen::Vector3f setScale);
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
		// 월드행렬
		Eigen::Matrix4f WorldMatrix;
		// 로컬행렬
		Eigen::Matrix4f LocalMatrix;


		// 부모 Transform
		Transform* parentTransform;
		std::vector<Transform*> children;
	};
}