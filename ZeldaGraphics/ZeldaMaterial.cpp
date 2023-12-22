#include "ZeldaMaterial.h"

#include "ZeldaTexture.h"

ZeldaMaterial::ZeldaMaterial(ID3D11Device* device, DirectX::XMFLOAT4 baseColor, const std::wstring& diffuseFilePath) :
	baseColor(baseColor),
	useDiffuseMap(false),
	diffuseMap(nullptr)
{
	if (diffuseFilePath != L"")
	{
		diffuseMap = new ZeldaTexture(device, diffuseFilePath);
	}
	useDiffuseMap = (diffuseMap != nullptr);
}

ZeldaMaterial::~ZeldaMaterial()
{

}

void ZeldaMaterial::SetShaderResource()
{

}
