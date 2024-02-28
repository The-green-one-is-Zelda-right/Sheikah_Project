#include "MeshRenderer.h"

#include "GameObject.h"
#include "Transform.h"

namespace PurahEngine
{
	MeshRenderer::MeshRenderer() :
		textureName(L""),
		meshType(MeshType::None),
		wireFrame(false), r(1.0f), g(1.0f), b(1.0f), a(1.0f)
	{

	}

	void MeshRenderer::Render(IZeldaRenderer* renderer)
	{
		// 게임오브젝트가 활성화 되어 있는 경우에만 작동한다.
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
					renderer->DrawCube(worldTM, textureID, wireFrame, r, g, b, a);

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

	void MeshRenderer::SetWireFrame(bool value)
	{
		wireFrame = value;
	}

	void MeshRenderer::SetColor(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	void MeshRenderer::PreSerialize(json& jsonData) const
	{

	}

	void MeshRenderer::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_WSTRING(textureName);
		PREDESERIALIZE_VALUE(meshType);
		PREDESERIALIZE_VALUE(wireFrame);
		PREDESERIALIZE_VALUE(r);
		PREDESERIALIZE_VALUE(g);
		PREDESERIALIZE_VALUE(b);
		PREDESERIALIZE_VALUE(a);
	}

	void MeshRenderer::PostSerialize(json& jsonData) const
	{

	}

	void MeshRenderer::PostDeserialize(const json& jsonData)
	{

	}

}