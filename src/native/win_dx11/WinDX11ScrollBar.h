
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeScrollBar.h"

class EUIScrollBar;

class WinDX11ScrollBar : public NativeScrollBar
{

public:

	WinDX11ScrollBar(EUIWidget* owner);
	virtual ~WinDX11ScrollBar();

	EUIScrollBar* Owner();

	void SetPosition(int pos) override;
	void SetLimitImpl();
	void SetLimit() override;
};
#endif