#pragma once
#include <Rpc.h>

namespace ZonaiPhysics
{
	enum class ResourceType
	{
		eNone,
		eMaterial,
		eConvex,
		eTriangle,
		eHeight,
	};

	template <ResourceType _Ty>
	struct ZnResourceID
	{
	public:
		ZnResourceID();
		ZnResourceID(size_t _data1, size_t _data2);
		ZnResourceID(const ZnResourceID& _id);
		ZnResourceID(ZnResourceID&& _id) noexcept;
		ZnResourceID& operator=(const ZnResourceID& _id);
		ZnResourceID& operator=(ZnResourceID&& _id) noexcept;
		ZnResourceID& operator=(std::nullptr_t) noexcept;

		static const ZnResourceID None;

		ResourceType type = _Ty;
		union 
		{
			GUID id;

			struct
			{
				size_t data1;
				size_t data2;
			};
		};

		friend struct std::hash<ZnResourceID>;

	public:
		bool operator==(const ZnResourceID& _id) const;
		bool operator!=(const ZnResourceID& _id) const;
		bool operator<(const ZnResourceID& _id) const;
		bool operator==(std::nullptr_t) const;
	};

	template <ResourceType _Ty>
	ZnResourceID<_Ty>::ZnResourceID(): 
		data1(), data2()
	{}

	template <ResourceType _Ty>
	ZnResourceID<_Ty>::ZnResourceID(size_t _data1, size_t _data2): 
		data1(_data1), data2(_data2)
	{}

	template <ResourceType _Ty>
	ZnResourceID<_Ty>::ZnResourceID(const ZnResourceID& _id):
		data1(_id.data1), data2(_id.data2)
	{}

	template <ResourceType _Ty>
	ZnResourceID<_Ty>::ZnResourceID(ZnResourceID&& _id) noexcept:
		data1(_id.data1), data2(_id.data2)
	{}

	template <ResourceType _Ty>
	ZnResourceID<_Ty>& ZnResourceID<_Ty>::operator=(const ZnResourceID& _id)
	{
		if (this != &_id)
		{
			data1 = _id.data1;
			data2 = _id.data2;
		}

		return *this;
	}

	template <ResourceType _Ty>
	ZnResourceID<_Ty>& ZnResourceID<_Ty>::operator=(ZnResourceID&& _id) noexcept
	{
		if (this != &_id)
		{
			data1 = std::move(_id.data1);
			data2 = std::move(_id.data2);
		}

		return *this;
	}

	template <ResourceType _Ty>
	ZnResourceID<_Ty>& ZnResourceID<_Ty>::operator=(std::nullptr_t) noexcept
	{
		data1 = 0;
		data2 = 0;

		return *this;
	}

	template <ResourceType _Ty>
	bool ZnResourceID<_Ty>::operator==(const ZnResourceID& _id) const
	{
		if (type != _id.type)
		{
			return type == _id.type;
		}
		else
		{
			return data1 == _id.data1 && data2 == _id.data2;
		}
	}

	template <ResourceType _Ty>
	bool ZnResourceID<_Ty>::operator!=(const ZnResourceID& _id) const
	{
		return !(*this == _id);
	}

	template <ResourceType _Ty>
	bool ZnResourceID<_Ty>::operator<(const ZnResourceID& _id) const
	{
		if (data1 < _id.data1)
		{
			return true;
		}
		else if (data1 == _id.data1)
		{
			if (data2 < _id.data2)
				return true;
		}

		return false;
	}

	template <ResourceType _Ty>
	bool ZnResourceID<_Ty>::operator==(std::nullptr_t) const
	{
		return data1 == 0 && data2 == 0;
	}

	template <ResourceType _Ty>
	const ZnResourceID<_Ty> ZnResourceID<_Ty>::None{};

	template <ResourceType _Ty>
	void CreateID(ZnResourceID<_Ty>& _id)
	{
		UuidCreate(&_id.id);
	}

	using ZnMaterialID = ZnResourceID<ResourceType::eMaterial>;
	using ZnConvexID = ZnResourceID<ResourceType::eConvex>;
	using ZnMeshID = ZnResourceID<ResourceType::eTriangle>;
	using ZnHeightID = ZnResourceID<ResourceType::eHeight>;
}


namespace std
{
	using namespace ZonaiPhysics;

	template<ZonaiPhysics::ResourceType _Ty>
	struct hash<ZnResourceID<_Ty>>
	{
		size_t operator()(const ZnResourceID<_Ty>& r) const noexcept
		{
			const std::size_t h1 = std::hash<std::size_t>{}(r.data1);
			const std::size_t h2 = std::hash<std::size_t>{}(r.data2);
			return h1 ^ (h2 << 1);
		}
	};
}