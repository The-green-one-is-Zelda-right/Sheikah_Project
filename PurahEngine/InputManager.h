#pragma once
#include "PurahEngineAPI.h"

#include <map>
#include <vector>
#include "eKey.h"

#include <windows.h>

namespace PurahEngine
{
	class PURAHENGINE_API InputManager
	{
	private:
		InputManager();
		~InputManager();
		InputManager(const InputManager& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		InputManager& operator=(const InputManager& ref) = delete;

	public:
		void Initialize(HWND hwnd, eKey* _inputArr, UINT _size);

		// Update를 돌며 키의 입력값을 계속 체크한다.
		void Update();
		
		/// 키보드
		// 키가 방금 눌렸는가?
		bool IsKeyDown(eKey keycode);
		// 키를 눌리고 있는가?
		bool IsKeyPressed(eKey keycode);
		// 키를 방금 뗐는가?
		bool IsKeyUp(eKey keycode);

		/// 마우스

	private:
		std::vector<eKey> keys;
		std::map<eKey, short> PrevKeyState;
		std::map<eKey, short> NowKeyState;

		HWND hWnd;

	public:
		static InputManager& Getinstance();
	};
}


// /// 패드 스틱 키
//x = state.Gamepad.sThumbLX;	// 왼쪽 스틱 X축
//y = state.Gamepad.sThumbLY;	// 왼쪽 스틱 Y축
//state.Gamepad.sThumbRX;	// 오른쪽 스틱 X축
//state.Gamepad.sThumbRY;	// 오른쪽 스틱 Y축

/// 키코드 확인용
// 패드 조작키
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