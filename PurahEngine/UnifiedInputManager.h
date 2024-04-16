#pragma once

#include "PurahEngineAPI.h"
#include "eUnfInput.h"

#include "ePad.h"
#include "eKey.h"

#include <unordered_map>

namespace PurahEngine
{
	class PURAHENGINE_API UnifiedInputManager final
	{
	private:
		enum class KeyResult : int
		{
			Unknown,
			Entered,
			NotEntered
		};

	public:
		void Initialize();

		void Update();

		// Ű�� ��� ���ȴ°�?
		bool GetKeyDown(eUnfInput keycode);
		// Ű �Է��� 
		bool GetKey(eUnfInput keycode);
		// Ű�� ������ �ִ°�?
		bool GetKeyPressed(eUnfInput keycode);
		// Ű�� ��� �ô°�?
		bool GetKeyUp(eUnfInput keycode);
		// Ű�� ���� �ִ� �����ΰ�?
		bool GetKeyReleased(eUnfInput keycode);

	private:
		std::unordered_map<eUnfInput, std::vector<ePad>> padMap;
		std::unordered_map<eUnfInput, std::vector<eKey>> keyMap;
		std::unordered_map<eUnfInput, KeyResult> keyResultMap;

	public:
		static UnifiedInputManager& Getinstance();

	private:
		UnifiedInputManager();
		~UnifiedInputManager();
		UnifiedInputManager(const UnifiedInputManager& ref) = delete;
		UnifiedInputManager& operator=(const UnifiedInputManager& ref) = delete;
	};
}