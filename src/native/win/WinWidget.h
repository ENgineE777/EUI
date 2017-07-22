
#pragma once

#define NOMINMAX
#include "windows.h"
#include "windowsx.h"
#include "CommCtrl.h"
#include <vector>
#include <string>

#include "Native/NativeWidget.h"
#include "WinTheme.h"

class WinWidget : public NativeWidget
{
protected:

	HWND  handle;
	HWND  tooltip;
	TOOLINFO ti;

	bool tooltip_inited;

	int    processRes;
	static WinWidget* mouse_over;
	static int win_id;

public:
	WinWidget(EUIWidget* owner);
	virtual ~WinWidget();

	virtual HWND GetHandle();
	virtual void* GetNative();

	virtual void Show(bool set);
	virtual void Enable(bool set);
	virtual void SetPos(float set_x, float set_y);

	virtual void SetSize(float w, float h);

	virtual void SetText(const char* txt);

	virtual void Release();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	virtual void Draw();
	virtual void Redraw();

	virtual void Resize();

	virtual void NotifyMouseOver(WinWidget* widget);
	virtual void OnMouseLeave();


	virtual void SetFocused();
	virtual bool IsFocused();

	void CreateTooltip();

	bool IsHoveredByMouse();

	void MakeSubClassing();

	static LRESULT CALLBACK WinWidgetProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
};