#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class GameObject;

	class TestMovement : public PurahEngine::Component
	{
	public:
		TestMovement();
		~TestMovement();

		void Update();
	};
}