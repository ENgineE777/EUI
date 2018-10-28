
#include "WinDialog.h"
#include "windows.h"
#include <string>
#include "Commdlg.h"

#ifdef PLATFORM_WIN

const char* WinDialog::FileDialog(void* data, char* extName, const char* ext, bool open)
{
	char curDir[512];
	GetCurrentDirectoryA(512, curDir);

	OPENFILENAME ofn;

	static char fileName[512];

	char filter[512];
	strcpy(filter, extName);

	int index = (int)strlen(filter);

	filter[index + 1] = '*';
	filter[index + 2] = '.';

	if (ext)
	{
		strcpy(&filter[index + 3], ext);
	}
	else
	{
		strcpy(&filter[index + 3], "*");
	}

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = *((HWND*)data);
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 512;
	ofn.lpstrFilter = filter;
	ofn.lpstrDefExt = ext;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = curDir;
	ofn.Flags = OFN_PATHMUSTEXIST;
	
	if (open)
	{
		ofn.Flags |= OFN_FILEMUSTEXIST;
	}

	bool res = false;

	if (open)
	{
		res = GetOpenFileNameA(&ofn) ? true : false;
	}
	else
	{
		res = GetSaveFileNameA(&ofn) ? true : false;
	}

	SetCurrentDirectoryA(curDir);

	if (res)
	{
		return fileName;
	}

	return NULL;
}

bool WinDialog::ColorDialog(void* data, float* color)
{
	CHOOSECOLOR cs_color;

	static int colors[16];

	cs_color.lStructSize = sizeof(cs_color);
	cs_color.hwndOwner = *((HWND*)data);
	cs_color.hInstance = NULL;
	cs_color.Flags = CC_FULLOPEN | CC_ANYCOLOR | CC_RGBINIT;

	cs_color.rgbResult = (((uint8_t)(color[3] * 255) & 0xff) << 24) |
						 (((uint8_t)(color[2] * 255) & 0xff) << 16) |
						 (((uint8_t)(color[1] * 255) & 0xff) << 8) |
						 ((uint8_t)(color[0] * 255) & 0xff);

	cs_color.lpCustColors = (COLORREF*)&colors;
	cs_color.lCustData = 0;
	cs_color.lpfnHook = 0;
	cs_color.lpTemplateName = 0;

	if (ChooseColor(&cs_color))
	{
		color[3] = 1.0f;
		color[2] = ((cs_color.rgbResult >>16)&0xff) / 255.0f;
		color[1] = ((cs_color.rgbResult >>8)&0xff) / 255.0f;
		color[0] = ((cs_color.rgbResult)&0xff) / 255.0f;

		return true;
	}

	return false;
}
#endif