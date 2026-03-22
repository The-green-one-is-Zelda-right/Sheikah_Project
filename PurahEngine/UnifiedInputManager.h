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

		// 키가 방금 눌렸는가?
		bool GetKeyDown(eUnfInput keycode);
		// 키 입력이 
		bool GetKey(eUnfInput keycode);
		// 키를 눌리고 있는가?
		bool GetKeyPressed(eUnfInput keycode);
		// 키를 방금 뗐는가?
		bool GetKeyUp(eUnfInput keycode);
		// 키를 떼고 있는 상태인가?
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