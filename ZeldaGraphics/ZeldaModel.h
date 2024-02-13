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
		double duration; // ƽ�� �ð�
		double tickPerSecond; // �ð��� ƽ

		// key: nodeName
		// value: map<time, Matrix> 
		std::map<std::wstring, std::map<double, AnimationKeyInfo>> animationKey;
	};

public:
	~ZeldaModel();

	void Render(
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
		ConstantBuffer<AnimationHierarchyBufferType, ShaderType::VertexShader>* animationHierarchyVsConstBuffer,
		ConstantBuffer<AnimationBufferType, ShaderType::VertexShader>* animationConstBuffer,
		ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
		DirectX::XMMATRIX worldMatrix,
		ZeldaShader* shader,
		const std::wstring& animationName,
		float animationTime
	);

	void RenderInstanced(
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
		ConstantBuffer<InstancingMatrixBufferType, ShaderType::VertexShader>* instancingMatrixConstBuffer,
		ConstantBuffer<AnimationHierarchyBufferType, ShaderType::VertexShader>* animationHierarchyVsConstBuffer,
		ConstantBuffer<InstancingAnimationBufferType, ShaderType::VertexShader>* instancingAnimationConstBuffer,
		ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
		const std::vector<ModelInstancingInfo>& instancingInfo,
		ZeldaShader* shader,
		const std::wstring& animationName
	);

	std::vector<std::wstring> GetAnimationList();
	std::vector<float> GetAnimationPlayTime();

private:
	ZeldaModel(ID3D11Device* device, FBXLoader::Model* fbxModel);
	ZeldaModel(const ZeldaModel& zeldaModel) = delete;

	// �ʴ� �ִϸ��̼� ƽ�� targetTPS�� �ǵ��� �ִϸ��̼� tick�� �����Ͽ� �����͸� �����Ѵ�.
	void CreateAnimationResourceView(ID3D11Device* device);
	void SetAnimationTexture(ID3D11DeviceContext* deviceContext, unsigned int animationID);

	// �⺻ ������ �� ���� matrix�� ���Ͽ� Node.worldMatrix�� ��´�.
	void CalculateIdleBoneTM();
	// Ư�� �ִϸ��̼��� boneTM�� ����Ͽ� Node.FinalTM�� ��´�.
	void CalculateAnimationBoneTM(Animation* animation, float tickTime);

	void CopyNode(Node* node, FBXLoader::Bone* bone, std::map<std::wstring, Node*>& nodeTable);

	Node* root;
	std::vector<Node*> bones;
	std::vector<ZeldaMesh*> meshes;
	std::vector<unsigned int> materialIndex; // meshes[0]�� materials[materialIndex[0]]�� ����
	std::vector<ZeldaMaterial*> materials;
	std::unordered_map<std::wstring, Animation*> animationTable;


	// animationTable������ ������� ID�� 1���� �ο��Ѵ�. (0�� �ִϸ��̼��� ������� ���� ����)
	std::unordered_map<std::wstring, unsigned int> animationIDTable;
	// �� �ִϸ��̼��� �ؽ���ȭ ��Ų �������� TickPerSecond�� �����ϴ� �����̳�
	std::unordered_map<unsigned int, float> animationTPSTable;


	std::unordered_map<unsigned int, ID3D11ShaderResourceView*> animationResourceViewTable;

	friend class ResourceManager;
};