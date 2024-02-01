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
		double duration; // 틱당 시간
		double tickPerSecond; // 시간당 틱

		// key: nodeName
		// value: map<time, Matrix> 
		std::map<std::wstring, std::map<double, AnimationKeyInfo>> animationKey;
	};

public:
	~ZeldaModel();

	void Render(
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
		ConstantBuffer<AnimationBufferType, ShaderType::VertexShader>* animationConstBuffer,
		ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
		DirectX::XMMATRIX worldMatrix,
		ZeldaShader* shader,
		const std::wstring& firstAnimationName,
		const std::wstring& secondAnimationName,
		float firstAnimationTime,
		float secondAnimationTime,
		float ratio
	);

	void RenderInstanced(
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
		ConstantBuffer<InstancingMatrixBufferType, ShaderType::VertexShader>* instancingMatrixConstBuffer,
		ConstantBuffer<InstancingAnimationBufferType, ShaderType::VertexShader>* instancingAnimationConstBuffer,
		ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
		const std::vector<ModelInstancingInfo>& instancingInfo,
		ZeldaShader* shader
	);

	std::vector<std::wstring> GetAnimationList();
	std::vector<float> GetAnimationPlayTime();

private:
	ZeldaModel(ID3D11Device* device, FBXLoader::Model* fbxModel);
	ZeldaModel(const ZeldaModel& zeldaModel) = delete;

	void CreateAnimationResourceView(ID3D11Device* device);
	void SetAnimationTexture(ID3D11DeviceContext* deviceContext);

	void CopyNode(Node* node, FBXLoader::Bone* bone, std::map<std::wstring, Node*>& nodeTable);

	Node* root;
	std::vector<Node*> bones;
	std::vector<ZeldaMesh*> meshes;
	std::vector<unsigned int> materialIndex; // meshes[0]은 materials[materialIndex[0]]을 가짐
	std::vector<ZeldaMaterial*> materials;
	std::unordered_map<std::wstring, Animation*> animationTable;

	// animationTable에서의 순서대로 ID를 1부터 부여한다. (0은 애니메이션이 적용되지 않은 상태)
	std::unordered_map<std::wstring, unsigned int> animationIDTable;

	ID3D11ShaderResourceView* animationResourceView;

	bool updatedWorldMatrix;

	friend class ResourceManager;
};