#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

struct VertexType
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texture;
	DirectX::XMUINT4 boneIndices;
	DirectX::XMFLOAT4 weight;

	const static unsigned int size;
	const static D3D11_INPUT_ELEMENT_DESC layout[];
};

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

#pragma region Constant Buffer

struct LightBufferType
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 lightDirection;//
	float padding; // CreateBuffer�Լ��� �����Ϸ��� 16����Ʈ�� ����� ������ �Ѵ�.(������Ʈ �� ������ ����ϱ� ������ ȿ������ �޸� ������ �Ǿ��־�� �Ѵٰ� ��)
};

struct UseBufferType
{
	unsigned int useNormal;
	unsigned int useTexture;
	unsigned int useColor;
	unsigned int useSRGB;

	unsigned int useTemp0;
	unsigned int useTemp1;
	unsigned int useTemp2;
	unsigned int useTemp3;
};

struct ColorBufferType
{
	DirectX::XMFLOAT4 baseColor;
};

#pragma endregion

enum class ShaderType
{
	VertexShader,
	PixelShader
};