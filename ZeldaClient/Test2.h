#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class Test2 : public Component
	{
	public:
		Test2();
		~Test2();

		void Awake();
		void Start();
		void Update();
		void FixedUpdate();

	};
}

