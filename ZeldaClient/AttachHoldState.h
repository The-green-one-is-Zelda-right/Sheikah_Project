#pragma once
#include "IState.h"
#include "Spring.h"

namespace Phyzzle
{
	class PzObject;

	class AttachHoldState final : public IState
	{
	public:
		AttachHoldState() = delete;
		explicit AttachHoldState(Player* _player)
			: IState(_player), selectBody(), attachble()
		{}
		~AttachHoldState() override;

#pragma region StateEvent
	private:
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;
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
		float diffWidth = -0.1f;
		float diffHeight = -0.1f;

		Eigen::Vector3f playerVelocity = Eigen::Vector3f::Zero();

		Eigen::Vector3f targetVelocity = Eigen::Vector3f::Zero();			// �̹� �����ӿ� ���� ���� �ӵ�
		Eigen::Vector3f targetAngularVelocity = Eigen::Vector3f::Zero();	// �̹� �����ӿ� ���� ���� ���ӵ�

		Eigen::Vector3f springL = Eigen::Vector3f::Zero();					// ���������� ���� ����� �ӵ�
		Eigen::Vector3f springR = Eigen::Vector3f::Zero();					// ���������� ���� ����� ���ӵ�

		Eigen::Vector3f targetPosition = Eigen::Vector3f::Zero();			// 
		Eigen::Quaternionf targetRotation = Eigen::Quaternionf::Identity();

		PurahEngine::RigidBody* selectBody;
		PzObject* attachble;

	private:
		void SetSelectObject();

		void PlayerMove(float _speed) const;					// �̵�
		void CameraAround() const;								// ī�޶� ȸ��
		void StateCancel() const;										// Default ���� ���ư�

		void TrySelect();										// ����

		void LookToWorldDirection(const Eigen::Vector3f& _to);	// �÷��̾ to �������� ȸ��
		void LookToLocalDirection(const Eigen::Vector3f& _to);	// �÷��̾ to �������� ȸ��

		void ApplyObjectVelocity() const;						// �Է��� ������Ʈ�� �����Ŵ
		void ResetObjectVelocity();								// �Է��� �ʱ�ȭ ��Ŵ

		void SpringCalculate();

		void SpringYTranslate(float _distance);									// ������ �̵�
		void SpringZTranslate(float _distance);									// ������ �̵�

		void ObjectTranslate(const Eigen::Vector3f& _direction, float power);	// ������Ʈ �̵�
		void ObjectXTranslate(float _distance);									// ������Ʈ �̵�
		void ObjectYTranslate(float _distance);									// ������Ʈ �̵�
		void ObjectZTranslate(float _distance);									// ������Ʈ �̵�

		void SpringRotate(const Eigen::Vector3f& _axis, float _angle);			// ������ �̵�
		void SpringXRotate(float _angle);										// ������ ȸ��
		void SpringYRotate(float _angle);										// ������ ȸ��

		bool TryAttach() const;									// ����
		bool TryDettach() const;									// ����

		void Put() const;

		void EnableOutline(bool ) const;

		void VariableSet();										// ���� ����
		void VariableReset();									// ���� �ʱ�ȭ
#pragma endregion Content

#if _DEBUG
	private:
		void SearchDebugDraw();

		Eigen::Vector3f debugVector0 = Eigen::Vector3f::Zero();
		Eigen::Vector3f debugVector1 = Eigen::Vector3f::Zero();
		Eigen::Vector3f debugVector2 = Eigen::Vector3f::Zero();
#endif _DEBUG
	};
}