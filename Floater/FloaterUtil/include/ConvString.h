#pragma once
#include <string>

namespace flt
{
	std::wstring convToWstring(const std::string& str);
	std::string convToString(const std::wstring& str);
}
