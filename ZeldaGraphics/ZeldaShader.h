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
	ZeldaShader(ID3D11Device* device, const std::wstring& vsFileName, const std::wstring& psFileName);
	ZeldaShader(const ZeldaShader& zeldaShader) = delete;
	~ZeldaShader();

	bool Render(ID3D11DeviceContext* deviceContext, ZeldaMesh* mesh, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, ZeldaTexture* texture, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 diffuse);

private:
	bool Initialize(ID3D11Device* device, const std::wstring& vsFileName, const std::wstring& psFileName);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 diffuse);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11SamplerState* samplerState;
};

