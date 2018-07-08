
#include "EUIMenu.h"
#include "native/win/WinMenu.h"

EUIMenu::EUIMenu() : EUIWidget(nullptr, "")
{
	nativeWidget = new WinMenu(this);
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