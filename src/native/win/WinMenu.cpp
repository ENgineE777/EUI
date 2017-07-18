
#include "EUIMenu.h"
#include "WinMenu.h"

WinMenu::WinMenu(EUIWidget* owner) : NativeMenu(owner)
{
	menu = CreateMenu();
	SetMenu(((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), menu );

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
		if (Owner()->listiner)
		{
			Owner()->listiner->OnMenuItem(Owner(), LOWORD(wParam));
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
		SetMenu(((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), NULL );
	}

	NativeMenu::Show(set);
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

	sub_menu_name = name;
}

void WinMenu::EndSubMenu()
{
	if (cur_depth == 0) return;

	AppendMenu(depth_menu[cur_depth-1], MF_POPUP, (UINT)depth_menu[cur_depth], sub_menu_name.c_str());
	cur_depth--;
}