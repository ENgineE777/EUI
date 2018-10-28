
#include "EUIMenu.h"
#include "WinDX11Menu.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Menu::WinDX11Menu(EUIWidget* owner) : NativeMenu(owner)
{
	menu = 0;

	cur_depth = 0;
	depth_menu[cur_depth] = menu;
}

WinDX11Menu::~WinDX11Menu()
{

}

EUIMenu* WinDX11Menu::Owner()
{
	return (EUIMenu*)owner;
}

bool WinDX11Menu::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_COMMAND)
	{
		if (Owner()->listener)
		{
			Owner()->listener->OnMenuItem(Owner(), LOWORD(wParam));
		}
	}

	return true;
}

void WinDX11Menu::Show(bool set)
{
	if (set)
	{
		//SetMenu(((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), menu );
	}
	else
	{
		//SetMenu(((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), nullptr );
	}

	NativeMenu::Show(set);
}

void WinDX11Menu::AttachToWidget(EUIWidget* widget)
{
	//((WinWidget*)widget->nativeWidget)->menu_wiget = this;
	//SetMenu(((WinWidget*)widget->nativeWidget)->GetHandle(), menu);
}

void WinDX11Menu::ShowAsPopup(EUIWidget* parent, int x, int y)
{
	//((WinWidget*)parent->nativeWidget)->menu_wiget = this;
	HWND handle;// = ((WinWidget*)parent->nativeWidget)->GetHandle();

	POINT p;
	p.x = x;
	p.y = y;
	ClientToScreen(handle, &p);
	SetForegroundWindow(handle);
	TrackPopupMenuEx(menu, TPM_TOPALIGN | TPM_LEFTALIGN, p.x, p.y, handle, 0);
}

void WinDX11Menu::StartMenu(bool is_popup)
{
	if (menu)
	{
		DestroyMenu(menu);
	}

	menu = is_popup ? CreatePopupMenu() : CreateMenu();

	cur_depth = 0;
	depth_menu[cur_depth] = menu;
}

void WinDX11Menu::AddItem(int id, const char* name, bool enabled)
{
	int flag = enabled ? 0 : MF_DISABLED;
	AppendMenu(depth_menu[cur_depth], MF_STRING | flag, id, name);
}

void WinDX11Menu::AddSeparator()
{
	AppendMenu(depth_menu[cur_depth], MF_SEPARATOR, 7, 0);
}

void WinDX11Menu::StartSubMenu(const char* name)
{
	cur_depth++;
	depth_menu[cur_depth] = CreatePopupMenu();
	sub_menu_name[cur_depth] = name;
}

void WinDX11Menu::EndSubMenu()
{
	if (cur_depth == 0) return;

	AppendMenu(depth_menu[cur_depth-1], MF_POPUP, (UINT_PTR)depth_menu[cur_depth], sub_menu_name[cur_depth].c_str());
	cur_depth--;
}
#endif