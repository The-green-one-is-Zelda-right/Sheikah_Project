#pragma once
#include "PurahEngineAPI.h"

#include <windows.h>

#define KEY_COUNT 0xFF

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
		// Update를 돌며 키의 입력값을 계속 체크한다.
		void Update();
		
		/// 키보드
		// 키가 방금 눌렸는가?
		bool IsKeyDown(int keycode);
		// 키를 눌리고 있는가?
		bool IsKeyPressed(int keycode);
		// 키를 방금 뗐는가?
		bool IsKeyUp(int keycode);

		/// 마우스

	private:
		int PrevKeyState[KEY_COUNT] = { 0, };
		int NowKeyState[KEY_COUNT] = { 0, };

	public:
		static InputManager& Getinstance();
	};
}

