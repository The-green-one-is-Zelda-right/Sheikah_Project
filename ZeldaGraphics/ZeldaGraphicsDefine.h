#pragma once

#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

constexpr unsigned int LIGHT_COUNT_MAX = 50u;
constexpr unsigned int ANIMATION_FRAME_MAX = 1024u;
constexpr unsigned int INSTANCING_MAX = 1024u;

constexpr unsigned int TEXTURE_SLOT_ANIMATION = 8u;
constexpr unsigned int TEXTURE_SLOT_CUBEMAP = 9u;

namespace Texture
{
	namespace Slot
	{
		constexpr unsigned int DiffuseMap = 0u;
		constexpr unsigned int NormalMap = 1u;
		constexpr unsigned int ARMMap = 2u;
		constexpr unsigned int HeightMap = 3u;
		constexpr unsigned int EmissionMap = 4u;
	}
}

namespace Model
{
	namespace Animation
	{
		namespace Bone
		{
			constexpr unsigned int Max = 256u;
		}

		namespace Tick
		{
			// 애니메이션 데이터를 텍스쳐화 할 때, 가능한 최대 duration
			// assimp로 로드한 데이터에는 0~duration까지의 틱을 쓰지만 여기서는 0~(Max-1)까지로 할 것임에 주의
			constexpr unsigned int Max = 8192u;

			// 애니메이션 데이터를 텍스쳐화 할 때, 생성하는 데이터의 목표 tickPerSecond
			// 여기에 정의된 값을 목표로 하되 애니메이션의 duration이 높다면 작아질 수 있다.
			constexpr float TargetTickPerSecond = 480u;
		}
	}
}

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

	constexpr unsigned int BufferCount = 6u;
	constexpr unsigned int SlotBegin = 5u;
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
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT2 texture;
	DirectX::XMUINT4 boneIndices;
	DirectX::XMFLOAT4 weight;

	const static unsigned int size;
	const static D3D11_INPUT_ELEMENT_DESC layout[];
};

struct InstancingVertexType
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT2 texture;
	DirectX::XMUINT4 boneIndices;
	DirectX::XMFLOAT4 weight;
	unsigned int instance;

	const static unsigned int size;
	const static D3D11_INPUT_ELEMENT_DESC layout[];

	InstancingVertexType& operator=(const VertexType& originVertexType)
	{
		this->position = originVertexType.position;
		this->normal = originVertexType.normal;
		this->texture = originVertexType.texture;
		this->boneIndices = originVertexType.boneIndices;
		this->weight = originVertexType.weight;

		return *this;
	}
};

#pragma region Constant Buffer

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	float cameraFar;
	float padding[3];

	constexpr static unsigned int registerNumB = 0;
};
static_assert(sizeof(MatrixBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

struct AnimationInfo
{
	float animationTime;
	float padding[3];
};

struct AnimationBufferType
{
	AnimationInfo animationInfo;

	constexpr static unsigned int registerNumB = 1;
};
static_assert(sizeof(AnimationBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

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

	constexpr static unsigned int registerNumB = 2;
};
static_assert(sizeof(LightInfoBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

struct LightIndexBufferType
{
	unsigned int lightIndex;
	float padding[3];

	constexpr static unsigned int registerNumB = 3;
};
static_assert(sizeof(LightIndexBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

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

	constexpr static unsigned int registerNumB = 4;
};
static_assert(sizeof(MaterialBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

struct ScreenBufferType
{
	DirectX::XMFLOAT2 screenSize;
	float padding[2];

	constexpr static unsigned int registerNumB = 5;
};
static_assert(sizeof(ScreenBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

struct InstancingMatrixBufferType
{
	DirectX::XMMATRIX instancingWorldMatrix[INSTANCING_MAX];

	constexpr static unsigned int registerNumB = 6;
};
static_assert(sizeof(InstancingMatrixBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

struct InstancingAnimationBufferType
{
	AnimationInfo animationInfo[INSTANCING_MAX];

	constexpr static unsigned int registerNumB = 7;
};
static_assert(sizeof(InstancingAnimationBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

struct BlendingAnimationBufferType
{
	DirectX::XMMATRIX finalTM[Model::Animation::Bone::Max];

	constexpr static unsigned int registerNumB = 8;
};
static_assert(sizeof(BlendingAnimationBufferType) % 16 == 0, "Constant Buffer size must be a multiple of 16 bytes");

#pragma endregion

enum class ShaderType
{
	VertexShader,
	PixelShader,
	VertexShaderAndPixelShader
};

struct MeshInstancingInfo
{
	DirectX::XMMATRIX worldMatrix;
};

struct ModelInstancingInfo
{
	DirectX::XMMATRIX worldMatrix;
	float time;
};

struct SpriteInstancingInfo
{
	DirectX::XMFLOAT2 position;
};