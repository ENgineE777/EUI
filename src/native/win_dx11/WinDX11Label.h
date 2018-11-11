
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeLabel.h"

class EUILabel;

class WinDX11Label : public NativeLabel
{
public:

	WinDX11Label(EUIWidget* owner);
	~WinDX11Label() override;

	EUILabel* Owner();

	void Draw() override;
};
#endif