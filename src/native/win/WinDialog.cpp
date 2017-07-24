
#include "windows.h"
#include <string>

const char* FileDialog(void* data, char* extName, const char* ext, bool open)
{
	char curDir[512];
	GetCurrentDirectoryA(512, curDir);

	OPENFILENAME ofn;

	static char fileName[512];
	char filter[128];
	sprintf(filter, "%s\0*.%s\0", extName, ext);

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
		res = GetOpenFileNameA(&ofn);
	}
	else
	{
		res = GetSaveFileNameA(&ofn);
	}

	SetCurrentDirectoryA(curDir);

	if (res)
	{
		return fileName;
	}

	return NULL;
}