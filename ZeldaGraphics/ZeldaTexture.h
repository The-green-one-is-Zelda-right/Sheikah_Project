#pragma once

#include <d3d11.h>
#include <string>

#include "DirectXTex.h"

class ZeldaTexture
{
public:
	~ZeldaTexture();

	ID3D11ShaderResourceView* GetTexture();
	bool UseSRGB();

	void SetShaderResource(ID3D11DeviceContext* deviceContext);
	void SetCubeMapShaderResource(ID3D11DeviceContext* deviceContext);

private:
	ZeldaTexture(ID3D11Device* device, const std::wstring& filePath);
	ZeldaTexture(const ZeldaTexture& zeldaTexture) = delete;

	ID3D11ShaderResourceView* textureView;

	bool useSRGB;

	friend class ZeldaMaterial;
	friend class ResourceManager;
};

