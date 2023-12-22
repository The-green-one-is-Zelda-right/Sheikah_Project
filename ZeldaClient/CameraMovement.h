#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class GameObject;

	class CameraMovement : public PurahEngine::Component
	{
	public:
		CameraMovement();
		~CameraMovement();

		void Update();
	};
}

