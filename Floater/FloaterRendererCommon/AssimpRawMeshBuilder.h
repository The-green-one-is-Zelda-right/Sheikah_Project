#pragma once
#include "./include/RawMesh.h"
#include "./include/IBuilder.h"
#include <unordered_map>
#include <string>

struct aiMesh;

namespace flt
{
	struct RawNode;

	template struct flt::IBuilder<flt::RawMesh>;

	struct AssimpRawMeshBuilder : public IBuilder<RawMesh>
	{
	public:
		AssimpRawMeshBuilder(std::unordered_map<std::wstring, RawNode*>& nodeMap);
		AssimpRawMeshBuilder(aiMesh* mesh, std::unordered_map<std::wstring, RawNode*>& nodeMap);

		virtual RawMesh* build() const override;

		aiMesh* mesh;
		std::unordered_map<std::wstring, RawNode*>& nodeMap;
	};
}

