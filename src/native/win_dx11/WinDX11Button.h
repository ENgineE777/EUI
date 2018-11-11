
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeButton.h"

class EUIButton;

class WinDX11Button : public NativeButton
{
public:

	WinDX11Button(EUIWidget* owner);
	~WinDX11Button() override;

	EUIButton* Owner();

	void SetImage(int img, const char* image_name) override;

	void Draw() override;
	void OnLeftMouseUp(int ms_x, int ms_y) override;
};
#endif
