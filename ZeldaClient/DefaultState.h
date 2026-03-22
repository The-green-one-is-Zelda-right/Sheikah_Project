#pragma once
#include "IState.h"
#include "Coroutine.h"

namespace Phyzzle
{
	class DefaultState final : public IState
	{


	public:
		DefaultState() = delete;
		explicit DefaultState(Player* _player)
			: IState(_player)
		{}
		~DefaultState() override;

	private:
		// IState을(를) 통해 상속됨
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;
		void PostStateStay() override;
		void StateCancel() override;

	private:
		void Stick_L() override;
		void Stick_R() override;

		void Click_A() override;

		void Click_LB() override;

	private:
		void Jump() const;
		void CameraUpdate();
		void Move() const;
		void Around() const;
		void LookToWorldDirection(const Eigen::Vector3f& _to) const;
		void LookToLocalDirection(const Eigen::Vector3f& _to) const;
	};
}
