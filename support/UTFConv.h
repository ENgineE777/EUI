
#pragma once

#include <string>

class UTFConv
{
public:
	static void UTF8to16(std::wstring& dest, const char* src);
	static void UTF16to8(std::string& dest, const wchar_t* src);
};
