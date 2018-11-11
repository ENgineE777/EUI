
#include "UTFConv.h"

std::map<wchar_t, int> UTFConv::upper2lower;
std::map<wchar_t, int> UTFConv::lower2upper;

void UTFConv::Init(JSONParser& reader)
{
	std::string str;

	while (reader.EnterBlock("table"))
	{
		reader.Read("lo", str);
		unsigned int lo = std::stoul(str.c_str(), nullptr, 16);

		reader.Read("hi", str);
		unsigned int hi = std::stoul(str.c_str(), nullptr, 16);

		upper2lower[lo] = hi;
		lower2upper[hi] = lo;

		reader.LeaveBlock();
	}
}

bool UTFConv::CompareABC(const char* str1, const char* str2)
{
	int bytes = 0;

	int w1 = 0;
	int len1 = (int)strlen(str1);
	int index1 = 0;

	int w2 = 0;
	int len2 = (int)strlen(str2);
	int index2 = 0;

	std::map<wchar_t, int>::iterator it;

	bool finished = false;
	int stage = 0;

	while (!finished)
	{
		if (stage == 0)
		{
			if (BuildUtf16fromUtf8(str1[index1], bytes, w1))
			{
				it = upper2lower.find(w1);

				if (it != upper2lower.end())
				{
					w1 = it->second;
				}

				stage++;
			}
			else
			{
				if (index1 + 1 >= len1)
				{
					return true;
				}
			}

			index1++;
		}
		else
			if (stage == 1)
			{
				if (BuildUtf16fromUtf8(str2[index2], bytes, w2))
				{
					it = upper2lower.find(w2);

					if (it != upper2lower.end())
					{
						w2 = it->second;
					}

					stage++;
				}
				else
				{
					if (index2 + 1 >= len2)
					{
						break;
					}
				}

				index2++;
			}
			else
			{
				if (w1 != w2)
				{
					if (w1 > 128 && w2 < 128)
					{
						return true;
					}

					if (w2 > 128 && w1 < 128)
					{
						return false;
					}

					return (w1 < w2);
				}

				if (index1 >= len1 && index2 < len2)
				{
					return true;
				}

				if (index2 >= len2 && index1 < len1)
				{
					break;
				}

				stage = 0;
			}
	}

	return false;
}

bool UTFConv::BuildUtf16fromUtf8(char c, int& bytes, int& w)
{
	if (c <= 0x7f)
	{
		if (bytes)
		{
			bytes = 0;
		}

		w = (wchar_t)c;

		return true;
	}
	else
	if (c <= 0xbf)
	{
	if (bytes)
		{
			w = ((w << 6) | (c & 0x3f));
			bytes--;

			if (bytes == 0)
			{
				return true;
			}
		}
	}
	else
	if (c <= 0xdf)
	{
		bytes = 1;
		w = c & 0x1f;
	}
	else
	if (c <= 0xef)
	{
		bytes = 2;
		w = c & 0x0f;
	}
	else
	if (c <= 0xf7)
	{
		bytes = 3;
		w = c & 0x07;
	}
	else
	{
		bytes = 0;
	}

	return false;
}

void UTFConv::BuildUtf8fromUtf16(int c, std::string& dest)
{
	dest.clear();

	if (c < (1 << 7))
	{
		dest += c;
	}
	else
	if (c < (1 << 11))
	{
		dest += ((c >> 6) | 0xcC0);
		dest += ((c & 0x3F) | 0x80);
	}
	else
	if (c < (1 << 16))
	{
		dest += ((c >> 12) | 0xE0);
		dest += (((c >> 6) & 0x3F) | 0x80);
		dest += ((c & 0x3F) | 0x80);
	}
	else
	if (c < (1 << 21))
	{
		dest += ((c >> 18) | 0xE0);
		dest += (((c >> 12) & 0x3F) | 0x80);
		dest += (((c >> 6) & 0x3F) | 0x80);
		dest += ((c & 0x3F) | 0x80);
	}
}

void UTFConv::UTF8to16(std::wstring& dest, const char* src)
{
	dest.clear();
	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = L'?';

	size_t count = (size_t)strlen(src);

	for (size_t i = 0; i < count; i++)
	{
		char c = src[i];

		if (c <= 0x7f)
		{
			if (bytes)
			{
				dest.push_back(err);
				bytes = 0;
			}

			dest.push_back((wchar_t)c);
		}
		else
		if (c <= 0xbf)
		{
			if (bytes)
			{
				w = ((w << 6) | (c & 0x3f));
				bytes--;
				if (bytes == 0)
				{
					dest.push_back(w);
				}
			}
			else
			{
				dest.push_back(err);
			}
		}
		else
		if (c <= 0xdf)
		{
			bytes = 1;
			w = c & 0x1f;
		}
		else
		if (c <= 0xef)
		{
			bytes = 2;
			w = c & 0x0f;
		}
		else
		if (c <= 0xf7)
		{
			bytes = 3;
			w = c & 0x07;
		}
		else
		{
			dest.push_back(err);
			bytes = 0;
		}
	}

	if (bytes) dest.push_back(err);
}

void UTFConv::UTF16to8(std::string& dest, const wchar_t* src)
{
	dest.clear();

	size_t len = wcslen(src);

	for (size_t i = 0; i < len; i++)
	{
		int c = src[i];

		if (c < (1 << 7))
		{
			dest += c;
		}
		else
		if (c < (1 << 11))
		{
			dest += ((c >> 6) | 0xcC0);
			dest += ((c & 0x3F) | 0x80);
		}
		else
		if (c < (1 << 16))
		{
			dest += ((c >> 12) | 0xE0);
			dest += (((c >> 6) & 0x3F) | 0x80);
			dest += ((c & 0x3F) | 0x80);
		}
		else
		if (c < (1 << 21))
		{
			dest += ((c >> 18) | 0xE0);
			dest += (((c >> 12) & 0x3F) | 0x80);
			dest += (((c >> 6) & 0x3F) | 0x80);
			dest += ((c & 0x3F) | 0x80);
		}
	}
}

