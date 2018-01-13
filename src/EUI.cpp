
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

		for (int i = 0; i < (int)wnds.size(); i++)
		{
			wnds[i]->Update();
		}
	}

	theme->Ulnload();

	return 0;
}

const char* EUI::OpenOpenDialog(void* parent, char* extName, const char* ext)
{
#ifdef PLATFORM_WIN
	return WinDialog::FileDialog(parent, extName, ext, true);
#endif
}

const char* EUI::OpenSaveDialog(void* parent, char* extName, const char* ext)
{
#ifdef PLATFORM_WIN
	return WinDialog::FileDialog(parent, extName, ext, false);
#endif
}

bool EUI::OpenColorDialog(void* parent, float* color)
{
#ifdef PLATFORM_WIN
	return WinDialog::ColorDialog(parent, color);
#endif
}