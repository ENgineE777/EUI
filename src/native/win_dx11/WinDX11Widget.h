
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
protected:

	static WinDX11Widget* mouse_over;

public:

	WinDX11Widget* menu_wiget = nullptr;

	WinDX11Widget(EUIWidget* owner);
	virtual ~WinDX11Widget();

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

	virtual void SetFocused();
	virtual bool IsFocused();

	bool IsHoveredByMouse() override;
	WinDX11Widget* GetHoveredWidget();
	void GetMousePos(int& x, int& y) override;

	virtual bool IsTreeView();
};
#endif