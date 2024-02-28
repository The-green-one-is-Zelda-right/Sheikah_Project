#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class Test3 : public Component
	{
	public:
		Test3();
		~Test3();

		void Awake();
		void Start();
		void Update();

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}

