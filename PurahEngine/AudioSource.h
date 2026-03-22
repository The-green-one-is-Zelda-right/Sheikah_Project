#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"

#include "Transform.h"
#include "SoundManager.h"
#include "AudioClip.h"

namespace PurahEngine
{
	class Transform;
	class GameObject;
	class SoundManager;

	class PURAHENGINE_API AudioSource : public Component
	{
	public:
		struct Sound
		{
			std::string name;
			SoundType soundType;
			float minDistance;
			float maxDistance;
		};

	public:
		AudioSource();
		~AudioSource();

	public:
		void Awake();
		void Initialize();
		void OnDataLoadComplete();
		void Update();


		void PlayAudio(std::wstring name); // 재생할 파일 이름
		void PlayAudio(int i); // 재생할 파일 순서
		void PlayAudio(); // 기본적으로 첫번째 파일을 재생

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		Transform* soundTransform;
		std::vector<Sound> soundFile;
		std::map<std::wstring, AudioClip*> soundMap;

	};
}

// AudioSource는 AudioClip을 관리만 해야한다. 여기서 구조체에 음원을 등록하게되면
// 여러개의 음원을 관리하는 AudioSource가 일처리를 제대로 못할 것 같다.