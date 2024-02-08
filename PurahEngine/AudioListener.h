#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class GameObject;

	class AudioListener : public Component
	{
	public:
		AudioListener();
		~AudioListener();

	private:
		Transform* listenerTransform;
	};
}

