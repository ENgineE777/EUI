
#pragma once

#include "json/JSONParser.h"
#include <string>
#include <map>

struct UTFConv
{
	static std::map<wchar_t, int> upper2lower;
	static std::map<wchar_t, int> lower2upper;
	static void Init(JSONParser* reader);
	static bool CompareABC(const char* str1, const char* str2);
	static bool BuildUtf16fromUtf8(char c, int& bytes, int& w);
	static void BuildUtf8fromUtf16(int c, std::string& dest);
	static void UTF8to16(std::wstring& dest, const char* src);
	static void UTF16to8(std::string& dest, const wchar_t* src);
};
