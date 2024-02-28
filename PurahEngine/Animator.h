#pragma once

#include "Component.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace std {
	template <>
	struct hash<std::pair<unsigned int, unsigned int>> {
		std::size_t operator()(const std::pair<unsigned int, unsigned int>& key) const {
			hash<unsigned int> uintHash;
			return uintHash(key.first) ^ uintHash(key.second);
		}
	};
}

namespace PurahEngine
{
	class ModelRenderer;
	class GraphicsManager;

	class PURAHENGINE_API Animator : public Component
	{
	public:
		Animator();
		~Animator();

		void Initialize() override;

		void Play(const std::wstring& animationName);
		void Play(unsigned int animationID);

		void SetPlaySpeed(unsigned int animationNumber, float speed);
		void SetPlaySpeed(const std::wstring& animationName, float speed);

		void SetLoop(unsigned int animationNumber, bool loop);
		void SetLoop(const std::wstring& animationName, bool loop);

		void SetBlendTime(unsigned int firstAnimationNumber, unsigned int secondAnimationNumber, float blendTime);
		void SetBlendTime(const std::wstring& firstAnimationName, const std::wstring& secondAnimationName, float blendTime);

		void SetBlend(bool animationBlend);

	private:
		void Play(const std::wstring& animationName, unsigned int animationID);

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

		ModelRenderer* targetRenderer;

		std::vector<float> playTime;
		std::vector<std::wstring> animationList;
		std::unordered_map<std::wstring, unsigned int> animationIDTable;
		std::unordered_map<std::pair<unsigned int, unsigned int>, float> blendTimeTable;

		std::vector<float> playSpeed;
		std::vector<bool> animationLoop;
		bool animationBlend;

		void CheckModelRenderer();
		void Initialize_Animator(ModelRenderer* modelRenderer);
		void UpdateAnimation(float deltaTime);

		const static float defaultPlaySpeed;
		const static bool defaultLoop;
		const static float defaultBlendTime;

		friend ModelRenderer;
		friend GraphicsManager;
	};
}