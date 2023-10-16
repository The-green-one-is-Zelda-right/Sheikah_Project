#pragma once

#include <windows.h>

#define KEY_COUNT 0xFF

namespace PurahEngine
{
	class InputManager
	{
	private:
		InputManager();
		~InputManager();
		InputManager(const InputManager& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		InputManager& operator=(const InputManager& ref) = delete;

	public:
		// Update�� ���� Ű�� �Է°��� ��� üũ�Ѵ�.
		void Update();
		// Ű�� ��� ���ȴ°�?
		bool IsKeyPressedNow(int keycode);
		// Ű�� ������ �ִ°�?
		bool IsKeyPressed(int keycode);
		// Ű�� ��� �ô°�?
		bool IsKeyReleasedNow(int keycode);
		// Ű�� ������ �ִ°�?
		bool IskeyRelease(int keycode);

	private:
		int PrevKeyState[KEY_COUNT] = { 0, };
		int NowKeyState[KEY_COUNT] = { 0, };

	public:
		static InputManager& Getinstance();
	};
}

