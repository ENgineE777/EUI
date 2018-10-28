
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeLayout.h"

class EUIButton;

class WinDX11Layout : public NativeLayout
{
public:

	WinDX11Layout(EUIWidget* owner);
	~WinDX11Layout() override;

	EUILayout* Owner();

	void Resize() override;
};
#endif