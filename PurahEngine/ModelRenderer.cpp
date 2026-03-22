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
		// 게임오브젝트가 활성화 되어 있는 경우에만 작동한다.
		if (GetGameObject()->IsRootEnable())
		{
			Eigen::Matrix4f worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
			ModelID modelID = GetModelID(modelName);

			// Animator가 없는 경우 DrawModel함수를 이용한다.
			if (animator == nullptr)
			{
				renderer->DrawModel(worldTM, modelID, wireFrame, dShadow, shadow, fastOutLine, outLine, { outLineR, outLineG, outLineB, outLineA });
			}
			// Animator가 있는 경우 DrawAnimation, DrawChangingAnimation 함수를 이용한다.
			else
			{
				if (isBlending)
				{
					renderer->DrawChangingAnimation(worldTM, modelID, animationName1, animationName2, time1, time2, ratio, wireFrame, dShadow, shadow, fastOutLine, outLine, { outLineR, outLineG, outLineB, outLineA });
				}
				else
				{
					renderer->DrawAnimation(worldTM, modelID, animationName1, time1, wireFrame, dShadow, shadow, fastOutLine, outLine, { outLineR, outLineG, outLineB, outLineA });
				}
			}
		}
	}

	void ModelRenderer::SetModelName(const std::wstring& modelName)
	{
		this->modelName = modelName;
	}

	void ModelRenderer::SetDShadow(bool value)
	{
		dShadow = value;
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

	void ModelRenderer::SetOutLineColor(const Eigen::Vector4f& _color)
	{
		outLineR = _color.x();
		outLineG = _color.y();
		outLineB = _color.z();
		outLineA = _color.w();
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
		PREDESERIALIZE_VALUE(dShadow);
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