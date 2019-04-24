
#pragma once

#define NOMINMAX
#include "windows.h"
#include "windowsx.h"
#include "CommCtrl.h"
#include <vector>
#include <string>

#include "Native/NativeWidget.h"
#include "WinDX11Theme.h"

#ifdef PLATFORM_WIN_DX11

class WinDX11Widget : public NativeWidget
{
public:

	static int timerID;
	int cur_timerID = 0;

	bool is_focused = false;
	bool is_howered = false;
	bool mouse_pressed = false;
	int global_x = 0;
	int global_y = 0;
	bool drag_on = false;
	WinDX11Widget* focused_widget = nullptr;
	WinDX11Widget* over_widget = nullptr;
	WinDX11Widget* mouse_over = nullptr;

	WinDX11Widget(EUIWidget* owner);
	virtual ~WinDX11Widget();

	void SetTimer(int tick);

	virtual void* GetNative();

	virtual void Show(bool set);
	virtual void Enable(bool set);
	virtual void SetPos(int set_x, int set_y);
	virtual void SetSize(int w, int h);

	virtual void SetText(const char* txt);

	virtual void Release();

	virtual void Draw();
	virtual void Redraw();

	virtual void CaptureMouse();
	virtual void ReleaseMouse();

	virtual void Resize();

	virtual void NotifyMouseOver();
	virtual void OnMouseLeave();

	virtual void OnFocusLost();

	virtual void SetFocused();
	virtual bool IsFocused();

	void GetMousePos(int& x, int& y) override;

	virtual bool IsTreeView();

	virtual void OnTextCopy();
	virtual void OnTextPaste();

	virtual void OnKeyDown(int key);
	virtual void OnCharDown(int key);
	virtual void OnTimer();
	virtual bool IsHitted(int ms_x, int ms_y);
	virtual void OnMouseMove(int ms_x, int ms_y);
	virtual void OnMouseWheel(int delta);
	virtual void OnLeftMouseDown(int ms_x, int ms_y);
	virtual void OnLeftMouseUp(int ms_x, int ms_y);
	virtual void OnRightMouseUp(int ms_x, int ms_y);

	void CalcGlopalPos();
};
#endif