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
	Light
};

enum class RenderOption : unsigned int
{
	None = 0x00000000u,
	OutLine = 0x00000001u,
	FastOutLine = 0x00000002u,
	Shadow = 0x00000004u
};

struct InstancingKey
{
	MeshID meshID;
	TextureID textureID;
	ModelID modelID;
	LightID lightID;
	unsigned int animationID; // ������ animationName�� Model class�� ���� id�� ��ȯ�ؼ� �����ϵ��� ����
	Color color;

	InstancingKey() :
		meshID(MeshID::ID_NULL),
		textureID(TextureID::ID_NULL),
		modelID(ModelID::ID_NULL),
		lightID(LightID::ID_NULL),
		animationID(0xffffffff),
		color({ 0.0f, 0.0f, 0.0f, 0.0f })
	{}

	bool operator==(const InstancingKey& right) const
	{
		return
			meshID == right.meshID &&
			textureID == right.textureID &&
			modelID == right.modelID &&
			lightID == right.lightID &&
			animationID == right.animationID &&
			color == right.color;
	}
};

struct InstancingValue
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 size;
	float animationTime;
	Color color; // �������� InstancingKey������ �׳� 1024������ �� �Ѱ��ָ鼭 �ν��Ͻ� �غ���
	int layer;
	float fontSize;
	std::wstring str;

	InstancingValue() :
		worldMatrix(DirectX::XMMatrixIdentity()),
		position({ 0.0f, 0.0f }),
		size({ 0.0f, 0.0f }),
		animationTime(0.0f),
		color({ 1.0f, 1.0f, 1.0f, 1.0f }),
		layer(0),
		fontSize(30.0f),
		str(L"")
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
				hash<unsigned int>{}(obj.animationID) ^
				hash<Color>{}(obj.color);
		};
	};
}