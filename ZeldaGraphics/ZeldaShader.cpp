#include "ZeldaShader.h"

#include "ZeldaMesh.h"
#include "ZeldaTexture.h"

#include "ZeldaGraphicsDefine.h"

ZeldaShader::ZeldaShader(ID3D11Device* device, const std::wstring& vsFileName, const std::wstring& psFileName) :
	vertexShader(nullptr),
	pixelShader(nullptr),
	layout(nullptr),
	samplerState(nullptr)
{
	Initialize(device, vsFileName, psFileName);
}

ZeldaShader::~ZeldaShader()
{
	// Release the sampler state.
	if (samplerState)
	{
		samplerState->Release();
		samplerState = 0;
	}
	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	// Release the pixel shader.
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = 0;
	}
	// Release the vertex shader.
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = 0;
	}
}

bool ZeldaShader::Initialize(ID3D11Device* device, const std::wstring& vsFileName, const std::wstring& psFileName)
{
	HRESULT result;
	ID3DBlob* errorMessage;
	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;
	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DReadFileToBlob(vsFileName.c_str(), &vertexShaderBuffer);
	if (FAILED(result))
	{
		MessageBox(0, L"Failed to load compiled shader file", L"Shader Error", MB_OK);
		return false;
	}

	// Compile the pixel shader code.
	result = D3DReadFileToBlob(psFileName.c_str(), &pixelShaderBuffer);
	if (FAILED(result))
	{
		MessageBox(0, L"Failed to load compiled pixel shader file", L"Shader Error", MB_OK);
		return false;
	}

	// Create the vertex shader from the buffer.
	// ID3D11VertexShader 객체 생성
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(result)) return false;

	// Create the pixel shader from the buffer.
	// ID3D11PixelShade 객체 생성
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(result)) return false;

	// Create the vertex input layout.
	result = device->CreateInputLayout(VertexType::layout, VertexType::size,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout);
	if (FAILED(result)) return false;

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	// ID3D11VertextShader, ID3D11PixelShader객체에 bytecode가 저장되어 있음. 더이상 버퍼는 필요없다.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(result)) return false;

	return true;
}

bool ZeldaShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, ZeldaTexture* texture)
{
	bool result = true;

	// Set the shader parameters that it will use for rendering.
	if (texture != nullptr)
	{
		result = SetShaderParameters(deviceContext, texture->GetTexture());
	}
	if (!result) return false;
	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

ID3D11SamplerState* ZeldaShader::GetSamplerState()
{
	return samplerState;
}

bool ZeldaShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void ZeldaShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
