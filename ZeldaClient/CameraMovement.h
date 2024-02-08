#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class GameObject;

	class CameraMovement : public Component
	{
	public:
		CameraMovement();
		~CameraMovement();

		void Update();
	};
}

