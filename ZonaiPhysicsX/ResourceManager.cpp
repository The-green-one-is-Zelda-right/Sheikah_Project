#include "FBXData.h"
#include "FBXLoader.h"

#include "ResourceManager.h"


namespace ZonaiPhysics
{
	std::unordered_map<std::wstring, ZnConvexID>				
	ResourceManager::convexPathTable{};

	std::unordered_map<std::wstring, ZnMeshID>				
	ResourceManager::trianglePathTable{};

	std::unordered_map<std::wstring, ZnHeightID>				
	ResourceManager::heightPathTable{};

	std::unordered_map<ZnMaterialID, physx::PxMaterial*>
	ResourceManager::materialIDTable{};

	std::unordered_map<ZnConvexID, physx::PxConvexMesh*>
	ResourceManager::convexIDTable{};

	std::unordered_map<ZnMeshID, physx::PxTriangleMesh*>
	ResourceManager::triangleIDTable{};

	std::unordered_map<ZnHeightID, physx::PxHeightField*>
	ResourceManager::heightIDTable{};

	void ResourceManager::Initialize()
	{

	}

	void ResourceManager::Release()
	{
		for (auto& material : materialIDTable | std::views::values)
		{
			material->release();
			material = nullptr;
		}

		materialIDTable.clear();

		for (auto& convex : convexIDTable | std::views::values)
		{
			convex->release();
			convex = nullptr;
		}

		convexIDTable.clear();

		for (auto& mesh : triangleIDTable | std::views::values)
		{
			mesh->release();
			mesh = nullptr;
		}

		triangleIDTable.clear();

		for (auto& height : heightIDTable | std::views::values)
		{
			height->release();
			height = nullptr;
		}

		heightIDTable.clear();

	}

	void ResourceManager::UnloadModel(FBXLoader::Model* _model)
	{
		FBXLoader::FBXLoader loader;
		loader.ReleaseModel(_model);
	}

	ZnMaterialID ResourceManager::RegistMaterial(physx::PxMaterial* _pxMaterial)
	{
		assert(_pxMaterial != nullptr);

		ZnMaterialID id;
		do
		{
			CreateID(id);
		} while (materialIDTable.contains(id));

		materialIDTable.insert(std::make_pair(id, _pxMaterial));

		return id;
	}

	physx::PxMaterial* ResourceManager::GetPxMaterial(const ZnMaterialID& _id)
	{
		if (_id == nullptr)
			return nullptr;

		if (materialIDTable.contains(_id))
			return materialIDTable[_id];

		return nullptr;
	}

	bool ResourceManager::ReleasePxMaterial(const ZnMaterialID& _id)
	{
		if (!materialIDTable.contains(_id))
			return false;

		const auto& material = materialIDTable[_id];
		if (!material->isReleasable())
			return false;

		material->release();
		materialIDTable.erase(_id);

		return true;
	}

	FBXLoader::Model* ResourceManager::LoadConvex(const std::wstring& _path)
	{
		if (convexPathTable.contains(_path))
			return nullptr;

		FBXLoader::FBXLoader loader;
		FBXLoader::Model* model = loader.CreateModelFromFBX(_path);

		return model;
	}

	ZnConvexID ResourceManager::AddConvex(physx::PxConvexMesh* _mesh)
	{
		assert(_mesh != nullptr);

		ZnConvexID id;
		do
		{
			CreateID(id);
		} while (convexIDTable.contains(id));

		convexIDTable.insert(std::make_pair(id, _mesh));

		return id;
	}

	physx::PxConvexMesh* ResourceManager::GetPxConvexShape(const ZnConvexID& _id)
	{
		if (_id == nullptr)
			return nullptr;

		if (convexIDTable.contains(_id))
			return convexIDTable[_id];

		return nullptr;
	}

	bool ResourceManager::ReleaseConvexMesh(const ZnConvexID& _id)
	{
		if (!convexIDTable.contains(_id))
			return false;

		const auto& convex = convexIDTable[_id];
		if (!convex->isReleasable())
			return false;

		convex->release();
		convexIDTable.erase(_id);

		return true;
	}

	FBXLoader::Model* ResourceManager::LoadMesh(const std::wstring& _path)
	{
		if (trianglePathTable.contains(_path))
			return nullptr;

		FBXLoader::FBXLoader loader;
		FBXLoader::Model* model = loader.CreateModelFromFBX(_path);

		return model;
	}

	ZnMeshID ResourceManager::AddMesh(physx::PxTriangleMesh* _mesh)
	{
		assert(_mesh != nullptr);

		ZnMeshID id;
		do
		{
			CreateID(id);
		} while (triangleIDTable.contains(id));

		triangleIDTable.insert(std::make_pair(id, _mesh));

		return id;
	}

	physx::PxTriangleMesh* ResourceManager::GetPxMeshShape(const ZnMeshID& _id)
	{
		if (_id == nullptr)
			return nullptr;

		if (triangleIDTable.contains(_id))
			return triangleIDTable[_id];

		return nullptr;
	}

	bool ResourceManager::ReleaseTriangleMesh(const ZnMeshID& _id)
	{
		if (!triangleIDTable.contains(_id))
			return false;

		const auto& mesh = triangleIDTable[_id];
		if (!mesh->isReleasable())
			return false;

		mesh->release();
		triangleIDTable.erase(_id);

		return true;
	}

	FBXLoader::Model* ResourceManager::LoadHeightField(const std::wstring& _path)
	{
		if (heightPathTable.contains(_path))
			return nullptr;

		FBXLoader::FBXLoader loader;
		FBXLoader::Model* model = loader.CreateModelFromFBX(_path);

		return model;
	}

	ZnHeightID ResourceManager::AddHeightField(physx::PxHeightField* _mesh)
	{
		assert(_mesh != nullptr);

		ZnHeightID id;
		do
		{
			CreateID(id);
		} while (heightIDTable.contains(id));

		heightIDTable.insert(std::make_pair(id, _mesh));

		return id;
	}

	physx::PxHeightField* ResourceManager::GetPxHeightField(const ZnHeightID& _id)
	{
		if (_id == nullptr)
			return nullptr;

		if (heightIDTable.contains(_id))
			return heightIDTable[_id];

		return nullptr;
	}

	bool ResourceManager::ReleaseHeightField(const ZnHeightID& _id)
	{
		if (!heightIDTable.contains(_id))
			return false;

		const auto& height = heightIDTable[_id];
		if (!height->isReleasable())
			return false;

		height->release();
		heightIDTable.erase(_id);

		return true;
	}
}
