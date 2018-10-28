
#include "EUIMenu.h"

#ifdef PLATFORM_WIN
#include "native/win/WinMenu.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11Menu.h"
#endif

EUIMenu::EUIMenu() : EUIWidget(nullptr, "")
{
#ifdef PLATFORM_WIN
	nativeWidget = new WinMenu(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11Menu(this);
#endif
}

EUIMenu::~EUIMenu()
{
}

NativeMenu* EUIMenu::Native()
{
	return (NativeMenu*)nativeWidget;
}

void EUIMenu::AttachToWidget(EUIWidget* widget)
{
	Native()->AttachToWidget(widget);
}

void EUIMenu::ShowAsPopup(EUIWidget* parent, int x, int y)
{
	Native()->ShowAsPopup(parent, x, y);
}

void EUIMenu::StartMenu(bool is_popup)
{
	Native()->StartMenu(is_popup);
}

void EUIMenu::AddItem(int id, const char* name, bool enabled)
{
	Native()->AddItem(id, name, enabled);
}

void EUIMenu::AddSeparator()
{
	Native()->AddSeparator();
}

void EUIMenu::StartSubMenu(const char* name)
{
	Native()->StartSubMenu(name);
}

void EUIMenu::EndSubMenu()
{
	Native()->EndSubMenu();
}