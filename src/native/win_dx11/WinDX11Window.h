
#pragma once

#include "native/NativeWindow.h"

#ifdef PLATFORM_WIN_DX11

class EUIWindow;

class WinDX11Window : public NativeWindow
{
	friend class WinDX11Menu;
	friend class WinMenu;
	friend class WinDX11Widget;
	friend class WinDX11EditBox;

	static bool wndClassReg;
	bool active;
	int flag;

	int hack_wnd_width;
	int hack_wnd_height;

	bool need_strict_size;
	bool is_minimazed;

	HWND handle = 0;
	bool skip_on_char = false;

	WinDX11Widget* left_moused = nullptr;

	class WinDX11Menu* menu_wiget = nullptr;
	class WinDX11Menu* popup_menu_wiget = nullptr;

	WinDX11Theme::WindowData window_data;

	int ms_x = 0;
	int ms_y = 0;

public:
	WinDX11Window(EUIWidget* owner, const char* icon, EUIWindow::Style style, bool adjust);
	~WinDX11Window() override;

	EUIWindow* Owner();

	void* GetNative() override;

	void Draw() override;

	static  LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT Proc( HWND hwnd, long msg, WPARAM wParam, LPARAM lParam );

	void GetMouseOver(WinDX11Widget* widget, int ms_x, int ms_y);

	void Show(bool set) override;
	void Enable(bool set) override;
	void SetPos(int set_x, int set_y) override;
	void SetSize(int w, int h) override;

	void Maximaze() override;
	void SetAtScreenCenter() override;
	void Close() override;
};
#endif
