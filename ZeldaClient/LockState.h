#pragma once
#include "IState.h"

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class PzObject;

	class LockState final : public IState
	{
	public:
		LockState() = delete;
		explicit LockState(Player* _player)
			: IState(_player)
		{}
		~LockState() override {}

	private:
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;
		void PostStateStay() override;

	private:
		void Stick_L() override;
		void Stick_R() override;
		void Click_B() override;
		void Click_LB() override;

#pragma region Content
	private:
		PurahEngine::RigidBody* result = nullptr;
		Phyzzle::PzObject* pzObject = nullptr;

	private:
		void CameraAround();
		bool Aim();
		void Select();
		void Cancel() const;
#pragma endregion Content

		// IState을(를) 통해 상속됨
		void StateCancel() override;
	};
}