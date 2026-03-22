#include "Tag.h"

#include "EngineSetting.h"

namespace PurahEngine
{
	Tag::Tag()
	{
		int tagSize = EngineSetting::GetInstance().TagSize();

		tagElements.resize(tagSize, false);
	}

	Tag::Tag(const std::initializer_list<int>& elements)
	{
		int tagSize = EngineSetting::GetInstance().TagSize();

		tagElements.resize(tagSize, false);

		for (auto element : elements)
		{
			tagElements[element] = true;
		}
	}

	Tag::Tag(const std::initializer_list<std::wstring>& elements)
	{
		EngineSetting& setting = EngineSetting::GetInstance();

		int tagSize = EngineSetting::GetInstance().TagSize();

		tagElements.resize(tagSize, false);

		for (auto element : elements)
		{
			tagElements[setting.TagtoTagID(element)] = true;
		}
	}

	Tag::~Tag()
	{

	}

	bool Tag::IsContain(const std::wstring& element) const
	{
		int id = EngineSetting::GetInstance().TagtoTagID(element);

		return tagElements[id];
	}

	bool Tag::IsExclude(const std::wstring& element) const
	{
		int id = EngineSetting::GetInstance().TagtoTagID(element);

		return !tagElements[id];
	}

	bool Tag::operator==(const Tag& right) const
	{
		assert(tagElements.size() == right.tagElements.size());

		for (int i = 0; i < tagElements.size(); i++)
		{
			if (tagElements[i] != right.tagElements[i])
			{
				return false;
			}
		}

		return true;
	}

	bool Tag::operator!=(const Tag& right) const
	{
		assert(tagElements.size() == right.tagElements.size());

		for (int i = 0; i < tagElements.size(); i++)
		{
			if (tagElements[i] != right.tagElements[i])
			{
				return true;
			}
		}

		return false;
	}

	Tag Tag::operator&(const Tag& right) const
	{
		assert(tagElements.size() == right.tagElements.size());

		Tag result;

		for (int i = 0; i < tagElements.size(); i++)
		{
			result.tagElements[i] = (tagElements[i] && right.tagElements[i]);
		}

		return result;
	}

	Tag Tag::operator|(const Tag& right) const
	{
		assert(tagElements.size() == right.tagElements.size());

		Tag result;

		for (int i = 0; i < tagElements.size(); i++)
		{
			result.tagElements[i] = (tagElements[i] || right.tagElements[i]);
		}

		return result;
	}

	Tag& Tag::operator=(const std::vector<std::wstring>& right)
	{
		EngineSetting& setting = EngineSetting::GetInstance();

		int tagSize = EngineSetting::GetInstance().TagSize();

		tagElements.resize(tagSize, false);

		for (const auto& element : right)
		{
			tagElements[setting.TagtoTagID(element)] = true;
		}

		return *this;
	}
}