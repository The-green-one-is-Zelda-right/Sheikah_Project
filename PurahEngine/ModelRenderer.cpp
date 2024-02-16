#include "ModelRenderer.h"

#include "GraphicsManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"

namespace PurahEngine
{
	void ModelRenderer::Initialize()
	{
		modelName = L"";
		activeAnimationBlend = false;
		animationName = L"";
		nextAnimationName = L"";

		Animator* animator = GetGameObject()->GetComponent<Animator>();
		if (animator != nullptr)
		{
			animator->Initialize_Animator(this);
		}
	}

	//void ModelRenderer::OnDestroy()
	//{
	//	Animator* animator = GetGameObject()->GetComponent<Animator>();
	//	if (animator != nullptr)
	//	{
	//		animator->CheckModelRenderer();
	//	}
	//}

	void ModelRenderer::Render(IZeldaRenderer* renderer)
	{
		// ���ӿ�����Ʈ�� Ȱ��ȭ �Ǿ� �ִ� ��쿡�� �۵��Ѵ�.
		if (GetGameObject()->IsRootEnable())
		{
			Eigen::Matrix4f worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
			ModelID modelID = GetModelID(modelName);
			renderer->DrawModel(worldTM, modelID, false);
		}
	}

	void ModelRenderer::SetModelName(const std::wstring& modelName)
	{
		this->modelName = modelName;
	}

	void ModelRenderer::RemoveAnimator()
	{
		animationName = L"";
		nextAnimationName = L"";
	}
}