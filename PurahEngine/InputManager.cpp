#include "InputManager.h"

namespace PurahEngine
{
	InputManager::InputManager() :
		hWnd(nullptr)
	{}

	InputManager::~InputManager()
		= default;

	void InputManager::Initialize(HWND hwnd, eKey* _inputArr, UINT _size)
	{
		hWnd = hwnd;

		for (auto i = 0; i < _size; i++)
		{
			keys.push_back(_inputArr[i]);
		}

		PrevKeyState.clear();
		NowKeyState.clear();
	}

	void InputManager::Update()
	{
		if (hWnd == GetFocus())
		{
			for (const auto e : keys)
			{
				PrevKeyState[e] = NowKeyState[e];
				NowKeyState[e] = GetAsyncKeyState(static_cast<int>(e));
			}
		}
	}

	// 키를 방금 눌렀는가
	bool InputManager::IsKeyDown(eKey keycode)
	{
		return (PrevKeyState[keycode] == 0) && (NowKeyState[keycode] & 0x8001);
	}

	// 키를 눌리고 있는가
	bool InputManager::IsKeyPressed(eKey keycode)
	{
		return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] & 0x8001);
	}

	// 키를 뗐는가
	bool InputManager::IsKeyUp(eKey keycode)
	{
		return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] == 0);
	}

	InputManager& PurahEngine::InputManager::Getinstance()
	{
		static InputManager instance;
		return instance;
	}
}