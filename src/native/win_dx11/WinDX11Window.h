
#pragma once

#include "native/NativeWindow.h"

#ifdef PLATFORM_WIN_DX11

class EUIWindow;

class WinDX11Window : public NativeWindow
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
	WinDX11Window(EUIWidget* owner, EUIWindow::Style style, bool adjust);
	~WinDX11Window() override;

	EUIWindow* Owner();

	static  LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT Proc( HWND hwnd, long msg, WPARAM wParam, LPARAM lParam );

	void Maximaze() override;
	void SetAtScreenCenter() override;
	void Close() override;
};
#endif
