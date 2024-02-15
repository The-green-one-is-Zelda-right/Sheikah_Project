#include "ModelRenderer.h"

#include "GraphicsManager.h"
#include "GameObject.h"
#include "Transform.h"

namespace PurahEngine
{
	void ModelRenderer::Render(IZeldaRenderer* renderer)
	{
		// 게임오브젝트가 활성화 되어 있는 경우에만 작동한다.
		if (GetGameObject()->IsEnable())
		{
			Eigen::Matrix4f worldTM = transform->GetWorldMatrix();
			ModelID modelID = GetModelID(modelName);
			renderer->DrawModel(worldTM, modelID, false);
		}
	}

	void ModelRenderer::SetModelName(const std::wstring& modelName)
	{
		this->modelName = modelName;
	}
}