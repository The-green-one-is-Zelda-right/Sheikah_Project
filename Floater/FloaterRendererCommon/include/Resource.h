#pragma once
#include <string>
#include <iostream>
#include "ResourceMgr.h"

namespace flt
{
	struct IBuilderBase;
	template<typename T> struct IBuilder;


	struct ResourceBase
	{
		friend class ResourceMgr;

		ResourceBase(const std::wstring& key) :
			_key(key),
			_pData(nullptr)
		{

		}
		ResourceBase(const ResourceBase& other) = delete;
		ResourceBase(ResourceBase&& other) noexcept :
			_pData(other._pData), 
			_key(other._key)
		{
			other._pData = nullptr;
		}

		ResourceBase& operator=(const ResourceBase& other) = delete;
		ResourceBase& operator=(ResourceBase&& other) noexcept
		{
			_pData = other._pData;
			_key = other._key;
			other._pData = nullptr;
			return *this;
		}

		virtual ~ResourceBase()
		{
		}

	protected:
		std::wstring _key;
		void* _pData;
	};

	template <typename T>
	concept ReleaseAble = requires(T a) {
		{ a.Release() } -> std::same_as<void>;
	};

	template<ReleaseAble Derived>
	struct Resource : ResourceBase
	{
	public:
		Resource() : ResourceBase(L"") {}
		Resource(const typename IBuilder<Derived>& builder) : ResourceBase(builder.key)
		{
			SetData(builder);
		}
		Resource(const Resource& other) = delete;
		Resource(Resource&& other) = default;

		Resource& operator=(const Resource& other) = delete;
		Resource& operator=(Resource&& other) noexcept
		{
			Release();
			return *this;
		}

		virtual ~Resource()
		{
			Release();
		}

		void SetData(const IBuilderBase& builder)
		{
			auto data = global::g_resourceMgr.GetResource(this, builder);
			Release();
			_pData = data;
		}

		void Release()
		{
			if (_pData)
			{
				if (global::g_resourceMgr.ReleaseResource(this))
				{
					((Derived*)_pData)->Release();
				}
				_pData = nullptr;
			}
		}

		operator Derived* () const
		{
			return (Derived*)_pData;
		}
		//Derived* operaotr* () const
		//{
		//	return (Derived*)_pData;
		//}
		//Derived* operator->() const
		//{
		//	return (Derived*)_pData;
		//}
	};
}
