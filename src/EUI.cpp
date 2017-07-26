
#include "EUI.h"
#include <windows.h>
#include "Native/win/WinTheme.h"
#include "Native/win/WinDialog.h"

std::vector<EUIWindow*> EUI::wnds;

#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

void EUI::Init(const char* themeName)
{
	InitCommonControls();

	theme = new WinTheme();
	ReloadTheme(themeName);
}

void EUI::ReloadTheme(const char* themeName)
{
	theme->ReadTheme(themeName);
}

int EUI::Run()
{
	MSG msg;

	while (wnds.size() > 0)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		for (int i = 0; i < wnds.size(); i++)
		{
			wnds[i]->Update();
		}
	}

	theme->Ulnload();

	return 0;
}

const char* EUI::OpenOpenDialog(char* extName, const char* ext)
{
#ifdef PLATFORM_WIN
	void* data = NULL;

	if (wnds.size() > 0)
	{
		data = wnds[0]->GetNative();
	}
	return WinDialog::FileDialog(data, extName, ext, true);
#endif
}

const char* EUI::OpenSaveDialog(char* extName, const char* ext)
{
#ifdef PLATFORM_WIN
	void* data = NULL;

	if (wnds.size() > 0)
	{
		data = wnds[0]->GetNative();
	}
	return WinDialog::FileDialog(data, extName, ext, false);
#endif
}