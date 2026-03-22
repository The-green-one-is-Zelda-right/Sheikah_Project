#include "ZnLayer.h"


namespace ZonaiPhysics
{
	std::map<uint32_t, std::bitset<32>> ZnLayer::layerData = {};

	void ZnLayer::ValidateLayer(uint32_t _layerID)
	{
		assert(MIN_LAYER <= _layerID && _layerID <= MAX_LAYER && "Invalid layer ID");
	}

	void ZnLayer::Clear()
	{
		layerData.clear();
	}

	void ZnLayer::SetCollisionData(uint32_t _layerID, const std::initializer_list<uint32_t>& _collisionLayers)
	{
		ValidateLayer(_layerID);

		for (const auto layer : _collisionLayers)
		{
			ValidateLayer(layer);

			layerData[_layerID].set(layer);
			layerData[layer].set(_layerID);
		}
	}

	void ZnLayer::SetCollisionLayer(uint32_t _layerID, uint32_t _collisionLayerID, bool _value)
	{
		ValidateLayer(_layerID);
		ValidateLayer(_collisionLayerID);

		if (_value)
		{
			layerData[_layerID].set(_collisionLayerID);
			layerData[_collisionLayerID].set(_layerID);
		}
		else
		{
			layerData[_layerID].reset(_collisionLayerID);
			layerData[_collisionLayerID].reset(_layerID);
		}
	}

	bool ZnLayer::CanCollide(uint32_t _layerID0, uint32_t _layerID1)
	{
		ValidateLayer(_layerID0);
		ValidateLayer(_layerID1);

		// 해당 비트를 확인해보고 1이면 true를 반환함
		return layerData[_layerID0].test(_layerID1);
	}

	bool ZnLayer::IsCollide(uint32_t _layerData, uint32_t _layerID0)
	{
		// assert(_layerData != 0);
		assert(_layerID0 != 0);

		return _layerData & _layerID0;
	}
}
