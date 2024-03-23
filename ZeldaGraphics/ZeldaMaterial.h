#pragma once

#include <d3d11.h>
#include <string>

#include "DirectXTex.h"

class ZeldaTexture;

class ZeldaMaterial
{
public:
	enum MAP
	{
		DIFFUSE_MAP
	};
	
	~ZeldaMaterial();

	void SetShaderResource(ID3D11DeviceContext* deviceContext);
	
	bool UseSRGB(MAP map);

private:
	ZeldaMaterial(ID3D11Device* device, DirectX::XMFLOAT4 baseColor, const std::wstring& diffuseFilePath, const std::wstring& normalFilePath);
	ZeldaMaterial(const ZeldaMaterial& zeldaMaterial) = delete;

	DirectX::XMFLOAT4 baseColor;

	bool useDiffuseMap;
	ZeldaTexture* diffuseMap;

	bool useNormalMap;
	ZeldaTexture* normalMap;

	friend class ZeldaModel;
};