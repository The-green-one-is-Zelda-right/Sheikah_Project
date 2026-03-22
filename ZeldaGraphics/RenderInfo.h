#pragma once

#include "GraphicsResourceID.h"
#include "ZeldaGraphicsDefine.h"

#include "Color.h"

enum class RenderType
{
	Deferred_Mesh,
	Deferred_Model,
	Deferred_BlendingAnimation,
	Forward_Mesh,
	Forward_Model,
	Forward_BlendingAnimation,
	Sprite,
	String,
	Light,
	CubeMap,
	BillBoard,
	Image,
	BillBoardParticle
};

using RenderOption = unsigned int;

namespace RenderInfoOption
{
	constexpr static unsigned int None = 0x00000000u;
	constexpr static unsigned int OutLine = 0x00000001u;
	constexpr static unsigned int FastOutLine = 0x00000002u;
	constexpr static unsigned int DirectionalLightShadow = 0x00000004u;
	constexpr static unsigned int PointLightShadow = 0x00000008u;
};

struct InstancingKey
{
	MeshID meshID;
	TextureID textureID;
	ModelID modelID;
	LightID lightID;
	unsigned int animationID; // 기존의 animationName을 Model class를 통해 id로 변환해서 저장하도록 하자

	InstancingKey() :
		meshID(MeshID::ID_NULL),
		textureID(TextureID::ID_NULL),
		modelID(ModelID::ID_NULL),
		lightID(LightID::ID_NULL),
		animationID(0xffffffff)
	{}

	bool operator==(const InstancingKey& right) const
	{
		return
			meshID == right.meshID &&
			textureID == right.textureID &&
			modelID == right.modelID &&
			lightID == right.lightID &&
			animationID == right.animationID;
	}
};

struct InstancingValue
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX particleMatrix;
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 size;
	float animationTime;
	Color color;
	Color outLineColor;
	unsigned int layer;
	float fontSize;
	std::wstring str;
	unsigned int blendAnimationID1;
	unsigned int blendAnimationID2;
	float blendAnimationTime1;
	float blendAnimationTime2;
	float ratio;
	float ccwRadianAngle;
	bool useAlphaTexture;
	unsigned int particleID;

	InstancingValue() :
		worldMatrix(DirectX::XMMatrixIdentity()),
		particleMatrix(DirectX::XMMatrixIdentity()),
		position({ 0.0f, 0.0f }),
		size({ 0.0f, 0.0f }),
		animationTime(0.0f),
		color({ 1.0f, 1.0f, 1.0f, 1.0f }),
		outLineColor({ 1.0f, 1.0f, 1.0f, 1.0f }),
		layer(0u),
		fontSize(30.0f),
		str(L""),
		blendAnimationID1(0u),
		blendAnimationID2(0u),
		blendAnimationTime1(0.0f),
		blendAnimationTime2(0.0f),
		ratio(0.0f),
		ccwRadianAngle(0.0f),
		useAlphaTexture(false),
		particleID(0u)
	{}
};

struct RenderInfo
{
	RenderType renderType;
	RenderOption renderOption;

	InstancingKey instancingKey;
	InstancingValue instancingValue;

	unsigned int drawID;
};

namespace std
{
	template <>
	struct hash<InstancingKey>
	{
		size_t operator()(const InstancingKey& obj) const
		{
			return
				hash<MeshID>{}(obj.meshID) ^
				hash<TextureID>{}(obj.textureID) ^
				hash<ModelID>{}(obj.modelID) ^
				hash<LightID>{}(obj.lightID) ^
				hash<unsigned int>{}(obj.animationID);
		};
	};
}