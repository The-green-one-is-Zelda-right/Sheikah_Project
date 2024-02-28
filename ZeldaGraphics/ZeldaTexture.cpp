#include "ZeldaTexture.h"

#include "ZeldaGraphicsDefine.h"

using namespace DirectX;

ZeldaTexture::ZeldaTexture(ID3D11Device* device, const std::wstring& filePath) :
	textureView(nullptr),
	useSRGB(false)
{
	HRESULT result = S_FALSE;

	ScratchImage image;

	std::wstring fileType = filePath.substr(filePath.size() - 4, 4);

	TexMetadata metaData;

	if (fileType == L".tga")
	{
		result = LoadFromTGAFile(filePath.c_str(), &metaData, image);
	}
	else if (fileType == L".dds")
	{
		result = LoadFromDDSFile(filePath.c_str(), DDS_FLAGS_NONE, &metaData, image);
	}
	else if (fileType == L".jpg" || fileType == L".png" || fileType == L".tif" || fileType == L".tiff")
	{
		result = LoadFromWICFile(filePath.c_str(), WIC_FLAGS_NONE, &metaData, image);
	}
	else
	{
		assert(0);
	}

	if (metaData.format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB ||
		metaData.format == DXGI_FORMAT_B8G8R8X8_UNORM_SRGB ||
		metaData.format == DXGI_FORMAT_BC1_UNORM_SRGB ||
		metaData.format == DXGI_FORMAT_BC2_UNORM_SRGB ||
		metaData.format == DXGI_FORMAT_BC3_UNORM_SRGB ||
		metaData.format == DXGI_FORMAT_BC7_UNORM_SRGB ||
		metaData.format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
	{
		useSRGB = true;
	}

	HRESULT result2 = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &textureView);

	if (FAILED(result) || FAILED(result2))
	{
		MessageBox(0, (L"Failed to create ZeldaTexture\n" + filePath).c_str(), L"ZeldaTexture Error", MB_OK);
	}
}

ZeldaTexture::~ZeldaTexture()
{
	if (textureView)
	{
		textureView->Release();
		textureView = 0;
	}
}

ID3D11ShaderResourceView* ZeldaTexture::GetTexture()
{
	return textureView;
}

bool ZeldaTexture::UseSRGB()
{
	return useSRGB;
}

void ZeldaTexture::SetShaderResource(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, &textureView);
}

void ZeldaTexture::SetCubeMapShaderResource(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(TEXTURE_SLOT_CUBEMAP, 1, &textureView);
}
