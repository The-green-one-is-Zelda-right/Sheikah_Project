#pragma once
#include "./include/RawMesh.h"

namespace flt
{
	template struct flt::IBuilder<flt::RawMesh>;

	struct AssimpRawMeshBuilder : public IBuilder<RawMesh>
	{
	public:
		virtual RawMesh* Build() override;
	};
}

