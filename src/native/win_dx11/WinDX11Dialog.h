
#pragma once

#ifdef PLATFORM_WIN_DX11

class WinDX11Dialog
{
public:
	static const char* FileDialog(void* data, char* extName, const char* ext, bool open);
	static bool ColorDialog(void* data, float* color);
};
#endif