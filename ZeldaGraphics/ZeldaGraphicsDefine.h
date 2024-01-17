#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

constexpr unsigned int LIGHT_COUNT_MAX = 50u;
constexpr unsigned int BONE_COUNT_MAX = 256u;

namespace Deferred
{
	namespace Object
	{
		constexpr unsigned int Begin = 0u;

		constexpr unsigned int Position = 0u;
		constexpr unsigned int Normal = 1u;
		constexpr unsigned int Albedo = 2u;
		constexpr unsigned int Depth = 3u;

		constexpr unsigned int Count = 4u;
	}

	namespace Light
	{
		constexpr unsigned int Begin = 4u;

		constexpr unsigned int Diffuse = 4u;
		constexpr unsigned int Specular = 5u;

		constexpr unsigned int Count = 2u;
	}

	constexpr unsigned int bufferCount = 6u;
	constexpr unsigned int slotBegin = 5u;
}

enum class LightType : unsigned int
{
	Directional = 0u,
	Point = 1u,
	Spot = 2u
};

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

#pragma region Constant Buffer

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	float cameraFar;
	float padding[3];

	constexpr static unsigned int registerNum = 0;
};

struct BoneBufferType
{
	DirectX::XMMATRIX boneTM[BONE_COUNT_MAX];

	constexpr static unsigned int registerNum = 1;
};

struct LightColor
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
};

struct LightInfo
{
	LightColor color;
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 direction;
	LightType type;
	float range;
	float angle;
	float padding;
};

struct LightInfoBufferType
{
	LightInfo lights[LIGHT_COUNT_MAX];

	constexpr static unsigned int registerNum = 2;
};

struct LightIndexBufferType
{
	unsigned int lightIndex;
	float padding[3];

	constexpr static unsigned int registerNum = 3;
};

struct MaterialBufferType
{
	DirectX::XMFLOAT4 baseColor;

	unsigned int useBaseColor;
	unsigned int useSRGB;
	unsigned int useDiffuse;
	unsigned int useNormal;

	unsigned int useARM;
	unsigned int useHeight;
	unsigned int useEmission;
	unsigned int useTemp0;

	unsigned int useTemp1;
	unsigned int useTemp2;
	unsigned int useTemp3;
	unsigned int useTemp4;

	constexpr static unsigned int registerNum = 4;
};

struct ScreenBufferType
{
	DirectX::XMFLOAT2 screenSize;
	float padding[2];

	constexpr static unsigned int registerNum = 5;
};

#pragma endregion

enum class ShaderType
{
	VertexShader,
	PixelShader,
	VertexShaderAndPixelShader
};