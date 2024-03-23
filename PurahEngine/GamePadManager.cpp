#include "GamePadManager.h"

namespace PurahEngine
{
	GamePadManager::GamePadManager()
	= default;

	GamePadManager::~GamePadManager()
	= default;

	void GamePadManager::Initialize(HWND _hwnd, ePad* _keyArr, int _size)
	{
		hWnd = _hwnd;

		int id = 0;

		if (_keyArr)
		{
			for (auto& i : pad)
			{
				i.Initialize(id++, _keyArr, _size);
			}
		}
		else
		{
			ePad input[] =
			{
				ePad::ePAD_A,
				ePad::ePAD_B,
				ePad::ePAD_X,
				ePad::ePAD_Y,

				ePad::ePAD_UP,
				ePad::ePAD_DOWN,
				ePad::ePAD_LEFT,
				ePad::ePAD_RIGHT,

				ePad::ePAD_SHOULDER_L,
				ePad::ePAD_SHOULDER_R,

				ePad::ePAD_START,
				ePad::ePAD_BACK,

				ePad::ePAD_STICK_CLICK_L,
				ePad::ePAD_STICK_CLICK_R,
			};

			for (auto& i : pad)
			{
				i.Initialize(id++, input, sizeof(input) / sizeof(ePad));
			}
		}
	}

	void GamePadManager::Update()
	{
		//if (hWnd != GetFocus())
		//{
		//	return;
		//}

		for (auto& i : pad)
		{
			if (i.enable)
			{
				i.Update();
			}
		}
	}

	void GamePadManager::AddGamePad(int _id)
	{
		Instance().pad[_id].enable = true;
	}

	void GamePadManager::RemoveGamePad(int _id)
	{
		Instance().pad[_id].enable = false;
	}

	GamePad* GamePadManager::GetGamePad(int _id)
	{
		return &(Instance().pad[_id]);
	}

	GamePadManager& GamePadManager::Instance()
	{
		static GamePadManager* instance = nullptr;

		if (!instance)
		{
			instance = new GamePadManager();
		}

		return *instance;
	}

	void GamePadManager::Release()
	{
		GamePadManager* instence = &Instance();

		if (instence)
		{
			delete instence;
		}
	}
}

