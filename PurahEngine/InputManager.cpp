#include "InputManager.h"

#include "TimeController.h"

namespace PurahEngine
{
	const float InputManager::firstInputDelay = 0.5f;
	const float InputManager::continuousInputCycles = 0.05f;

	InputManager::InputManager() :
		hWnd(nullptr)
	{}

	InputManager::~InputManager()
		= default;

	void InputManager::Initialize(HWND hwnd, eKey* _inputArr, UINT _size)
	{
		hWnd = hwnd;

		PrevKeyState.clear();
		NowKeyState.clear();
		keyDownElapsed.clear();
		keyState.clear();

		for (auto i = 0; i < _size; i++)
		{
			keys.push_back(_inputArr[i]);

			PrevKeyState.insert({ _inputArr[i], static_cast<short>(0) });
			NowKeyState.insert({ _inputArr[i], static_cast<short>(0) });
			keyDownElapsed.insert({ _inputArr[i], -1.0f });
			keyState.insert({ _inputArr[i], false });
		}
	}

	void InputManager::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		//if (hWnd == GetFocus())
		{
			for (const auto e : keys)
			{
				keyState[e] = false;
			}

			for (const auto e : keys)
			{
				PrevKeyState[e] = NowKeyState[e];
				NowKeyState[e] = GetAsyncKeyState(static_cast<int>(e));

				// 키가 방금 눌렸다면 키가 눌린시간을 초기화 한다.
				if (!(PrevKeyState[e] & 0x8000) && (NowKeyState[e] & 0x8000))
				{
					keyDownElapsed[e] = 0.0f;
					keyState[e] = true;
				}
				// 키가 눌렸다면 시간을 누적한다.
				else if (NowKeyState[e] & 0x8000)
				{
					keyDownElapsed[e] += deltaTime;
					if (keyDownElapsed[e] >= firstInputDelay)
					{
						keyDownElapsed[e] -= continuousInputCycles;
						keyState[e] = true;
					}
				}
			}
		}
	}

	// 키를 방금 눌렀는가
	bool InputManager::IsKeyDown(eKey keycode)
	{
		return !(PrevKeyState[keycode] & 0x8000) && (NowKeyState[keycode] & 0x8000);
	}

	// 키를 눌리고 있는가
	bool InputManager::IsKeyPressed(eKey keycode)
	{
		return (PrevKeyState[keycode] & 0x8000) && (NowKeyState[keycode] & 0x8000);
	}

	// 키를 뗐는가
	bool InputManager::IsKeyUp(eKey keycode)
	{
		return (PrevKeyState[keycode] & 0x8000) && !(NowKeyState[keycode] & 0x8000);
	}

	bool InputManager::IsKeyReleased(eKey keycode)
	{
		return !(PrevKeyState[keycode] & 0x8000) && !(NowKeyState[keycode] & 0x8000);
	}

	bool InputManager::GetKey(eKey keycode)
	{
		return keyState[keycode];
	}

	InputManager& PurahEngine::InputManager::Getinstance()
	{
		static InputManager instance;
		return instance;
	}
}