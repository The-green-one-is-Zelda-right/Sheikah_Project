#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <algorithm>

#include "ZeldaGraphicsDefine.h"
#include "FBXData.h"

#include "ConstantBuffer.h"

class ZeldaMesh;
class ZeldaMaterial;
class ZeldaShader;
class ZeldaLight;

class ZeldaModel
{
private:
	struct Node
	{
		Node* parent;
		std::vector<Node*> children;

		std::wstring name;

		std::vector<unsigned int> meshes;

		DirectX::XMMATRIX finalTM;
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX transformMatrix;
		DirectX::XMMATRIX offsetMatrix;
	};

	struct Mesh
	{
		ZeldaMesh* meshData;

		unsigned int materialIndex;
	};

	struct AnimationKeyInfo
	{
		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR rotation;
		DirectX::XMVECTOR position;
	};

	struct Animation
	{
		double duration; // Æ½´ç ½Ã°£
		double tickPerSecond; // ½Ã°£´ç Æ½

		// key: nodeName
		// value: map<time, Matrix> 
		std::map<std::wstring, std::map<double, AnimationKeyInfo>> animationKey;
	};

public:
	~ZeldaModel();

	void Render(
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
		ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer,
		ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
		DirectX::XMMATRIX worldMatrix,
		ZeldaShader* shader,
		const std::wstring& animationName,
		float animationTime
	);

	std::vector<std::wstring> GetAnimationList();
	std::vector<float> GetAnimationPlayTime();

private:
	ZeldaModel(ID3D11Device* device, FBXLoader::Model* fbxModel);
	ZeldaModel(const ZeldaModel& zeldaModel) = delete;

	void CopyNode(Node* node, FBXLoader::Bone* bone, std::map<std::wstring, Node*>& nodeTable);

	void RenderAnimation(
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
		ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer,
		ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
		DirectX::XMMATRIX worldMatrix,
		ZeldaShader* shader,
		const std::wstring& animationName,
		float animationTime
	);

	Node* root;
	std::vector<Node*> bones;
	std::vector<ZeldaMesh*> meshes;
	std::vector<unsigned int> materialIndex; // meshes[0]Àº materials[materialIndex[0]]À» °¡Áü
	std::vector<ZeldaMaterial*> materials;
	std::unordered_map<std::wstring, Animation*> animationTable;

	bool updatedWorldMatrix;

	friend class ResourceManager;
};