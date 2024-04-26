#pragma once
#include "IState.h"

namespace Phyzzle
{
	class AttachHoldState final : public IState
	{
	public:
		AttachHoldState() = delete;
		explicit AttachHoldState(Player* _player)
			: IState(_player)
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
#pragma endregion Input

#pragma region Content
	private:
		const float selectRange = 40.f;
		const float lerpTime = 0.1f;

	private:
		bool hasGravity = false;
		float mass = -0.1f;
		float distance = -0.1f;
		Eigen::Vector3f playerVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f targetVelocity = Eigen::Vector3f::Zero();
		PurahEngine::RigidBody* selectBody = nullptr;

	private:
		void SetSelectObject();

		void PlayerMove(float _speed) const;					// �̵�
		void CameraAround() const;								// ī�޶� ȸ��
		void StateCancel() const;								// Default ���� ���ư�

		void TrySelect();										// ����

		void LookToWorldDirection(const Eigen::Vector3f& _to);	// �÷��̾ to �������� ȸ��
		void LookToLocalDirection(const Eigen::Vector3f& _to);	// �÷��̾ to �������� ȸ��

		void ApplyObjectVelocity() const;						// �Է��� ������Ʈ�� �����Ŵ
		void ResetObjectVelocity();								// �Է��� �ʱ�ȭ ��Ŵ

		void ObjectSpring();
		void ObjectTranslate(const Eigen::Vector3f& _direction, float power);	// ������Ʈ �̵�

		void XRotate() const;									// ������Ʈ ȸ��
		void YRotate() const;									// ������Ʈ ȸ��

		void Attach() const;									// ����

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