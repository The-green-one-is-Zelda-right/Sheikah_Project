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
				renderer->DrawModel(worldTM, modelID, wireFrame, shadow, fastOutLine, outLine, { outLineR, outLineG, outLineB, outLineA });
			}
			// Animator�� �ִ� ��� DrawAnimation, DrawChangingAnimation �Լ��� �̿��Ѵ�.
			else
			{
				if (isBlending)
				{
					renderer->DrawChangingAnimation(worldTM, modelID, animationName1, animationName2, time1, time2, ratio, wireFrame, shadow, fastOutLine, outLine, { outLineR, outLineG, outLineB, outLineA });
				}
				else
				{
					renderer->DrawAnimation(worldTM, modelID, animationName1, time1, wireFrame, shadow, fastOutLine, outLine, { outLineR, outLineG, outLineB, outLineA });
				}
			}
		}
	}

	void ModelRenderer::SetModelName(const std::wstring& modelName)
	{
		this->modelName = modelName;
	}

	void ModelRenderer::SetShadow(bool value)
	{
		shadow = value;
	}

	void ModelRenderer::SetFastOutLine(bool value)
	{
		fastOutLine = value;
	}

	bool ModelRenderer::GetFastOutLine() const
	{
		return fastOutLine;
	}

	void ModelRenderer::SetOutLine(bool value)
	{
		outLine = value;
	}

	bool ModelRenderer::GetOutLine() const
	{
		return outLine;
	}

	void ModelRenderer::PreSerialize(json& jsonData) const
	{

	}

	void ModelRenderer::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_WSTRING(modelName);
		PREDESERIALIZE_VALUE(shadow);
		PREDESERIALIZE_VALUE(wireFrame);
		PREDESERIALIZE_VALUE(fastOutLine);
		PREDESERIALIZE_VALUE(outLine);
		PREDESERIALIZE_VALUE(outLineR);
		PREDESERIALIZE_VALUE(outLineG);
		PREDESERIALIZE_VALUE(outLineB);
		PREDESERIALIZE_VALUE(outLineA);
	}

	void ModelRenderer::PostSerialize(json& jsonData) const
	{

	}

	void ModelRenderer::PostDeserialize(const json& jsonData)
	{

	}

}