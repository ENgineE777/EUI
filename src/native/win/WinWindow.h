
#pragma once

#include "native/NativeWindow.h"
#include "WinWindow.h"

class EUIWindow;

class WinWindow : public NativeWindow
{
	friend class WinMenu;

	static bool wndClassReg;
	bool active;
	int flag;

	int hack_wnd_width;
	int hack_wnd_height;

	bool need_strict_size;
	bool is_minimazed;

public:
	WinWindow(EUIWidget* owner, bool popup, bool adjust);
	virtual ~WinWindow();

	EUIWindow* Owner();

	static  LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT Proc( HWND hwnd, long msg, WPARAM wParam, LPARAM lParam );

	virtual void Maximaze();
	virtual void SetAtScreenCenter();
};
