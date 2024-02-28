#include "ModelRenderer.h"

#include "GraphicsManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"

namespace PurahEngine
{
	ModelRenderer::~ModelRenderer()
	{
		if (animator != nullptr)
		{
			animator->targetRenderer = nullptr;
		}
	}

	void ModelRenderer::Initialize()
	{
		animator = nullptr;

		isBlending = false;

		modelName = L"";
		animationName1 = L"";
		animationName2 = L"";
		time1 = 0.0f;
		time2 = 0.0f;
		ratio = 0.0f;

		Animator* animator = GetGameObject()->GetComponent<Animator>();
		if (animator != nullptr)
		{
			animator->Initialize_Animator(this);
		}
	}

	void ModelRenderer::Render(IZeldaRenderer* renderer)
	{
		// 게임오브젝트가 활성화 되어 있는 경우에만 작동한다.
		if (GetGameObject()->IsRootEnable())
		{
			Eigen::Matrix4f worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
			ModelID modelID = GetModelID(modelName);

			// Animator가 없는 경우 DrawModel함수를 이용한다.
			if (animator == nullptr)
			{
				renderer->DrawModel(worldTM, modelID, false);
			}
			// Animator가 있는 경우 DrawAnimation, DrawChangingAnimation 함수를 이용한다.
			else
			{
				if (isBlending)
				{
					renderer->DrawChangingAnimation(worldTM, modelID, animationName1, animationName2, time1, time2, ratio, false);
				}
				else
				{
					renderer->DrawAnimation(worldTM, modelID, animationName1, time1, false);
				}
			}
		}
	}

	void ModelRenderer::SetModelName(const std::wstring& modelName)
	{
		this->modelName = modelName;
	}

	void ModelRenderer::PreSerialize(json& jsonData) const
	{

	}

	void ModelRenderer::PreDeserialize(const json& jsonData)
	{

	}

	void ModelRenderer::PostSerialize(json& jsonData) const
	{

	}

	void ModelRenderer::PostDeserialize(const json& jsonData)
	{

	}

}