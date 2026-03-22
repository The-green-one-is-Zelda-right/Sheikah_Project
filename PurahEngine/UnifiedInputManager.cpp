#include "UnifiedInputManager.h"

#include "GamePadManager.h"
#include "GamePad.h"
#include "InputManager.h"

namespace PurahEngine
{
	void UnifiedInputManager::Initialize()
	{
		{
			eUnfInput unfInput = eUnfInput::UI_Ok;
			std::vector<ePad> padvec;
			std::vector<eKey> keyvec;
			
			padvec.push_back(ePad::ePAD_B);
			keyvec.push_back(eKey::eKEY_SPACE);

			padMap[unfInput] = padvec;
			keyMap[unfInput] = keyvec;
			keyResultMap[unfInput] = KeyResult::Unknown;
		}

		{
			eUnfInput unfInput = eUnfInput::UI_Cancel;
			std::vector<ePad> padvec;
			std::vector<eKey> keyvec;

			padvec.push_back(ePad::ePAD_A);
			keyvec.push_back(eKey::eKEY_ESCAPE);

			padMap[unfInput] = padvec;
			keyMap[unfInput] = keyvec;
			keyResultMap[unfInput] = KeyResult::Unknown;
		}

		{
			eUnfInput unfInput = eUnfInput::UI_Left;
			std::vector<ePad> padvec;
			std::vector<eKey> keyvec;

			padvec.push_back(ePad::ePAD_LEFT);
			keyvec.push_back(eKey::eKEY_LEFT);

			padMap[unfInput] = padvec;
			keyMap[unfInput] = keyvec;
			keyResultMap[unfInput] = KeyResult::Unknown;
		}

		{
			eUnfInput unfInput = eUnfInput::UI_Right;
			std::vector<ePad> padvec;
			std::vector<eKey> keyvec;

			padvec.push_back(ePad::ePAD_RIGHT);
			keyvec.push_back(eKey::eKEY_RIGHT);

			padMap[unfInput] = padvec;
			keyMap[unfInput] = keyvec;
			keyResultMap[unfInput] = KeyResult::Unknown;
		}

		{
			eUnfInput unfInput = eUnfInput::UI_Up;
			std::vector<ePad> padvec;
			std::vector<eKey> keyvec;

			padvec.push_back(ePad::ePAD_UP);
			keyvec.push_back(eKey::eKEY_UP);

			padMap[unfInput] = padvec;
			keyMap[unfInput] = keyvec;
			keyResultMap[unfInput] = KeyResult::Unknown;
		}

		{
			eUnfInput unfInput = eUnfInput::UI_Down;
			std::vector<ePad> padvec;
			std::vector<eKey> keyvec;

			padvec.push_back(ePad::ePAD_DOWN);
			keyvec.push_back(eKey::eKEY_DOWN);

			padMap[unfInput] = padvec;
			keyMap[unfInput] = keyvec;
			keyResultMap[unfInput] = KeyResult::Unknown;
		}

		{
			eUnfInput unfInput = eUnfInput::UI_MENU;
			std::vector<ePad> padvec;
			std::vector<eKey> keyvec;

			padvec.push_back(ePad::ePAD_START);
			keyvec.push_back(eKey::eKEY_ESCAPE);

			padMap[unfInput] = padvec;
			keyMap[unfInput] = keyvec;
			keyResultMap[unfInput] = KeyResult::Unknown;
		}
	}

	void UnifiedInputManager::Update()
	{
		for (auto& [key, value] : keyResultMap)
		{
			value = KeyResult::Unknown;
		}
	}

	bool UnifiedInputManager::GetKeyDown(eUnfInput keycode)
	{
		GamePadManager& padManager = GamePadManager::Instance();
		InputManager& inputManager = InputManager::Getinstance();

		auto paditer = padMap.find(keycode);
		auto keyiter = keyMap.find(keycode);

		if (paditer == padMap.end() && keyiter == keyMap.end())
		{
			return false;
		}

		if (paditer != padMap.end())
		{
			auto& padvec = paditer->second;

			for (ePad epad : padvec)
			{
				for (int i = 0; i < 4; i++)
				{
					IGamePad* pad = padManager.GetGamePad(i);
					if (pad->IsConnected())
					{
						if (pad->IsKeyDown(epad))
						{
							return true;
						}
					}
				}
			}
		}

		if (keyiter != keyMap.end())
		{
			auto& keyvec = keyiter->second;

			for (eKey ekey : keyvec)
			{
				if (inputManager.IsKeyDown(ekey))
				{
					return true;
				}
			}
		}

		return false;
	}

	bool UnifiedInputManager::GetKey(eUnfInput keycode)
	{
		if (keyResultMap[keycode] != KeyResult::Unknown)
		{
			return keyResultMap[keycode] == KeyResult::Entered;
		}
		
		GamePadManager& padManager = GamePadManager::Instance();
		InputManager& inputManager = InputManager::Getinstance();

		auto paditer = padMap.find(keycode);
		auto keyiter = keyMap.find(keycode);

		if (paditer == padMap.end() && keyiter == keyMap.end())
		{
			keyResultMap[keycode] = KeyResult::NotEntered;
			return false;
		}

		if (paditer != padMap.end())
		{
			auto& padvec = paditer->second;

			for (ePad epad : padvec)
			{
				for (int i = 0; i < 4; i++)
				{
					IGamePad* pad = padManager.GetGamePad(i);
					if (pad->IsConnected())
					{
						if (pad->GetKey(epad))
						{
							keyResultMap[keycode] = KeyResult::Entered;
							return true;
						}
					}
				}
			}
		}

		if (keyiter != keyMap.end())
		{
			auto& keyvec = keyiter->second;

			for (eKey ekey : keyvec)
			{
				if (inputManager.GetKey(ekey))
				{
					keyResultMap[keycode] = KeyResult::Entered;
					return true;
				}
			}
		}

		keyResultMap[keycode] = KeyResult::NotEntered;
		return false;
	}

	bool UnifiedInputManager::GetKeyPressed(eUnfInput keycode)
	{
		GamePadManager& padManager = GamePadManager::Instance();
		InputManager& inputManager = InputManager::Getinstance();

		auto paditer = padMap.find(keycode);
		auto keyiter = keyMap.find(keycode);

		if (paditer == padMap.end() && keyiter == keyMap.end())
		{
			return false;
		}

		if (paditer != padMap.end())
		{
			auto& padvec = paditer->second;

			for (ePad epad : padvec)
			{
				for (int i = 0; i < 4; i++)
				{
					IGamePad* pad = padManager.GetGamePad(i);
					if (pad->IsConnected())
					{
						if (pad->IsKeyPressed(epad))
						{
							return true;
						}
					}
				}
			}
		}

		if (keyiter != keyMap.end())
		{
			auto& keyvec = keyiter->second;

			for (eKey ekey : keyvec)
			{
				if (inputManager.IsKeyPressed(ekey))
				{
					return true;
				}
			}
		}

		return false;
	}

	bool UnifiedInputManager::GetKeyUp(eUnfInput keycode)
	{
		GamePadManager& padManager = GamePadManager::Instance();
		InputManager& inputManager = InputManager::Getinstance();

		auto paditer = padMap.find(keycode);
		auto keyiter = keyMap.find(keycode);

		if (paditer == padMap.end() && keyiter == keyMap.end())
		{
			return false;
		}

		if (paditer != padMap.end())
		{
			auto& padvec = paditer->second;

			for (ePad epad : padvec)
			{
				for (int i = 0; i < 4; i++)
				{
					IGamePad* pad = padManager.GetGamePad(i);
					if (pad->IsConnected())
					{
						if (pad->IsKeyUp(epad))
						{
							return true;
						}
					}
				}
			}
		}

		if (keyiter != keyMap.end())
		{
			auto& keyvec = keyiter->second;

			for (eKey ekey : keyvec)
			{
				if (inputManager.IsKeyUp(ekey))
				{
					return true;
				}
			}
		}

		return false;
	}

	bool UnifiedInputManager::GetKeyReleased(eUnfInput keycode)
	{
		GamePadManager& padManager = GamePadManager::Instance();
		InputManager& inputManager = InputManager::Getinstance();

		auto paditer = padMap.find(keycode);
		auto keyiter = keyMap.find(keycode);

		if (paditer == padMap.end() && keyiter == keyMap.end())
		{
			return false;
		}

		if (paditer != padMap.end())
		{
			auto& padvec = paditer->second;

			for (ePad epad : padvec)
			{
				for (int i = 0; i < 4; i++)
				{
					IGamePad* pad = padManager.GetGamePad(i);
					if (pad->IsConnected())
					{
						if (pad->IsKeyReleased(epad))
						{
							return true;
						}
					}
				}
			}
		}

		if (keyiter != keyMap.end())
		{
			auto& keyvec = keyiter->second;

			for (eKey ekey : keyvec)
			{
				if (inputManager.IsKeyReleased(ekey))
				{
					return true;
				}
			}
		}

		return false;
	}

	UnifiedInputManager& UnifiedInputManager::Getinstance()
	{
		static UnifiedInputManager instance;
		return instance;
	}

	UnifiedInputManager::UnifiedInputManager()
	{

	}

	UnifiedInputManager::~UnifiedInputManager()
	{

	}
}