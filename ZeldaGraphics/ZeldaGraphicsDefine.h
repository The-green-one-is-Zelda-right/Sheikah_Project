#pragma once

#include <DirectXMath.h>

struct VertexType
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;

	const static int size = 3;
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
	unsigned int useTemp0;
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