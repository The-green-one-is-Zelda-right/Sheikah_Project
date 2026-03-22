#pragma once

#include "PurahEngineAPI.h"

#include <string>
#include <vector>

namespace PurahEngine
{
	class PURAHENGINE_API Tag final
	{
	public:
		Tag();
		Tag(const std::initializer_list<int>& elements);
		Tag(const std::initializer_list<std::wstring>& elements);
		~Tag();

		bool IsContain(const std::wstring& element) const;
		bool IsExclude(const std::wstring& element) const;

		bool operator==(const Tag& right) const;
		bool operator!=(const Tag& right) const;

		Tag operator&(const Tag& right) const;
		Tag operator|(const Tag& right) const;

		Tag& operator=(const std::vector<std::wstring>& right);

	private:
		std::vector<bool> tagElements;
	};
}