
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeScrollBar.h"

class EUIScrollBar;

class WinDX11ScrollBar : public NativeScrollBar
{
	int sel_elem = -1;
	int thumb_size = 0;
	int thumb_pos = 0;
	int thumb_max_pos = 0;
	int thumb_delta = 0;

public:

	WinDX11ScrollBar(EUIWidget* owner);
	virtual ~WinDX11ScrollBar();

	EUIScrollBar* Owner();

	void SetPosition(int pos) override;
	void SetLimitImpl();
	void SetLimit() override;

	void Draw() override;
	void OnMouseMove(int ms_x, int ms_y) override;
	void OnMouseLeave() override;
	void OnLeftMouseUp(int ms_x, int ms_y) override;
};
#endif