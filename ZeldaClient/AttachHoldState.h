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

		void VariableSet();										// ���� ����
		void VariableReset();									// ���� �ʱ�ȭ
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
		bool roateMode = false;
		bool adjustmentMode = false;

		PositionSpring posSpring;
		QuaternionSpring quatSpring;
		const float selectRange = 40.f;			// ��Ʈ���ڵ� ������ �Ÿ�
		const float pushingVelocity = 5.f;		// up, down �Է���
		const float rotateAngle = 0.25f * std::numbers::pi_v<float>;

	private:
		Eigen::Vector3f targetVelocity = Eigen::Vector3f::Zero();			// �̹� �����ӿ� ���� ���� �ӵ�
		Eigen::Vector3f targetAngularVelocity = Eigen::Vector3f::Zero();	// �̹� �����ӿ� ���� ���� ���ӵ�

		Eigen::Vector3f linearSpringForce = Eigen::Vector3f::Zero();					// ���������� ���� ����� �ӵ�
		Eigen::Vector3f angularSpringForce = Eigen::Vector3f::Zero();					// ���������� ���� ����� ���ӵ�

		Eigen::Vector3f targetPosition = Eigen::Vector3f::Zero();			// 
		Eigen::Quaternionf targetRotation = Eigen::Quaternionf::Identity();

		PurahEngine::RigidBody* selectBody;
		PzObject* attachble;

	private:
		void PlayerMove(float _speed) const;					// �̵�

#pragma region Camera
		void UpdateCamera();									// ī�޶� ������Ʈ
		void UpdateHoldingCameraPosition(Eigen::Vector3f& _local, Eigen::Vector3f& _world) const;						// ī�޶� ��ġ ������Ʈ
		void UpdateHoldingCameraRotation() const;							// ī�޶� ȸ�� ������Ʈ
		void CameraReset() const;								// ī�޶� ȸ��
#pragma endregion Camera

		void Cancel() const;										// Default ���� ���ư�
		bool TrySelect();										// ����

		void LookToWorldDirection(const Eigen::Vector3f& _to);	// �÷��̾ to �������� ȸ��
		void LookToLocalDirection(const Eigen::Vector3f& _to);	// �÷��̾ to �������� ȸ��

		void ApplyObjectVelocity() const;						// �Է��� ������Ʈ�� �����Ŵ
		void ResetObjectVelocity();								// �Է��� �ʱ�ȭ ��Ŵ

		// void SpringMassModel(const Eigen::Vector3f& worldTargetPosition);
		// void UpdateTargetPosition();

		Eigen::Vector3f GetWorldTargetPosition();
		Eigen::Quaternionf GetWorldTargetQuaternion();

		void UpdateTargetPosition();

		void CalculateSpringForces();
		void CalculateSpringPosition();
		void CalculateSpringRotation();

		// void TranslateSpringAlongY(float _distance);									// ������ �̵�
		void TranslateSpringAlongZ(float _distance);									// ������ �̵�

		void TranslateObjectAlongXZ(float _distance);									// ������Ʈ �̵�
		void TranslateObjectAlongY(float _distance);									// ������Ʈ �̵�
		// void TranslateObjectAlongZ(float _distance);									// ������Ʈ �̵�
		void TranslateObject(const Eigen::Vector3f& _direction, float power);			// ������Ʈ �̵�

		void RotateSpringAlongX(float _angle);											// ������ ȸ��
		void RotateSpringAlongY(float _angle);											// ������ ȸ��
		void RotateWithSpring(const Eigen::Vector3f& _axis, float _angle);				// ������ �̵�

		bool TryAttach() const;									// ����
		bool TryDettach() const;									// ����

		void EnableOutline(bool) const;

		void Snap();
		void Put() const;

		Eigen::Quaternionf FindAxis(const Eigen::Quaternionf& _direction);

		bool TryTranslate(float _distance);
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


