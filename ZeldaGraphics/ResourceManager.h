#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include <d3d11.h>
#include <DirectXMath.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GraphicsResourceID.h"

class ZeldaCamera;
class ZeldaTexture;
class ZeldaModel;
class ZeldaMesh;
class ZeldaShader;
class ZeldaLight;

struct ZNode;
struct ZMesh;

namespace std {
	template <>
	struct hash<std::pair<std::wstring, std::wstring>> {
		std::size_t operator()(const std::pair<std::wstring, std::wstring>& key) const {
			hash<wstring> strHash;
			return strHash(key.first + key.second);
		}
	};
}

class ResourceManager
{
public:
	void Initialize(ID3D11Device* device);
	void Finalize();

	MeshID CreateCubeMesh();
	MeshID CreateSquareMesh();
	MeshID CreateCircleMesh(); // 반지름 0.5인 원에 외접하는 정 16각형을 생성함
	MeshID CreateSphereMesh();
	MeshID CreateCapsuleMesh();
	MeshID CreateCylinderMesh();
	CameraID CreateCamera(unsigned int screenWidth, unsigned int screenHeight);
	TextureID CreateTexture(const std::wstring& filePath);
	ModelID CreateModelFromModelingFile(const std::wstring& filePath);
	ShaderID CreateShader(const std::wstring& vsFilePath, const std::wstring& psFilePath);
	LightID CreateDirectionalLight();
	LightID CreatePointLight();
	LightID CreateSpotLight();

	void ReleaseCubeMesh();
	void ReleaseSquareMesh();
	void ReleaseCircleMesh();
	void ReleaseSphereMesh();
	void ReleaseCapsuleMesh();
	void ReleaseCylinderMesh();
	void ReleaseCamera(CameraID cameraID);
	void ReleaseTexture(TextureID textureID);
	void ReleaseModel(ModelID modelID);
	void ReleaseMesh(MeshID meshID);
	void ReleaseShader(ShaderID shaderID);
	void ReleaseLight(LightID lightID);

	MeshID GetCubeID();
	MeshID GetSquareID();
	MeshID GetCircleID();
	MeshID GetSphereID();
	MeshID GetCapsuleID();
	MeshID GetCylinderID();

	ZeldaMesh* GetCubeMesh();
	ZeldaMesh* GetSquareMesh();
	ZeldaMesh* GetCircleMesh();
	ZeldaMesh* GetSphereMesh();
	ZeldaMesh* GetCapsuleMesh();
	ZeldaMesh* GetCylinderMesh();
	ZeldaCamera* GetCamera(CameraID cameraID);
	ZeldaTexture* GetTexture(TextureID key);
	ZeldaModel* GetModel(ModelID key);
	ZeldaMesh* GetMesh(MeshID key);
	ZeldaShader* GetShader(ShaderID key);
	ZeldaLight* GetLight(LightID key);

	bool CheckCameraID(CameraID cameraID);


private:
	ID3D11Device* device;
	std::unordered_map<CameraID, ZeldaCamera*> cameraTable;
	std::unordered_map<TextureID, ZeldaTexture*> textureTable;
	std::unordered_map<ModelID, ZeldaModel*> modelTable;
	std::unordered_map<MeshID, ZeldaMesh*> meshTable;
	std::unordered_map<ShaderID, ZeldaShader*> shaderTable;
	std::unordered_map<LightID, ZeldaLight*> lightTable;


	MeshID cubeID;
	MeshID squareID;
	MeshID circleID;
	MeshID sphereID;
	MeshID capsuleID;
	MeshID cylinderID;

	//singleton
public:
	static ResourceManager& GetInstance();

private:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager& ref) = delete;
	ResourceManager& operator=(const ResourceManager& ref) = delete;
};