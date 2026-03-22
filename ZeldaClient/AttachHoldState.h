#pragma once
#include <unordered_set>

#include "IState.h"
#include "Spring.h"

namespace Phyzzle
{
	class PzObject;

	class AttachHoldState final : public IState
	{
	public:
		enum RotateInfo : int
		{
			None		= 0,
			RotateX		= 1,
			RotateY		= 2,
			RotateX_Y	= 3,
			RotateXY	= 4,
			RotateY_X	= 5,
			RotateYX	= 6,
			RotateZ		= 7,
		};

		AttachHoldState() = delete;
		explicit AttachHoldState(Player* _player);
		~AttachHoldState() override;

#pragma region Initialize
		void InitializeAxis(std::vector<Eigen::Quaternionf>& axis, float increment);

		void InitializeAxisHelper(
			std::vector<Eigen::Quaternionf>& axis, 
			float angle, const Eigen::Vector3f& vector);
		
		void InitializeRotations(
			const std::vector<Eigen::Quaternionf>& input, 
			std::vector<Eigen::Quaternionf>& output, 
			float angle, const Eigen::Vector3f& axis, 
			RotateInfo type);

		void InitializeRotations(
			const std::vector<Eigen::Quaternionf>& input,
			float angle, const Eigen::Vector3f& axis,
			RotateInfo type);

		void VariableSet();										// 선택 대상 관련 상태를 설정
		void VariableReset();									// 선택 대상 관련 상태를 초기화
#pragma endregion Initialize

#pragma region StateEvent
	private:
		void StateEnter() override;
		
		void StateStay() override;
		void PostStateStay() override;
		
		void StateExit() override;
		
		void StateCancel() override;
#pragma endregion StateEvent

#pragma region Input
	private:
		void Stick_L() override;
		void Stick_R() override;

		void Trigger_L() override;

		void Click_A() override;
		void Click_B() override;
		void Click_X() override;
		void Click_Y() override;

		void Click_DUp() override;
		void Click_DDown() override;
		void Click_DLeft() override;
		void Click_DRight() override;

		void Pressing_DUp() override;
		void Pressing_DDown() override;
		void Pressing_DLeft() override;
		void Pressing_DRight() override;

		void Click_LB() override;

		void Pressing_RB() override;
		void Up_RB() override;
#pragma endregion Input

#pragma region Content
	private:
		bool cancel = false;
		bool around = false;
		bool roateMode = false;
		bool adjustmentMode = false;

		std::vector<Phyzzle::PzObject*> aroundObject;

		PositionSpring posSpring;
		QuaternionSpring quatSpring;
		const float pushingVelocity = 5.f;		// 상하 이동 속도
		const float rotateAngle = 0.25f * std::numbers::pi_v<float>;

	private:
		Eigen::Vector3f targetVelocity = Eigen::Vector3f::Zero();			// 목표 선형 속도
		Eigen::Vector3f targetAngularVelocity = Eigen::Vector3f::Zero();	// 목표 각속도

		Eigen::Vector3f linearSpringForce = Eigen::Vector3f::Zero();					// 선형 스프링 계산 결과
		Eigen::Vector3f angularSpringForce = Eigen::Vector3f::Zero();					// 회전 스프링 계산 결과

		Eigen::Vector3f targetPosition = Eigen::Vector3f::Zero();			// 플레이어 기준 목표 위치
		Eigen::Quaternionf targetRotation = Eigen::Quaternionf::Identity();

		PurahEngine::RigidBody* selectBody;
		Phyzzle::PzObject* attachble;

	private:
		void PlayerMove(float _speed) const;					// 플레이어 이동

#pragma region Camera
		void UpdateCamera();									// 카메라를 갱신
		void UpdateHoldingCameraPosition(Eigen::Vector3f& _local, Eigen::Vector3f& _world) const;						// 홀드 상태 카메라 위치를 갱신
		void UpdateHoldingCameraRotation() const;							// 홀드 상태 카메라 회전을 갱신
		void CameraReset() const;								// 카메라를 초기 상태로 되돌림
#pragma endregion Camera

		void Cancel() const;										// 기본 상태로 복귀
		bool TrySelect();										// 현재 홀드 대상 선택 시도

		void LookToWorldDirection(const Eigen::Vector3f& _to);	// 플레이어를 월드 방향으로 회전
		void LookToLocalDirection(const Eigen::Vector3f& _to);	// 플레이어를 로컬 방향으로 회전

		void ApplyObjectVelocity() const;						// 계산된 오브젝트 속도를 적용
		void ResetObjectVelocity();								// 오브젝트 속도 버퍼를 초기화

		// void SpringMassModel(const Eigen::Vector3f& worldTargetPosition);
		// void UpdateTargetPosition();

		Eigen::Vector3f GetWorldTargetPosition();
		Eigen::Quaternionf GetWorldTargetQuaternion();

		void UpdateTargetPosition();

		void CalculateSpringForces();
		void CalculateSpringPosition();
		void CalculateSpringRotation();

		// void TranslateSpringAlongY(float _distance);									// 스프링을 Y축으로 이동
		void TranslateSpringAlongZ(float _distance);									// 스프링을 Z축으로 이동

		void TranslateObjectAlongXZ(float _distance);									// 오브젝트를 XZ 평면으로 이동
		void TranslateObjectAlongY(float _distance);									// 오브젝트를 Y축으로 이동
		// void TranslateObjectAlongZ(float _distance);									// 오브젝트를 Z축으로 이동
		void TranslateObject(const Eigen::Vector3f& _direction, float power);			// 오브젝트 이동량을 누적

		void RotateSpringAlongX(float _angle);											// 스프링을 X축으로 회전
		void RotateSpringAlongY(float _angle);											// 스프링을 Y축으로 회전
		void RotateWithSpring(const Eigen::Vector3f& _axis, float _angle);				// 스프링 기준 회전을 적용

		bool TryAttach() const;									// 부착 시도
		bool TryDettach();									// 분리 시도

		bool SearchAround();
		void AroundObjectEnableOutline(bool _value);
		void EnableOutline(bool) const;

		void Snap();
		void Put() const;

		Eigen::Quaternionf FindAxis(const Eigen::Quaternionf& _direction);

		bool TryTranslate(float _distance);

		void AttachTouchUIRender(bool _value);
		void AttachRotateUIRender(bool _value);
		void DettachUIRender(bool _value);
		void RotationArowRender(bool _value);

		void UIDisable();

		// 리버레코 관련 함수
		void SaveRecallState();  // 현재 상태 저장
		void StartRecall();      // 리버레코 시작
		void UpdateRecall(float _deltaTime);  // 리버레코 업데이트
		void EndRecall();        // 리버레코 종료
		bool CanUseRecall() const;  // 리버레코 사용 가능 여부
#pragma endregion Content

	private:
		struct Rotate
		{
			RotateInfo info;
		};

		using RotateData = std::pair<Eigen::Quaternionf, Rotate>;
		std::vector<Eigen::Quaternionf> rotate;
		std::vector<RotateData> axisies;
		Rotate info;

	private:
#pragma region Debug
		void SearchDebugDraw();

		Eigen::Vector3f debugVector0 = Eigen::Vector3f::Zero();
		Eigen::Vector3f debugVector1 = Eigen::Vector3f::Zero();
		Eigen::Vector3f debugVector2 = Eigen::Vector3f::Zero();
#pragma endregion Debug
	};
}


