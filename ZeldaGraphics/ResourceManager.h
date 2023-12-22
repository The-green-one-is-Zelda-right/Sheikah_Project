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

class ZeldaModel;
class ZeldaMesh;
class ZeldaTexture;
class ZeldaShader;
class ZeldaCamera;

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

namespace std {
	template <>
	struct hash<CameraID> {
		std::size_t operator()(const CameraID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<TextureID> {
		std::size_t operator()(const TextureID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<ModelID> {
		std::size_t operator()(const ModelID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<MeshID> {
		std::size_t operator()(const MeshID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<ShaderID> {
		std::size_t operator()(const ShaderID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

class ResourceManager
{
public:
	void Initialize(ID3D11Device* device);

	ModelID CreateModelFromModelingFile(const std::wstring& filePath);

	bool CreateCubeMesh();
	
	TextureID CreateTexture(const std::wstring& filePath);

	bool CreateDefaultShader();
	ShaderID CreateShader(const std::wstring& vsFilePath, const std::wstring& psFilePath);

	CameraID CreateCamera(unsigned int screenWidth, unsigned int screenHeight);

	ZeldaModel* GetModel(ModelID key);
	ZeldaMesh* GetCubeMesh();
	ZeldaMesh* GetMesh(MeshID key);
	ZeldaTexture* GetTexture(TextureID key);
	ZeldaShader* GetDefaultShader();
	ZeldaShader* GetShader(ShaderID key);
	ZeldaCamera* GetCamera(CameraID cameraID);

	bool CheckCameraID(CameraID cameraID);


	bool ReleaseCamera(CameraID cameraID);

private:
	ID3D11Device* device;
	std::unordered_map<ModelID, ZeldaModel*> modelTable;
	std::unordered_map<MeshID, ZeldaMesh*> meshTable;
	std::unordered_map<TextureID, ZeldaTexture*> textureTable;
	std::unordered_map<ShaderID, ZeldaShader*> shaderTable;

	std::unordered_map<CameraID, ZeldaCamera*> cameraTable;

	ZeldaMesh* cubeMesh;
	ZeldaShader* defaultShader;

	//singleton
public:
	static ResourceManager& GetInstance();

private:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager& ref) = delete;
	ResourceManager& operator=(const ResourceManager& ref) = delete;
};