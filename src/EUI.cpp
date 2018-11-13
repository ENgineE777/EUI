
#include "EUI.h"
#include <windows.h>
#include "Native/win/WinTheme.h"
#include "Native/win/WinDialog.h"

#ifdef PLATFORM_WIN
#include "Native/win/WinTheme.h"
#include "Native/win/WinDialog.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "Native/win_dx11/WinDX11Theme.h"
#include "Native/win_dx11/WinDX11Dialog.h"
#endif

std::vector<EUIWindow*> EUI::wnds;

#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

const char* EUI::GetName()
{
#ifdef PLATFORM_WIN
	return "EUI_WinApi";
#endif
#ifdef PLATFORM_WIN_DX11
	return "EUI_DX11";
#endif
}

void EUI::Init(const char* theme_path, const char* theme_name)
{
	InitCommonControls();

#ifdef PLATFORM_WIN
	theme = new WinTheme();
#endif
#ifdef PLATFORM_WIN_DX11
	theme = new WinDX11Theme();
#endif

	theme->Init(theme_path, theme_name);
}

void* EUI::GetRenderDevice()
{
	return theme->GetRenderDevice();
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
			if (!wnds[i]->IsVisible() || !wnds[i]->IsEnabled())
			{
				continue;
			}

			wnds[i]->Update();
#ifdef PLATFORM_WIN_DX11
			((WinDX11Widget*)wnds[i]->nativeWidget)->Draw();
#endif
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

#ifdef PLATFORM_WIN_DX11
	return WinDX11Dialog::FileDialog(parent, extName, ext, true);
#endif
}

const char* EUI::OpenSaveDialog(void* parent, char* extName, const char* ext)
{
#ifdef PLATFORM_WIN
	return WinDialog::FileDialog(parent, extName, ext, false);
#endif

#ifdef PLATFORM_WIN_DX11
	return WinDX11Dialog::FileDialog(parent, extName, ext, false);
#endif
}

bool EUI::OpenColorDialog(void* parent, float* color)
{
#ifdef PLATFORM_WIN
	return WinDialog::ColorDialog(parent, color);
#endif

#ifdef PLATFORM_WIN_DX11
	return WinDX11Dialog::ColorDialog(parent, color);
#endif
}