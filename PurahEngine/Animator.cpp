#include "Animator.h"

#include "GraphicsManager.h"
#include "GameObject.h"
#include "ModelRenderer.h"

namespace PurahEngine
{
	const float Animator::defaultPlaySpeed = 1.0f;
	const bool Animator::defaultLoop = false;
	const float Animator::defaultBlendTime = 1.0f;

	void Animator::Initialize()
	{
		targetRenderer = nullptr;
		animationBlend = false;

		CheckModelRenderer();
	}

	void Animator::OnDestroy()
	{
		if (targetRenderer != nullptr)
		{
			targetRenderer->RemoveAnimator();
		}
	}

	void Animator::Play(const std::wstring& animationName)
	{
		if (targetRenderer == nullptr) return;

		unsigned int animationID = animationIDTable[animationName];

		if (animationBlend)
		{
			targetRenderer->loop = animationLoop[animationID];
			targetRenderer->activeAnimationBlend = true;
			targetRenderer->animationName = targetRenderer->nextAnimationName;
			targetRenderer->nextAnimationName = animationName;
			targetRenderer->playElapsed = 0.0f;
			targetRenderer->playSpeed = playSpeed[animationID];
			targetRenderer->animationTime = playTime[animationID];
			targetRenderer->blendTime = defaultBlendTime;
		}
		else
		{
			targetRenderer->loop = animationLoop[animationID];
			targetRenderer->activeAnimationBlend = false;
			targetRenderer->animationName = animationName;
			targetRenderer->nextAnimationName = animationName;
			targetRenderer->playElapsed = 0.0f;
			targetRenderer->playSpeed = playSpeed[animationID];
			targetRenderer->animationTime = playTime[animationID];
		}
	}

	void Animator::Play(unsigned int animationNumber)
	{
		if (targetRenderer == nullptr) return;

		std::wstring animationName = animationList[animationNumber];

		targetRenderer->activeAnimationBlend = false;
		targetRenderer->animationName = animationName;
		targetRenderer->nextAnimationName = animationName;
		targetRenderer->playElapsed = 0.0f;
		targetRenderer->playSpeed = playSpeed[animationNumber];
		targetRenderer->animationTime = playTime[animationNumber];
		targetRenderer->loop = animationLoop[animationNumber];
	}

	void Animator::PlayBlendingAnimation(const std::wstring& animationName, float blendTime)
	{
		if (targetRenderer == nullptr) return;

		unsigned int animationID = animationIDTable[animationName];

		targetRenderer->activeAnimationBlend = true;
		targetRenderer->animationName = targetRenderer->nextAnimationName;
		targetRenderer->nextAnimationName = animationName;
		targetRenderer->playElapsed = 0.0f;
		targetRenderer->playSpeed = playSpeed[animationID];
		targetRenderer->animationTime = playTime[animationID];
		targetRenderer->loop = animationLoop[animationID];
	}

	void Animator::PlayBlendingAnimation(unsigned int animationNumber, float blendTime)
	{
		if (targetRenderer == nullptr) return;

		std::wstring animationName = animationList[animationNumber];

		targetRenderer->activeAnimationBlend = true;
		targetRenderer->animationName = targetRenderer->nextAnimationName;
		targetRenderer->nextAnimationName = animationName;
		targetRenderer->playElapsed = 0.0f;
		targetRenderer->playSpeed = playSpeed[animationNumber];
		targetRenderer->animationTime = playTime[animationNumber];
		targetRenderer->loop = animationLoop[animationNumber];
	}

	void Animator::CheckModelRenderer()
	{
		ModelRenderer* modelRenderer = GetGameObject()->GetComponent<ModelRenderer>();
		Initialize_Animator(modelRenderer);
	}

	// 기존에 설정한 값들을 clear로 없애지 않고
	// 최대한 보존하면서 초기화한다.
	void Animator::Initialize_Animator(ModelRenderer* modelRenderer)
	{
		if (modelRenderer == nullptr)
		{
			targetRenderer = nullptr;
			return;
		}

		targetRenderer = modelRenderer;
		ModelID modelID = modelRenderer->GetModelID(modelRenderer->modelName);

		// 이 세가지는 애니메이션 데이터와 관련되어 있기 때문에 반드시 clear 후에 초기화 해야한다.
		animationList.clear();
		playTime.clear();
		animationIDTable.clear();

		animationList = GraphicsManager::GetInstance().graphicsModule->GetAnimationListByModel(modelID);
		playTime = GraphicsManager::GetInstance().graphicsModule->GetAnimationPlayTime(modelID);

		for (unsigned int i = 0; i < animationList.size(); i++)
		{
			animationIDTable[animationList[i]] = i;
		}

		// 크기를 변경하면서 초기화되어있지 않은 값들은 기본값으로 초기화한다.
		playSpeed.resize(animationList.size(), defaultPlaySpeed);
		animationLoop.resize(animationList.size(), defaultLoop);
	}

	void Animator::SetPlaySpeed(unsigned int animationNumber, float speed)
	{
		if (playSpeed.size() <= animationNumber)
		{
			playSpeed.resize(animationNumber + 1, defaultPlaySpeed);
		}

		playSpeed[animationNumber] = speed;
	}

	void Animator::SetPlaySpeed(const std::wstring& animationName, float speed)
	{
		assert(animationIDTable.count(animationName) != 0);

		auto iter = animationIDTable.find(animationName);
		if (iter != animationIDTable.end())
		{
			unsigned int animationNumber = iter->second;

			SetPlaySpeed(animationNumber, speed);
		}
	}

	void Animator::SetLoop(unsigned int animationNumber, bool loop)
	{
		if (animationLoop.size() <= animationNumber)
		{
			animationLoop.resize(animationNumber + 1, defaultLoop);
		}

		animationLoop[animationNumber] = loop;
	}

	void Animator::SetLoop(const std::wstring& animationName, bool loop)
	{
		assert(animationIDTable.count(animationName) != 0);

		auto iter = animationIDTable.find(animationName);
		if (iter != animationIDTable.end())
		{
			unsigned int animationNumber = iter->second;

			SetLoop(animationNumber, loop);
		}
	}
}