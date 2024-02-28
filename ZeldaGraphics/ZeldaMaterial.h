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
	ZeldaMaterial(ID3D11Device* device, DirectX::XMFLOAT4 baseColor, const std::wstring& diffuseFilePath);
	ZeldaMaterial(const ZeldaMaterial& zeldaMaterial) = delete;

	DirectX::XMFLOAT4 baseColor;

	bool useDiffuseMap;
	ZeldaTexture* diffuseMap;

	friend class ZeldaModel;
};