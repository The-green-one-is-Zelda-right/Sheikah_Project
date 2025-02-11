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
		enum RotateInfo;

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

		void VariableSet();										// 변수 저장
		void VariableReset();									// 변수 초기화
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
		const float pushingVelocity = 5.f;		// up, down 입력의
		const float rotateAngle = 0.25f * std::numbers::pi_v<float>;

	private:
		Eigen::Vector3f targetVelocity = Eigen::Vector3f::Zero();			// 이번 프레임에 생긴 종합 속도
		Eigen::Vector3f targetAngularVelocity = Eigen::Vector3f::Zero();	// 이번 프레임에 생긴 종합 각속도

		Eigen::Vector3f linearSpringForce = Eigen::Vector3f::Zero();					// 스프링으로 인해 생기는 속도
		Eigen::Vector3f angularSpringForce = Eigen::Vector3f::Zero();					// 스프링으로 인해 생기는 각속도

		Eigen::Vector3f targetPosition = Eigen::Vector3f::Zero();			// 
		Eigen::Quaternionf targetRotation = Eigen::Quaternionf::Identity();

		PurahEngine::RigidBody* selectBody;
		Phyzzle::PzObject* attachble;

	private:
		void PlayerMove(float _speed) const;					// 이동

#pragma region Camera
		void UpdateCamera();									// 카메라 업데이트
		void UpdateHoldingCameraPosition(Eigen::Vector3f& _local, Eigen::Vector3f& _world) const;						// 카메라 위치 업데이트
		void UpdateHoldingCameraRotation() const;							// 카메라 회전 업데이트
		void CameraReset() const;								// 카메라 회전
#pragma endregion Camera

		void Cancel() const;										// Default 모드로 돌아감
		bool TrySelect();										// 선택

		void LookToWorldDirection(const Eigen::Vector3f& _to);	// 플레이어가 to 방향으로 회전
		void LookToLocalDirection(const Eigen::Vector3f& _to);	// 플레이어가 to 방향으로 회전

		void ApplyObjectVelocity() const;						// 입력을 오브젝트에 적용시킴
		void ResetObjectVelocity();								// 입력을 초기화 시킴

		// void SpringMassModel(const Eigen::Vector3f& worldTargetPosition);
		// void UpdateTargetPosition();

		Eigen::Vector3f GetWorldTargetPosition();
		Eigen::Quaternionf GetWorldTargetQuaternion();

		void UpdateTargetPosition();

		void CalculateSpringForces();
		void CalculateSpringPosition();
		void CalculateSpringRotation();

		// void TranslateSpringAlongY(float _distance);									// 스프링 이동
		void TranslateSpringAlongZ(float _distance);									// 스프링 이동

		void TranslateObjectAlongXZ(float _distance);									// 오브젝트 이동
		void TranslateObjectAlongY(float _distance);									// 오브젝트 이동
		// void TranslateObjectAlongZ(float _distance);									// 오브젝트 이동
		void TranslateObject(const Eigen::Vector3f& _direction, float power);			// 오브젝트 이동

		void RotateSpringAlongX(float _angle);											// 스프링 회전
		void RotateSpringAlongY(float _angle);											// 스프링 회전
		void RotateWithSpring(const Eigen::Vector3f& _axis, float _angle);				// 스프링 이동

		bool TryAttach() const;									// 부착
		bool TryDettach();									// 부착

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
#pragma endregion Content

	private:
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


