#pragma once
#include "Eigen/Dense"
#include "PurahEngine.h"

namespace Phyzzle
{
	class Player;

	class PlayerCamera
	{
	public:
		explicit PlayerCamera(Player* player);

		void UpdateCamera();

		void UpdateDefaultCamera();					// 카메라 업데이트
		void UpdateSelectCamera();					// 카메라 업데이트

		void UpdateDefaultCameraCore();							// 카메라 코어 업데이트
		void UpdateSelectCameraCore();							// 카메라 코어 업데이트

		/// <summary>
		/// Camera Core 위치 계산
		/// 
		/// 카메라 위치는 Arm의 각도에 의해 계산됨
		/// </summary>
		/// <returns>카메라 코어 로컬 좌표</returns>
		void CalculateDefaultCameraCorePosition(Eigen::Vector3f& localOut, Eigen::Vector3f& worldOut, bool _isSelect);	// 카메라 위치 업데이트

		/// <summary>
		/// 카메라가 지형 지물에 충돌 되는지 체크하고 위치를 변경함
		/// </summary>
		/// <param name="pos">카메라 코어 로컬 좌표</param>
		/// <returns>지형 지물에 부딪치면 true</returns>
		bool ResolveCameraCollision(Eigen::Vector3f& localIn, Eigen::Vector3f& worldIn);

		void UpdateCameraPositionLerp();
		void UpdateCameraRotationLerp();

		void CharacterDisable();

		/// <summary>
		/// 카메라 코어의 목표 로컬 좌표를 설정함
		/// 
		/// 현재 좌표를 저장하고 목표 좌표로 보간함
		/// </summary>
		/// <param name="_worldPosision">코어의 로컬 좌표</param>
		void SetCameraCoreLocalTargetPosition(const Eigen::Vector3f& _localPosision);
		void SetCameraCoreLocalTargetRotation(const Eigen::Quaternionf& _localRotation);

		/// <summary>
		/// 카메라 코어의 목표 월드 좌표를 설정함
		/// 
		/// 현재 좌표를 저장하고 목표 좌표로 보간함
		/// </summary>
		/// <param name="_worldPosision">코어의 월드 좌표</param>
		void SetCameraCoreWorldTargetPosition(const Eigen::Vector3f& _worldPosision);
		void SetCameraCoreWorldTargetRotation(const Eigen::Quaternionf& _worldRotation);

		/// <summary>
		/// 카메라 코어가 XY, XZ평면의 어디를 보고 있는지 계산
		/// </summary>
		/// <param name="cameraPos">카메라 코어의 로컬 좌표</param>
		/// <param name="direction">카메라 코어의 로컬 방향 벡터</param>
		/// <returns>카메라 코어의 로컬 XY, XZ 평면의 한 점</returns>
		Eigen::Vector3f CalculateCameraFocusPosition(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction);

		/// <summary>
		/// 카메라 코어가 보고 있는 로컬 XY 평면의 좌표를 계산하는 함수
		/// </summary>
		/// <param name="cameraPos">카메라 코어 로컬 좌표</param>
		/// <param name="direction">카메라 코어 로컬 방향 벡터</param>
		/// <param name="out">계산된 로컬 XY 평면의 좌표</param>
		/// <returns>XY 평면과 평행하면 false</returns>
		bool IntersectXYPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out);

		/// <summary>
		/// 카메라 코어가 보고 있는 로컬 XZ 평면의 좌표를 계산하는 함수
		/// </summary>
		/// <param name="cameraPos">카메라 코어 로컬 좌표</param>
		/// <param name="direction">카메라 코어 로컬 방향 벡터</param>
		/// <param name="out">계산된 로컬 XZ 평면의 좌표</param>
		/// <returns>평면과 평행하면 false</returns>
		bool IntersectXZPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out);

		/// <summary>
		/// 카메라 암과 카메라 코어의 트랜스폼을 초기화 함
		/// </summary>
		void ResetCameraArmAndCameraCore();
		void ResetCameraCoreTarget();
		void ResetCamera();							// 카메라 위치 초기화
		void ResetCameraArm();						// 카메라 암 위치 초기화
		void ResetCameraCore();						// 카메라 코어 위치 초기화

		void SetCameraArmFoward(const Eigen::Vector3f& _direction);
		void RotateCameraArm();							// 카메라 암 회전
		void RotateCameraArmYaw(float yawAngle);		// 카메라 암 yaw 회전
		void RotateCameraArmPitch(float pitchAngle);	// 카메라 암 pitch 회전

		void CameraLookTo(const Eigen::Vector3f& _direction);
		void CameraLookAt(const Eigen::Vector3f& _position);

	private:
		friend class Player;

		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		PurahEngine::Transform* attachLowCamera0;
		PurahEngine::Transform* attachLowCamera1;
		PurahEngine::Transform* attachDefaultCamera0;
		PurahEngine::Transform* attachDefaultCamera1;
		PurahEngine::Transform* attachHighCamera0;
		PurahEngine::Transform* attachHighCamera1;

		float cameraLerpTime = 0.5f;			// 보간 시간
		float cameraLerpTime0 = 1.0f;

		Eigen::Vector3f		highPosition;
		Eigen::Vector3f		lowPosition;

		Eigen::Vector3f		differenceHigh;
		Eigen::Vector3f		differenceLow;

		Eigen::Vector3f		coreDefaultPosition;
		Eigen::Quaternionf	coreDefaultRotation;

		Eigen::Vector3f		coreSelectPosition;

		Eigen::Vector3f		armDefaultPosition;
		Eigen::Quaternionf	armDefaultRotation;

		Eigen::Vector3f coreTargetPosition;
		Eigen::Quaternionf	coreTargetRotation;

		Eigen::Vector3f armTargetPosition;
		Eigen::Quaternionf	armTargetRotation;

		float height = 1.f;
		float radius = 0.5f;

		float sensitivity = 90.f;			// 카메라 회전 속도

		float xAngle = 0.f;					// 현재 앵글
		const float limitHighAngle = 80.f;	// 하이 앵글
		const float limitLowAngle = -70.f;	// 로우 앵글

		unsigned int cameraCollisionLayers = 0;
		float cameraCollisionRadius = 5.f;
		float smoothingSpeed = 0.2f;

		Player* player;

		Eigen::Vector3f Lerp(const Eigen::Vector3f start, const Eigen::Vector3f end, float _t);
	};
}
