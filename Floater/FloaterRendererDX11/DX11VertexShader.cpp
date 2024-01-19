#include "DX11VertexShader.h"
#include "DX11VSConstantBuffer.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include <d3dcompiler.h>
#include <wrl/client.h>

#if defined(DEBUG) || defined(_DEBUG)
#include <dxgidebug.h>
#endif


//#pragma comment(lib, "D3DCompiler.lib")
template<typename T>
using comptr = Microsoft::WRL::ComPtr<T>;

flt::DX11VertexShaderBuilder::~DX11VertexShaderBuilder()
{

}

flt::DX11VertexShader* flt::DX11VertexShaderBuilder::build() const 
{
	if (pDevice == nullptr)
	{
		ASSERT(false, "디바이스 세팅 안됨.");
		return nullptr;
	}

	UINT flags1 = 0;
#if defined(_DEBUG) || defined(_DEBUG)
	flags1 |= D3DCOMPILE_DEBUG;
#endif

	comptr<ID3D10Blob> vertexShaderBlob = nullptr;
	HRESULT hResult = D3DCompileFromFile(key.c_str(), nullptr, nullptr, "main", "vs_5_0", flags1, 0, &vertexShaderBlob, nullptr);
	if (hResult != S_OK)
	{
		ASSERT(false, "버텍스 쉐이더 컴파일 실패");
		return nullptr;
	}

	ID3D11VertexShader* vertexShader = nullptr;
	hResult = pDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
	if (hResult != S_OK)
	{
		ASSERT(false, "버텍스 쉐이더 생성 실패");
		return nullptr;
	}

	ID3D11InputLayout* inputLayout = nullptr;
	hResult = pDevice->CreateInputLayout(&(inputLayoutDesc[0]), inputLayoutDesc.size(),
		vertexShaderBlob->GetBufferPointer(), (UINT)(vertexShaderBlob->GetBufferSize()),
		&inputLayout);
	if (hResult != S_OK)
	{
		ASSERT(false, "인풋 레이아웃 생성 실패");
		vertexShader->Release();
		return nullptr;
	}

	ID3D11Buffer* constantBuffer = nullptr;
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(VSConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	hResult = pDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
	if (hResult != S_OK)
	{
		ASSERT(false, "상수 버퍼 생성 실패");
		vertexShader->Release();
		inputLayout->Release();
		return nullptr;
	}

	ID3D11Buffer* boneConstantBuffer = nullptr;
	D3D11_BUFFER_DESC boneConstantBufferDesc = {};
	boneConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	boneConstantBufferDesc.ByteWidth = sizeof(VSBoneConstantBuffer);
	boneConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	boneConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	boneConstantBufferDesc.MiscFlags = 0;
	boneConstantBufferDesc.StructureByteStride = 0;

	hResult = pDevice->CreateBuffer(&boneConstantBufferDesc, nullptr, &boneConstantBuffer);
	if (hResult != S_OK)
	{
		ASSERT(false, "본 상수 버퍼 생성 실패");
		vertexShader->Release();
		inputLayout->Release();
		constantBuffer->Release();
		return nullptr;
	}

	DX11VertexShader* dx11VertexShader = new DX11VertexShader();
	dx11VertexShader->pVertexShader = vertexShader;
	dx11VertexShader->pInputLayout = inputLayout;
	dx11VertexShader->pConstantBuffer = constantBuffer;
	dx11VertexShader->pBoneConstantBuffer = boneConstantBuffer;

	return dx11VertexShader;
}
