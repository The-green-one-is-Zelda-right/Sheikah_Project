#include "ZeldaMaterial.h"

#include "ZeldaTexture.h"

#include "ZeldaGraphicsDefine.h"

ZeldaMaterial::ZeldaMaterial(ID3D11Device* device, DirectX::XMFLOAT4 baseColor, const std::wstring& diffuseFilePath, const std::wstring& normalFilePath) :
	baseColor(baseColor),
	useDiffuseMap(false),
	diffuseMap(nullptr),
	useNormalMap(false),
	normalMap(nullptr)
{
	if (diffuseFilePath != L"")
	{
		diffuseMap = new ZeldaTexture(device, diffuseFilePath);
	}
	useDiffuseMap = (diffuseMap != nullptr);

	if (normalFilePath != L"")
	{
		normalMap = new ZeldaTexture(device, normalFilePath);
	}
	useNormalMap = (normalMap != nullptr);
}

ZeldaMaterial::~ZeldaMaterial()
{
	if (diffuseMap != nullptr)
	{
		delete diffuseMap;
		diffuseMap = nullptr;
	}

	if (normalMap != nullptr)
	{
		delete normalMap;
		normalMap = nullptr;
	}
}

void ZeldaMaterial::SetShaderResource(ID3D11DeviceContext* deviceContext)
{
	if (useDiffuseMap && diffuseMap)
	{
		diffuseMap->SetDiffuseMapShaderResource(deviceContext);
	}

	if (useNormalMap && normalMap)
	{
		normalMap->SetNormalMapShaderResource(deviceContext);
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
