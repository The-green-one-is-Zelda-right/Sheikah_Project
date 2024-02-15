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

	// 초당 애니메이션 틱이 targetTPS가 되도록 애니메이션 tick을 보간하여 데이터를 생성한다.
	void CreateAnimationResourceView(ID3D11Device* device);
	void SetAnimationTexture(ID3D11DeviceContext* deviceContext, unsigned int animationID);

	// 기본 상태의 각 본의 matrix를 구하여 Node.worldMatrix에 담는다.
	void CalculateIdleBoneTM();
	// 특정 애니메이션의 boneTM을 계산하여 Node.FinalTM에 담는다.
	void CalculateAnimationBoneTM(Animation* animation, float tickTime);

	void CopyNode(Node* node, FBXLoader::Bone* bone, std::map<std::wstring, Node*>& nodeTable);

	Node* root;
	std::vector<Node*> bones;
	std::vector<ZeldaMesh*> meshes;
	std::vector<unsigned int> materialIndex; // meshes[0]은 materials[materialIndex[0]]을 가짐
	std::vector<ZeldaMaterial*> materials;
	std::unordered_map<std::wstring, Animation*> animationTable;


	// animationTable에서의 순서대로 ID를 1부터 부여한다. (0은 애니메이션이 적용되지 않은 상태)
	std::unordered_map<std::wstring, unsigned int> animationIDTable;
	// 각 애니메이션을 텍스쳐화 시킨 데이터의 TickPerSecond를 저장하는 컨테이너
	std::unordered_map<unsigned int, float> animationTPSTable;


	std::unordered_map<unsigned int, ID3D11ShaderResourceView*> animationResourceViewTable;

	friend class ResourceManager;
};