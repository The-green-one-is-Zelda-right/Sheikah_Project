#pragma once
#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <PxPhysicsAPI.h>
#pragma warning (pop)

#include <ranges>
#include <string>
#include <unordered_map>
#include "ZnResourceID.h"

namespace physx
{
	class PxTriangleMesh;
	class PxConvexMesh;
	class PxShape;
	class PxMaterial;
}

namespace FBXLoader
{
	struct Model;
}

namespace ZonaiPhysics
{
	struct MaterialDesc;

	class ResourceManager
	{
	private:
		static std::unordered_map<std::wstring, ZnConvexID>				convexPathTable;
		static std::unordered_map<std::wstring, ZnMeshID>				trianglePathTable;
		static std::unordered_map<std::wstring, ZnHeightID>				heightPathTable;

		static std::unordered_map<ZnMaterialID, physx::PxMaterial*>		materialIDTable;
		static std::unordered_map<ZnConvexID, physx::PxConvexMesh*>		convexIDTable;
		static std::unordered_map<ZnMeshID, physx::PxTriangleMesh*>		triangleIDTable;
		static std::unordered_map<ZnHeightID, physx::PxHeightField*>	heightIDTable;

	public:
		static void						Initialize();
		static void						Release();

	public:
		static void						UnloadModel(FBXLoader::Model* _model);

		static ZnMaterialID				RegistMaterial(physx::PxMaterial*);
		static physx::PxMaterial*		GetPxMaterial(const ZnMaterialID& _material);
		static bool						ReleasePxMaterial(const ZnMaterialID& _material);

		static FBXLoader::Model*		LoadConvex(const std::wstring& _path);
		static ZnConvexID				AddConvex(physx::PxConvexMesh*);
		static physx::PxConvexMesh*		GetPxConvexShape(const ZnConvexID& _id);
		static bool						ReleaseConvexMesh(const ZnConvexID& _id);

		static FBXLoader::Model*		LoadMesh(const std::wstring& _path);
		static ZnMeshID					AddMesh(physx::PxTriangleMesh*);
		static physx::PxTriangleMesh*	GetPxMeshShape(const ZnMeshID& _id);
		static bool						ReleaseTriangleMesh(const ZnMeshID& _id);

		static FBXLoader::Model*		LoadHeightField(const std::wstring& _path);
		static ZnHeightID				AddHeightField(physx::PxHeightField*);
		static physx::PxHeightField*	GetPxHeightField(const ZnHeightID& _id);
		static bool						ReleaseHeightField(const ZnHeightID& _id);
	};
}
