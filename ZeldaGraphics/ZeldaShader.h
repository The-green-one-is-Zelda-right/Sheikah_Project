#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <Windows.h>

class ZeldaMesh;
class ZeldaTexture;

class ZeldaShader
{
public:
	ZeldaShader(ID3D11Device* device, const std::wstring& vsFileName, const std::wstring& psFileName, const std::wstring& instVSFileName = L"");
	ZeldaShader(const ZeldaShader& zeldaShader) = delete;
	~ZeldaShader();

	void Render(ID3D11DeviceContext* deviceContext, unsigned int indexCount);
	void RenderInstanced(ID3D11DeviceContext* deviceContext, unsigned int indexCount, unsigned int instanceCount, unsigned int instanceStart);
	ID3D11SamplerState* GetSamplerState();

private:
	bool Initialize(ID3D11Device* device, const std::wstring& vsFileName, const std::wstring& psFileName, const std::wstring& instVSFileName);

private:
	ID3D11VertexShader* vertexShader;
	ID3D11VertexShader* instancingVertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11InputLayout* instancingLayout;
	ID3D11SamplerState* samplerState;
};

