
#include "EUIMenu.h"
#include "WinMenu.h"

WinMenu::WinMenu(EUIWidget* owner) : NativeMenu(owner)
{
	menu = 0;
	handle = 0;

	cur_depth = 0;
	depth_menu[cur_depth] = menu;
}

WinMenu::~WinMenu()
{

}

EUIMenu* WinMenu::Owner()
{
	return (EUIMenu*)owner;
}

bool WinMenu::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
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

void WinMenu::Show(bool set)
{
	if (set)
	{
		SetMenu(((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), menu );
	}
	else
	{
		SetMenu(((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), nullptr );
	}

	NativeMenu::Show(set);
}

void WinMenu::AttachToWidget(EUIWidget* widget)
{
	((WinWidget*)widget->nativeWidget)->menu_wiget = this;
	SetMenu(((WinWidget*)widget->nativeWidget)->GetHandle(), menu);
}

void WinMenu::ShowAsPopup(EUIWidget* parent, int x, int y)
{
	((WinWidget*)parent->nativeWidget)->menu_wiget = this;
	HWND handle = ((WinWidget*)parent->nativeWidget)->GetHandle();

	POINT p;
	p.x = x;
	p.y = y;
	ClientToScreen(handle, &p);
	SetForegroundWindow(handle);
	TrackPopupMenuEx(menu, TPM_TOPALIGN | TPM_LEFTALIGN, p.x, p.y, handle, 0);
}

void WinMenu::StartMenu(bool is_popup)
{
	if (menu)
	{
		DestroyMenu(menu);
	}

	menu = is_popup ? CreatePopupMenu() : CreateMenu();

	cur_depth = 0;
	depth_menu[cur_depth] = menu;
}

void WinMenu::AddItem(int id, const char* name)
{
	AppendMenu(depth_menu[cur_depth], MF_STRING, id, name);
}

void WinMenu::AddSeparator()
{
	AppendMenu(depth_menu[cur_depth], MF_SEPARATOR, 7, 0);
}

void WinMenu::StartSubMenu(const char* name)
{
	cur_depth++;
	depth_menu[cur_depth] = CreatePopupMenu();
	sub_menu_name[cur_depth] = name;
}

void WinMenu::EndSubMenu()
{
	if (cur_depth == 0) return;

	AppendMenu(depth_menu[cur_depth-1], MF_POPUP, (UINT_PTR)depth_menu[cur_depth], sub_menu_name[cur_depth].c_str());
	cur_depth--;
}