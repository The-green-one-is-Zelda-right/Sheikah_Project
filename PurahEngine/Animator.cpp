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

	void Animator::PreSerialize(json& jsonData) const
	{

	}

	void Animator::PreDeserialize(const json& jsonData)
	{

	}

	void Animator::PostSerialize(json& jsonData) const
	{

	}

	void Animator::PostDeserialize(const json& jsonData)
	{

	}

	void Animator::CheckModelRenderer()
	{
		ModelRenderer* modelRenderer = GetGameObject()->GetComponent<ModelRenderer>();
		Initialize_Animator(modelRenderer);
	}

	// ������ ������ ������ clear�� ������ �ʰ�
	// �ִ��� �����ϸ鼭 �ʱ�ȭ�Ѵ�.
	void Animator::Initialize_Animator(ModelRenderer* modelRenderer)
	{
		if (modelRenderer != nullptr)
		{
			// �̹� �ٸ� Animator, Renderer�� �ִٸ� ������ �� �� ����
			assert(modelRenderer->animator == nullptr);
			assert(targetRenderer == nullptr);

			modelRenderer->animator = this;
			targetRenderer = modelRenderer;
			ModelID modelID = modelRenderer->GetModelID(modelRenderer->modelName);

			// �� �ִϸ��̼� �����Ϳ� ���õǾ� �ִ� �����̳� �ʱ�ȭ
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

			// ũ�⸦ �����ϸ鼭 �ʱ�ȭ�Ǿ����� ���� ������ �⺻������ �ʱ�ȭ�Ѵ�.
			playSpeed.resize(animationList.size(), defaultPlaySpeed);
			animationLoop.resize(animationList.size(), defaultLoop);

			// Blend Time �ʱ�ȭ
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

		// Blending Animation�� ����ǰ� �ִ� ��쿡��
		// ����� �Ŀ� ���� �ִϸ��̼��� ����Ѵ�.

		// �ִϸ��̼��� ����� ������ loop�� �����ִ� �ִϸ��̼��� ��쿡�� �ٽ� �ݺ��ؼ� ����ϸ�
		// �׷��� �ʴٸ� ������ ���¿��� play����� �ٽ� ���� ������ ����Ѵ�.
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

		// Blending Animation�� ����ϴ� ��
		if (isBlending)
		{
			// ���� ������� Bleding Animation�� ��ü ����ð�
			float currentPlayTime = blendTimeTable[{ animationID1, animationID2 }];
			// ���ݱ��� ����� �ð�
			float currentElapsedTime = ratio * currentPlayTime;

			currentElapsedTime += deltaTime;

			// Blending Animation�� ����ð��� ����� ���
			// �̾ ���� �ִϸ��̼��� ����Ѵ�.
			if (currentElapsedTime > currentPlayTime)
			{
				float overTime = currentPlayTime - currentElapsedTime;

				isBlending = false;
				animationName1 = animationName2;
				time2 = time2 + overTime;

				float secondPlayTime = playTime[animationID2];

				// �̹� �ð��� �̾ ����� �ִϸ��̼��� ����ð��� �ʰ��� ���
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
			// Blending Animation�� �̾ ���
			else
			{
				ratio = currentElapsedTime / currentPlayTime;
			}
		}
		// Blending Animation�� �ƴ�
		else
		{
			// ���� ������� Bleding Animation�� ��ü ����ð�
			float currentPlayTime = playTime[animationID1];
			// ���ݱ��� ����� �ð�
			float& currentElapsedTime = time1;
			// ���� ������� �ִϸ��̼��� �ӵ�
			float currentPlaySpeed = playSpeed[animationID1];

			currentElapsedTime += currentPlaySpeed * deltaTime;

			// ����ð��� ����� ���
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
				// �� ���� ��� time1�� �̹� deltaTime�� �������� ������ ���� ó���� �ʿ䰡 ����
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

		// blending�� �ִ� �ִϸ��̼�
		// �ʱ���¿��� ó������ Play����� �ϸ� blending ���� ����
		if (animationBlend && animationName2 != L"")
		{
			// �̹� blending �ϴ� �߿��� ������ ���·� ��� ����
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
		// blending�� ���� �ִϸ��̼��� ��� ����
		else
		{
			isBlending = false;
			animationName1 = targetAnimationName;
			animationName2 = targetAnimationName;
			time1 = 0.0f;
		}
	}
}