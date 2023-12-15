#pragma once
#include "../FloaterMath/include/Vector2f.h"
#include "../FloaterMath/include/Vector3f.h"
#include "../FloaterMath/include/Vector4f.h"
#include "fbxsdk.h"
#include <vector>

namespace flt
{
	template<typename FbxLayerElementType>
	struct FLTVector;

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementNormal>
	{
		using type = Vector3f;
		static constexpr int count = 3;
	};

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementUV>
	{
		using type = Vector2f;
		static constexpr int count = 2;
	};

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementVertexColor>
	{
		using type = Vector4f;
		static constexpr int count = 4;
	};

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementTangent>
	{
		using type = Vector3f;
		static constexpr int count = 3;
	};

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementBinormal>
	{
		using type = Vector3f;
		static constexpr int count = 3;
	};


	template<typename FbxLayerElementType>
	class VertexDataLayer
	{
		using FltVector = typename FLTVector<FbxLayerElementType>::type;
	public:
		VectorDayaLayer(const fbxsdk::FbxMesh& mesh)
		{

			const fbxsdk::FbxLayerElementNormal* layer = mesh.GetElementNormal(i);

			auto& element = layer->GetDirectArray();
			fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;

			auto referenceMode = layer->GetReferenceMode();
			if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
			{
				pIndices = &layer->GetIndexArray();
			}

			auto mappingMode = layer->GetMappingMode();
			// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
			// ByControlPoint : 컨트롤 포인트 별로 매핑
			// ByPolygon : 폴리곤 별로 매핑
			// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
			ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

			int elementCount = element.GetCount();

			_vertexData.resize(elementCount);

			for (int j = 0; j < elementCount; ++j)
			{
				int index = j;
				if (pIndices != nullptr)
				{
					index = pIndices->GetAt(j);
				}

				auto data = element.GetAt(index);
				for (int i = 0; i < FltVector::count; ++i)
				{
					_vertexData[i].m[i] = data.mData[i];
				}
			}
		}

	private:
		std::vector<FltVector> _vertexData;
		std::vector<int> _indices;
	};
}
