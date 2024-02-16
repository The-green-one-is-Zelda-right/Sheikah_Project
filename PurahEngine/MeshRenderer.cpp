#include "MeshRenderer.h"

#include "GameObject.h"
#include "Transform.h"

namespace PurahEngine
{
	MeshRenderer::MeshRenderer() :
		meshType(MeshType::None),
		textureName(L""),
		meshColor(White)
	{

	}

	void MeshRenderer::Render(IZeldaRenderer* renderer)
	{
		// ���ӿ�����Ʈ�� Ȱ��ȭ �Ǿ� �ִ� ��쿡�� �۵��Ѵ�.
		if (GetGameObject()->IsRootEnable())
		{
			Eigen::Matrix4f worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
			TextureID textureID = TextureID::ID_NULL;
			if (textureName != L"")
			{
				textureID = GetTextureID(textureName);
			}
			
			switch (meshType)
			{
				case PurahEngine::MeshRenderer::MeshType::None:
				{
					// Please Select MeshType
					assert(0);
					break;
				}
				case PurahEngine::MeshRenderer::MeshType::Cube:
				{
					renderer->DrawCube(worldTM, textureID, false, meshColor.r, meshColor.g, meshColor.b, meshColor.a);

					break;
				}
				default:
				{
					assert(0);
					break;
				}
			}
		}
	}
	void MeshRenderer::SetTexture(const std::wstring& textureName)
	{
		this->textureName = textureName;
	}

	void MeshRenderer::SetMesh(MeshType type)
	{
		meshType = type;
	}
}