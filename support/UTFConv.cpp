
#include "UTFConv.h"

void UTFConv::UTF8to16(std::wstring& dest, const char* src)
{
	dest.clear();
	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = L'?';

	int count = strlen(src);

	for (int i = 0; i < count; i++)
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

	int len = wcslen(src);

	for (int i = 0; i < len; i++)
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

