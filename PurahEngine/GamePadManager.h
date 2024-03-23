#pragma once
#include "PurahEngineAPI.h"

#include "ePad.h"
#include "GamePad.h"
#include <Windows.h>

namespace PurahEngine
{
	class GamePad;

	class PURAHENGINE_API GamePadManager
	{
	private:
		GamePadManager();
		~GamePadManager();
		GamePadManager& operator=(const GamePadManager&) = delete;
		GamePadManager&& operator=(GamePadManager&&) noexcept = delete;

	public:
		void Initialize(HWND _hwnd, ePad* _keyArr, int _size);
		void Update();

	public:
		static void AddGamePad(int _id);
		static void RemoveGamePad(int _id);
		static GamePad* GetGamePad(int _id);

	private:
		HWND hWnd;
		GamePad pad[4];

	public:
		static GamePadManager& Instance();
		static void Release();
	};
}

