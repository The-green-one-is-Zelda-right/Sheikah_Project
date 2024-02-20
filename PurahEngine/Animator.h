#pragma once

#include "Component.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace PurahEngine
{
	class ModelRenderer;

	class Animator : public Component
	{
	public:
		void Initialize() override;

		void OnDestroy();

		void Play(const std::wstring& animationName);
		void Play(unsigned int animationID);
		void PlayBlendingAnimation(const std::wstring& animationName, float blendTime);
		void PlayBlendingAnimation(unsigned int animationID, float blendTime);


		void SetPlaySpeed(unsigned int animationNumber, float speed);
		void SetPlaySpeed(const std::wstring& animationName, float speed);

		void SetLoop(unsigned int animationNumber, bool loop);
		void SetLoop(const std::wstring& animationName, bool loop);

	private:
		ModelRenderer* targetRenderer;

		std::vector<float> playTime;
		std::vector<std::wstring> animationList;
		std::unordered_map<std::wstring, unsigned int> animationIDTable;

		std::vector<float> playSpeed;
		std::vector<bool> animationLoop;
		bool animationBlend;

		void CheckModelRenderer();
		void Initialize_Animator(ModelRenderer* modelRenderer);

		const static float defaultPlaySpeed;
		const static bool defaultLoop;
		const static float defaultBlendTime;

		friend ModelRenderer;
	};
}