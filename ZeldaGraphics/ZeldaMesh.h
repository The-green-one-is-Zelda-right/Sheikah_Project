#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>

#include "ZeldaGraphicsDefine.h"

class ZeldaMesh
{
public:
	ZeldaMesh(const ZeldaMesh& zeldaMesh) = delete;
	~ZeldaMesh();

	void Render(ID3D11DeviceContext* deviceContext);
	int GetIndexCount();

private:
	ZeldaMesh(ID3D11Device* device, const std::vector<VertexType>& vertexList, const std::vector<unsigned int>& indexList);

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount, indexCount;

	friend class ZeldaModel;
	friend class ResourceManager;
};

