#include "PhysicsSystem.h"

#include "Physics.h"

#include <bitset>

#include "EngineSetting.h"
#include "ZnQueryDesc.h"

namespace PurahEngine
{
	bool Physics::Raycast(
		const Eigen::Vector3f& _from, 
		const Eigen::Vector3f& _to, float _distance,
		const std::vector<int>& _layers,
		ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _from;
		desc.direction = _to;
		desc.distance = _distance;

		std::bitset<32> layer;
		for (int i = 0; i < _layers.size(); i++)
			layer.set(_layers[i]);
		desc.queryLayer = layer.to_ulong();

		return PhysicsSystem::GetInstance().Raycast(desc, _info);
	}

	bool Physics::Raycast(
		const Eigen::Vector3f& _from, 
		const Eigen::Vector3f& _to, float _distance,
		unsigned int _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _from;
		desc.direction = _to;
		desc.distance = _distance;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Raycast(desc, _info);
	}

	bool Physics::Boxcast(
		const Eigen::Vector3f& _extend, 
		const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
		const Eigen::Vector3f& _dir, float _distance, 
		const std::vector<int>& _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;

		std::bitset<32> layer;
		for (int i = 0; i < _layers.size(); i++)
			layer.set(_layers[i]);
		desc.queryLayer = layer.to_ulong();

		return PhysicsSystem::GetInstance().Boxcast(_extend, desc, _info);
	}

	bool Physics::Boxcast(
		const Eigen::Vector3f& _extend, 
		const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
		const Eigen::Vector3f& _dir, float _distance, 
		unsigned int _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Boxcast(_extend, desc, _info);
	}

	bool Physics::Spherecast(
		float radius, 
		const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
		const Eigen::Vector3f& _dir, float _distance, 
		const std::vector<int>& _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;

		std::bitset<32> layer;
		for (int i = 0; i < _layers.size(); i++)
			layer.set(_layers[i]);
		desc.queryLayer = layer.to_ulong();

		return PhysicsSystem::GetInstance().Spherecast(radius, desc, _info);
	}

	bool Physics::Spherecast(
		float radius, 
		const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
		const Eigen::Vector3f& _dir, float _distance, 
		unsigned int _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Spherecast(radius, desc, _info);
	}

	bool Physics::Capsulecast(
		float height, float radius, 
		const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
		const Eigen::Vector3f& _dir, float _distance, 
		const std::vector<int>& _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;

		std::bitset<32> layer;
		for (int i = 0; i < _layers.size(); i++)
			layer.set(_layers[i]);
		desc.queryLayer = layer.to_ulong();

		return PhysicsSystem::GetInstance().Capsulecast(radius, height, desc, _info);
	}

	bool Physics::Capsulecast(
		float height, float radius, 
		const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot, 
		const Eigen::Vector3f& _dir, float _distance, 
		unsigned int _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Capsulecast(radius, height, desc, _info);
	}

	bool Physics::SphereOverlap(float _radius, const Eigen::Vector3f& _pos, const Eigen::Quaternionf _rot, unsigned int _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.distance = _radius;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().SphereOverlap(_radius, desc, _info);
	}

	int Physics::GetLayerID(const std::wstring& _name)
	{
		return EngineSetting::GetInstance().LayertoLayerID(_name);
	}

	Eigen::Vector3f Physics::GetGravity()
	{
		return PhysicsSystem::GetInstance().GetGravity();
	}

	void Physics::SetGravity(const Eigen::Vector3f& _gravity)
	{
		PhysicsSystem::GetInstance().SetGravity(_gravity);
	}
}
