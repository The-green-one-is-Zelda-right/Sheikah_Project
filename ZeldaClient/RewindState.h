#pragma once
#include <vector>

#include "IState.h"

namespace Phyzzle
{
	class PzObject;

	class RewindState final : public IState
	{
	public:
		RewindState() = delete;
		explicit RewindState(Player* _player)
			: IState(_player)
		{
		}
		~RewindState() override;

	private:
		void StateEnter() override;
		void StateExit() override;
		void PostStateStay() override;
		void StateStay() override;
		void StateCancel() override;

	private:
		void Stick_L() override;
		void Stick_R() override;
		void Trigger_L() override;
		void Trigger_R() override;
		void Click_DUp() override;
		void Click_DDown() override;
		void Click_DLeft() override;
		void Click_DRight() override;
		void Click_A() override;
		void Click_B() override;
		void Click_X() override;
		void Click_Y() override;
		void Click_LB() override;
		void Click_RB() override;

	private:
		void Aim();
		void Cancel();
		void Select();
		void CameraUpdate() const;
		bool Search();
		bool SearchAround();
		void EnableOutline(bool value) const;
		void AroundObjectEnableOutline(bool value);
		void CrossHeadRender(bool value) const;
		void CrossHeadSelectRender(bool value) const;
		void SearchUIRender(bool value) const;
		void SearchCatchUIRender(bool value) const;

	private:
		bool paused = false;
		bool around = false;
		bool select = false;
		int pauseLevel = 0;
		PzObject* selectObject = nullptr;
		PurahEngine::RigidBody* selectBody = nullptr;
		std::vector<PzObject*> aroundObject;
	};
}
