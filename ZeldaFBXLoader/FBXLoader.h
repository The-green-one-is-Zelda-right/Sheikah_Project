#pragma once

#include <string>
#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace FBXLoader
{
	struct Material;
	struct Model;
	struct Bone;

	class FBXLoader
	{
	public:
		Model* CreateModelFromFBX(const std::wstring& filePath);
		void ReleaseModel(Model* model);

	private:
		void CopyNodeData(Bone* bone, aiNode* ainode, std::map<std::wstring, unsigned int>& boneIndexMap, std::vector<Bone*>& boneList, int& boneCount);
		void CopyMaterialData(Material* materialData, aiMaterial* assimpMaterial, const std::wstring& fileDirectory);

		static const unsigned int BONE_MAX;

	};
}