
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeMenu.h"

class EUIMenu;

class WinDX11Menu : public NativeMenu
{
	HMENU menu;

	int cur_depth;
	HMENU depth_menu[16];

	std::string sub_menu_name[16];

public:
	
	WinDX11Menu(EUIWidget* owner);
	virtual ~WinDX11Menu();

	EUIMenu* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	void Show(bool set) override;

	void AttachToWidget(EUIWidget* widget);
	void ShowAsPopup(EUIWidget* parent, int x, int y);

	void StartMenu(bool is_popup);
	void AddItem(int id, const char* name, bool enabled);
	void AddSeparator();
	void StartSubMenu(const char* name);
	void EndSubMenu();
};
#endif