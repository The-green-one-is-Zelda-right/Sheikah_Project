#include "InputManager.h"

PurahEngine::InputManager::InputManager()
{

}

PurahEngine::InputManager::~InputManager()
{

}

void PurahEngine::InputManager::Initialize()
{
	result = XInputGetState(0, &state);

	if (result == ERROR_SUCCESS)
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));
	}
}

void PurahEngine::InputManager::Update()
{
	for (int i = 0; i < KEY_COUNT; i++)
	{
		PrevKeyState[i] = NowKeyState[i];
		NowKeyState[i] = GetAsyncKeyState(i);
	}

	if (result == ERROR_SUCCESS)
	{
		XInputGetState(0, &state);

		if (abs(state.Gamepad.sThumbLX) < deadZone)
		{
			state.Gamepad.sThumbLX = 0;
		}

		if (abs(state.Gamepad.sThumbLY) < deadZone)
		{
			state.Gamepad.sThumbLY = 0;
		}
	}

}

// 키를 방금 눌렀는가
bool PurahEngine::InputManager::IsKeyDown(int keycode)
{
	return (PrevKeyState[keycode] == 0) && (NowKeyState[keycode] & 0x8001);
}

// 키를 눌리고 있는가
bool PurahEngine::InputManager::IsKeyPressed(int keycode)
{
	return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] & 0x8001);
}

// 키를 뗐는가
bool PurahEngine::InputManager::IsKeyUp(int keycode)
{
	return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] == 0);
}


XINPUT_STATE PurahEngine::InputManager::GetState()
{
	return state;
}

PurahEngine::InputManager& PurahEngine::InputManager::Getinstance()
{
	static InputManager instance;
	return instance;
}