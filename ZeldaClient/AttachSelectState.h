#pragma once
#include "IState.h"

namespace Phyzzle
{
	class AttachSelectState final : public IState
	{
	public:
		AttachSelectState() = delete;
		explicit AttachSelectState(Player* _player)
			: IState(_player)
		{}
		~AttachSelectState() override;

#pragma region StateEvent
	private:
		// IState을(를) 통해 상속됨
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

		void Click_A() override;
		void Click_B() override;
		void Click_X() override;
		void Click_Y() override;

		void Click_LB() override;
#pragma endregion Input

#pragma region Content
	private:
		int searchUIRenderCount = 0;
		int searchCatchUIRenderCount = 0;

		bool around = false;
		bool select = false;
		PurahEngine::RigidBody* seleteBody = nullptr;
		std::vector<PzObject*> aroundObject;
		PzObject* selectObject = nullptr;

	private:
		void PlayerMove(float _speed) const;
		void CameraUpdate() const;
		void Cancel();

		void Jump() const;

		void LookToWorldDirection(const Eigen::Vector3f& _to);
		void LookToLocalDirection(const Eigen::Vector3f& _to);

		bool Search();
		bool SearchAround();
		void AroundObjectEnableOutline(bool);
		void EnableOutline(bool) const;
		void CrossHeadRender(bool _value);
		void CrossHeadSelectRender(bool _value);

		void SearchUIRender(bool _value);
		void SearchCatchUIRender(bool _value);
#pragma endregion Content
		
	private:
#pragma region Debug
		void SearchDebugDraw(bool _value);
#pragma endregion Debug
	};
}

