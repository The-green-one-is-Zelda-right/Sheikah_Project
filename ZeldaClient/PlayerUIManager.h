#pragma once
#include "PurahEngine.h"

namespace Phyzzle
{
	class Player;

	class PlayerUIManager
	{
	public:
		explicit PlayerUIManager(Player* player);

		void UpdateUI();

		void HideAllUI();

		void ShowAttachDefaultUI();
		void HideAttachDefaultUI();

		void ShowAttachSelectUI();
		void HideAttachSelectUI();
		
		void ShowAttachTouchUI();
		void HideAttachTouchUI();

		void ShowAttachRotateUI();
		void HideAttachRotateUI();

		void ShowDettachUI();
		void HideDettachUI();

		void ShowAttachHoldDefaultUI();
		void ShowAttachHoldRotateUI();

		void HideAttachHoldDefaultUI();
		void HideAttachHoldRotateUI();

	private:
		friend Player;

		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		PurahEngine::GameObject* Attach_Default;
		PurahEngine::GameObject* Catch_B;
		PurahEngine::GameObject* crossHead01;
		PurahEngine::GameObject* crossHead02;

		PurahEngine::GameObject* Attach_Hold_NoneStick;
		PurahEngine::GameObject* Rotation_NoneStick;
		PurahEngine::GameObject* Attach_Hold_Stick;
		PurahEngine::GameObject* Rotation_Stick;
		PurahEngine::GameObject* Stick_B;

		PurahEngine::GameObject* rotationArow;

		Player* player;
	};
}
