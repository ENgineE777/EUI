
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeButton.h"

class EUIButton;

class WinDX11Button : public NativeButton
{
	bool is_howered;

public:

	WinDX11Button(EUIWidget* owner);
	~WinDX11Button() override;

	EUIButton* Owner();

	void Enable(bool set) override;

	void SetImage(int img, const char* image_name) override;
	void SetText(const char* txt) override;

	void NotifyMouseOver() override;
	void OnMouseLeave() override;

	void Draw() override;
};
#endif
