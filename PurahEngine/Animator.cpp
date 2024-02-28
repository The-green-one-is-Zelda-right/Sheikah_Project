#include "Animator.h"

#include "GraphicsManager.h"
#include "GameObject.h"
#include "ModelRenderer.h"

namespace PurahEngine
{
	const float Animator::defaultPlaySpeed = 1.0f;
	const bool Animator::defaultLoop = false;
	const float Animator::defaultBlendTime = 1.0f;

	Animator::Animator() :
		targetRenderer(nullptr),
		animationBlend(false)
	{
		GraphicsManager::GetInstance().AddAnimator(this);
	}

	Animator::~Animator()
	{
		if (targetRenderer != nullptr)
		{
			targetRenderer->animator = nullptr;
		}

		GraphicsManager::GetInstance().RemoveAnimator(this);
	}

	void Animator::Initialize()
	{
		targetRenderer = nullptr;
		animationBlend = false;

		CheckModelRenderer();
	}

	void Animator::Play(const std::wstring& animationName)
	{
		if (targetRenderer == nullptr) return;

		unsigned int animationID = animationIDTable[animationName];

		Play(animationName, animationID);
	}

	void Animator::Play(unsigned int animationNumber)
	{
		if (targetRenderer == nullptr) return;

		std::wstring animationName = animationList[animationNumber];

		Play(animationName, animationNumber);
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
		if (modelRenderer != nullptr)
		{
			// 이미 다른 Animator, Renderer가 있다면 문제가 될 수 있음
			assert(modelRenderer->animator == nullptr);
			assert(targetRenderer == nullptr);

			modelRenderer->animator = this;
			targetRenderer = modelRenderer;
			ModelID modelID = modelRenderer->GetModelID(modelRenderer->modelName);

			// 이 애니메이션 데이터와 관련되어 있는 컨테이너 초기화
			animationList.clear();
			playTime.clear();
			animationIDTable.clear();
			blendTimeTable.clear();

			animationList = GraphicsManager::GetInstance().graphicsModule->GetAnimationListByModel(modelID);
			playTime = GraphicsManager::GetInstance().graphicsModule->GetAnimationPlayTime(modelID);

			for (unsigned int i = 0; i < animationList.size(); i++)
			{
				animationIDTable[animationList[i]] = i;
			}

			// 크기를 변경하면서 초기화되어있지 않은 값들은 기본값으로 초기화한다.
			playSpeed.resize(animationList.size(), defaultPlaySpeed);
			animationLoop.resize(animationList.size(), defaultLoop);

			// Blend Time 초기화
			for (unsigned int i = 0; i < animationList.size(); i++)
			{
				for (unsigned int j = 0; j < animationList.size(); j++)
				{
					blendTimeTable[{ i, j }] = defaultBlendTime;
				}
			}
		}
	}

	void Animator::UpdateAnimation(float deltaTime)
	{
		if (targetRenderer == nullptr)
		{
			return;
		}

		if (targetRenderer->animationName1 == L"" || targetRenderer->animationName2 == L"")
		{
			return;
		}

		// Blending Animation이 재생되고 있는 경우에는
		// 종료된 후에 다음 애니메이션을 재생한다.

		// 애니메이션의 재생이 끝나면 loop가 켜져있는 애니메이션의 경우에는 다시 반복해서 재생하며
		// 그렇지 않다면 마지막 상태에서 play명령이 다시 들어올 때까지 대기한다.
		bool& isBlending = targetRenderer->isBlending;
		std::wstring& animationName1 = targetRenderer->animationName1;
		std::wstring& animationName2 = targetRenderer->animationName2;
		float& time1 = targetRenderer->time1;
		float& time2 = targetRenderer->time2;
		float& ratio = targetRenderer->ratio;

		assert(animationIDTable.count(animationName1) > 0);
		assert(animationIDTable.count(animationName2) > 0);

		unsigned int animationID1 = animationIDTable[animationName1];
		unsigned int animationID2 = animationIDTable[animationName2];

		// Blending Animation을 재생하는 중
		if (isBlending)
		{
			// 현재 재생중인 Bleding Animation의 전체 재생시간
			float currentPlayTime = blendTimeTable[{ animationID1, animationID2 }];
			// 지금까지 재생된 시간
			float currentElapsedTime = ratio * currentPlayTime;

			currentElapsedTime += deltaTime;

			// Blending Animation의 재생시간이 종료된 경우
			// 이어서 다음 애니메이션을 재생한다.
			if (currentElapsedTime > currentPlayTime)
			{
				float overTime = currentPlayTime - currentElapsedTime;

				isBlending = false;
				animationName1 = animationName2;
				time2 = time2 + overTime;

				float secondPlayTime = playTime[animationID2];

				// 이미 시간이 이어서 재생할 애니메이션의 재생시간을 초과한 경우
				if (time2 > secondPlayTime)
				{
					if (animationLoop[animationID2])
					{
						while (time2 > secondPlayTime)
						{
							time2 -= secondPlayTime;
						}
					}
					else
					{
						time2 = secondPlayTime;
					}
				}

				time1 = time2;
			}
			// Blending Animation을 이어서 재생
			else
			{
				ratio = currentElapsedTime / currentPlayTime;
			}
		}
		// Blending Animation이 아님
		else
		{
			// 현재 재생중인 Bleding Animation의 전체 재생시간
			float currentPlayTime = playTime[animationID1];
			// 지금까지 재생된 시간
			float& currentElapsedTime = time1;
			// 현재 재생중인 애니메이션의 속도
			float currentPlaySpeed = playSpeed[animationID1];

			currentElapsedTime += currentPlaySpeed * deltaTime;

			// 재생시간이 종료된 경우
			if (currentElapsedTime > currentPlayTime)
			{
				if (animationLoop[animationID1])
				{
					while (currentElapsedTime > currentPlayTime)
					{
						currentElapsedTime -= currentPlayTime;
					}
				}
				else
				{
					currentElapsedTime = currentPlayTime;
				}
			}
			else
			{
				// 그 외의 경우 time1에 이미 deltaTime이 더해졌기 때문에 따로 처리할 필요가 없음
			}
		}
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
	
	void Animator::SetBlendTime(unsigned int firstAnimationNumber, unsigned int secondAnimationNumber, float blendTime)
	{
		blendTimeTable[{ firstAnimationNumber, secondAnimationNumber }] = blendTime;
	}

	void Animator::SetBlendTime(const std::wstring& firstAnimationName, const std::wstring& secondAnimationName, float blendTime)
	{
		assert(animationIDTable.count(firstAnimationName) > 0);
		assert(animationIDTable.count(secondAnimationName) > 0);

		unsigned int firstAnimationNumber = animationIDTable[firstAnimationName];
		unsigned int secondAnimationNumber = animationIDTable[secondAnimationName];

		blendTimeTable[{ firstAnimationNumber, secondAnimationNumber }] = blendTime;
	}

	void Animator::SetBlend(bool animationBlend)
	{
		this->animationBlend = animationBlend;
	}

	void Animator::Play(const std::wstring& targetAnimationName, unsigned int targetAnimationID)
	{
		bool& isBlending = targetRenderer->isBlending;
		std::wstring& animationName1 = targetRenderer->animationName1;
		std::wstring& animationName2 = targetRenderer->animationName2;
		float& time1 = targetRenderer->time1;
		float& time2 = targetRenderer->time2;
		float& ratio = targetRenderer->ratio;

		// blending이 있는 애니메이션
		// 초기상태에서 처음으로 Play명령을 하면 blending 하지 않음
		if (animationBlend && animationName2 != L"")
		{
			// 이미 blending 하는 중에는 마지막 상태로 즉시 변경
			if (isBlending)
			{
				isBlending = true;
				animationName1 = animationName2;
				animationName2 = targetAnimationName;
				time1 = time2;
				time2 = 0.0f;
				ratio = 0.0f;
			}
			else
			{
				isBlending = true;
				animationName2 = targetAnimationName;
				time2 = 0.0f;
				ratio = 0.0f;
			}
		}
		// blending이 없는 애니메이션은 즉시 변경
		else
		{
			isBlending = false;
			animationName1 = targetAnimationName;
			animationName2 = targetAnimationName;
			time1 = 0.0f;
		}
	}

	void Animator::PreSerialize(json& jsonData) const
	{

	}

	void Animator::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}

	void Animator::PostSerialize(json& jsonData) const
	{

	}

	void Animator::PostDeserialize(const json& jsonData)
	{

	}
}