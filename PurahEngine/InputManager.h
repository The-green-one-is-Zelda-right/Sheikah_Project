#pragma once
#include "PurahEngineAPI.h"

#include <windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")


#define KEY_COUNT 0xFF
#define CONTROLLER_COUNT 0xFFFF

namespace PurahEngine
{
	class PURAHENGINE_API InputManager
	{
	private:
		InputManager();
		~InputManager();
		InputManager(const InputManager& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		InputManager& operator=(const InputManager& ref) = delete;

	public:
		void Initialize(HWND hwnd);

		// Update�� ���� Ű�� �Է°��� ��� üũ�Ѵ�.
		void Update();
		
		/// Ű����
		// Ű�� ��� ���ȴ°�?
		bool IsKeyDown(int keycode);
		// Ű�� ������ �ִ°�?
		bool IsKeyPressed(int keycode);
		// Ű�� ��� �ô°�?
		bool IsKeyUp(int keycode);

		/// �����е�
		XINPUT_STATE GetState();
		int deadZone = 5000;


		/// ���콺

	private:
		int PrevKeyState[KEY_COUNT] = { 0, };
		int NowKeyState[KEY_COUNT] = { 0, };

		XINPUT_STATE state;
		DWORD result;
		HWND hWnd;
	public:
		static InputManager& Getinstance();
	};
}


// /// �е� ��ƽ Ű
//x = state.Gamepad.sThumbLX;	// ���� ��ƽ X��
//y = state.Gamepad.sThumbLY;	// ���� ��ƽ Y��
//state.Gamepad.sThumbRX;	// ������ ��ƽ X��
//state.Gamepad.sThumbRY;	// ������ ��ƽ Y��

/// Ű�ڵ� Ȯ�ο�
// �е� ����Ű
//#define XINPUT_GAMEPAD_DPAD_UP          0x0001
//#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
//#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
//#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
//#define XINPUT_GAMEPAD_START            0x0010
//#define XINPUT_GAMEPAD_BACK             0x0020
//#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
//#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
//#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
//#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
//#define XINPUT_GAMEPAD_A                0x1000
//#define XINPUT_GAMEPAD_B                0x2000
//#define XINPUT_GAMEPAD_X                0x4000
//#define XINPUT_GAMEPAD_Y                0x8000