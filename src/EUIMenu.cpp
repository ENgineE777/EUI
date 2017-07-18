
#include "EUIMenu.h"
#include "native/win/WinMenu.h"

EUIMenu::EUIMenu(EUIWidget* prnt) : EUIWidget(0, prnt, "")
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

void EUIMenu::AddItem(int id, const char* name)
{
	Native()->AddItem(id, name);
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