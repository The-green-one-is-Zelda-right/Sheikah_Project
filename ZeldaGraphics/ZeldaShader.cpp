#include "ZeldaShader.h"

#include "ZeldaMesh.h"
#include "ZeldaTexture.h"

#include "ZeldaGraphicsDefine.h"

ZeldaShader::ZeldaShader(ID3D11Device* device, const std::wstring& vsFileName, const std::wstring& psFileName, const std::wstring& instVSFileName) :
	vertexShader(nullptr),
	instancingVertexShader(nullptr),
	pixelShader(nullptr),
	layout(nullptr),
	instancingLayout(nullptr),
	samplerState(nullptr)
{
	Initialize(device, vsFileName, psFileName, instVSFileName);
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
	if (instancingLayout)
	{
		instancingLayout->Release();
		instancingLayout = 0;
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
	if (instancingVertexShader)
	{
		instancingVertexShader->Release();
		instancingVertexShader = 0;
	}
}

bool ZeldaShader::Initialize(ID3D11Device* device, const std::wstring& vsFileName, const std::wstring& psFileName, const std::wstring& instVSFileName)
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
		result = D3DReadFileToBlob((L"CompiledShader\\" + vsFileName).c_str(), &vertexShaderBuffer);
	}
	if (FAILED(result))
	{
		MessageBox(0, L"Failed to load compiled shader file", L"Shader Error", MB_OK);
		return false;
	}

	// Compile the pixel shader code.
	result = D3DReadFileToBlob(psFileName.c_str(), &pixelShaderBuffer);
	if (FAILED(result))
	{
		result = D3DReadFileToBlob((L"CompiledShader\\" + psFileName).c_str(), &pixelShaderBuffer);
	}
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

	// Use Instancing Shader
	if (instVSFileName != L"")
	{
		ID3DBlob* instVertexShaderBuffer = 0;

		// Compile the instacing vertex shader code.
		result = D3DReadFileToBlob(instVSFileName.c_str(), &instVertexShaderBuffer);
		if (FAILED(result))
		{
			result = D3DReadFileToBlob((L"CompiledShader\\" + instVSFileName).c_str(), &instVertexShaderBuffer);
		}
		if (FAILED(result))
		{
			MessageBox(0, L"Failed to load compiled shader file", L"Shader Error", MB_OK);
			return false;
		}

		// Create the vertex shader from the buffer.
		// ID3D11VertexShader 객체 생성
		result = device->CreateVertexShader(instVertexShaderBuffer->GetBufferPointer(),
			instVertexShaderBuffer->GetBufferSize(), nullptr, &instancingVertexShader);
		if (FAILED(result)) return false;

		// Create the instancing vertex input layout.
		result = device->CreateInputLayout(InstancingVertexType::layout, InstancingVertexType::size,
			instVertexShaderBuffer->GetBufferPointer(), instVertexShaderBuffer->GetBufferSize(), &instancingLayout);
		if (FAILED(result)) return false;

		//ID3D11ShaderReflection* shaderReflection = nullptr;
		//D3DReflect(instVertexShaderBuffer->GetBufferPointer(), instVertexShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&shaderReflection);

		//// 상수 버퍼 정보 얻기
		//D3D11_SHADER_DESC shaderDesc;
		//shaderReflection->GetDesc(&shaderDesc);

		//for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
		//{
		//	ID3D11ShaderReflectionConstantBuffer* cb = shaderReflection->GetConstantBufferByIndex(i);

		//	D3D11_SHADER_BUFFER_DESC bufferDesc;
		//	cb->GetDesc(&bufferDesc);

		//	auto size = bufferDesc.Size;

		//}
		
		// 해제
		instVertexShaderBuffer->Release();
		instVertexShaderBuffer = nullptr;
	}

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

void ZeldaShader::Render(ID3D11DeviceContext* deviceContext, unsigned int indexCount)
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
}

void ZeldaShader::RenderInstanced(ID3D11DeviceContext* deviceContext, unsigned int indexCount, unsigned int instanceCount, unsigned int instanceStart)
{
	assert(instancingVertexShader != nullptr && instancingLayout != nullptr);

	// Set the vertex input layout.
	deviceContext->IASetInputLayout(instancingLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(instancingVertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	// Render the triangle.
	deviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, instanceStart);
}

ID3D11SamplerState* ZeldaShader::GetSamplerState()
{
	return samplerState;
}