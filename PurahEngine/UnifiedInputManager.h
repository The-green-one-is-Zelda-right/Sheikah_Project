#pragma once

#include "PurahEngineAPI.h"
#include "eUnfInput.h"

namespace PurahEngine
{
	class PURAHENGINE_API UnifiedInputManager final
	{
	public:
		// Ű�� ��� ���ȴ°�?
		bool IsKeyDown(eUnfInput keycode);
		// Ű�� ������ �ִ°�?
		bool IsKeyPressed(eUnfInput keycode);
		// Ű�� ��� �ô°�?
		bool IsKeyUp(eUnfInput keycode);

	private:


	public:
		static UnifiedInputManager& Getinstance();

	private:
		UnifiedInputManager();
		~UnifiedInputManager();
		UnifiedInputManager(const UnifiedInputManager& ref) = delete;
		UnifiedInputManager& operator=(const UnifiedInputManager& ref) = delete;
	};
}