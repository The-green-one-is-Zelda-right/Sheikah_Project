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

	void ModelRenderer::OnDataLoadComplete()
	{
		animator = nullptr;

		isBlending = false;

		modelName = L"";
		animationName1 = L"";
		animationName2 = L"";
		time1 = 0.0f;
		time2 = 0.0f;
		ratio = 0.0f;
	}

	void ModelRenderer::Render(IZeldaRenderer* renderer)
	{
		// ���ӿ�����Ʈ�� Ȱ��ȭ �Ǿ� �ִ� ��쿡�� �۵��Ѵ�.
		if (GetGameObject()->IsRootEnable())
		{
			Eigen::Matrix4f worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
			ModelID modelID = GetModelID(modelName);

			// Animator�� ���� ��� DrawModel�Լ��� �̿��Ѵ�.
			if (animator == nullptr)
			{
				renderer->DrawModel(worldTM, modelID, false);
			}
			// Animator�� �ִ� ��� DrawAnimation, DrawChangingAnimation �Լ��� �̿��Ѵ�.
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
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_WSTRING(modelName);
	}

	void ModelRenderer::PostSerialize(json& jsonData) const
	{

	}

	void ModelRenderer::PostDeserialize(const json& jsonData)
	{

	}

}