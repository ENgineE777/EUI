
#pragma once

#include "WinWidget.h"
#include "native/NativeMenu.h"

class EUIMenu;

class WinMenu : public NativeMenu
{
	HMENU menu;

	int cur_depth;
	HMENU depth_menu[16];

	std::string sub_menu_name;

public:
	
	WinMenu(EUIWidget* owner);
	virtual ~WinMenu();

	EUIMenu* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	virtual void Show(bool set);

	void AddItem(int id, const char* name);
	void AddSeparator();
	void StartSubMenu(const char* name);
	void EndSubMenu();
};
