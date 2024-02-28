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
	if (diffuseMap != nullptr)
	{
		delete diffuseMap;
		diffuseMap = nullptr;
	}
}

void ZeldaMaterial::SetShaderResource(ID3D11DeviceContext* deviceContext)
{
	if (useDiffuseMap && diffuseMap)
	{
		diffuseMap->SetShaderResource(deviceContext);
	}
}

bool ZeldaMaterial::UseSRGB(MAP map)
{
	switch (map)
	{
		case ZeldaMaterial::DIFFUSE_MAP:
		{
			return (diffuseMap == nullptr) ? (false) : (diffuseMap->UseSRGB());
		}
		default:
		{
			assert(0);
			break;
		}
	}
}
