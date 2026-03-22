#pragma once
#include "PurahEngine.h"

namespace Phyzzle
{
	class DebugCameraSetting : public PurahEngine::Component
	{
	public:
		DebugCameraSetting() = default;
		~DebugCameraSetting() override;

		void Awake() override;
		void Update() override;
		void LateUpdate() override;
		void OnDestroy() override;


	public:
		void PreSerialize(json& jsonData) const override {};
		void PreDeserialize(const json& jsonData) override {};
		void PostSerialize(json& jsonData) const override {};
		void PostDeserialize(const json& jsonData) override {};

	private:
		static std::vector<PurahEngine::Camera*> cameraList;
		static int currIndex;
		bool check = false;
	};
}

