
#pragma once

#include "native/NativeCheckBox.h"

#ifdef PLATFORM_WIN_DX11
class EUICheckBox;

class WinDX11CheckBox : public NativeCheckBox
{
public:
	
	WinDX11CheckBox(EUIWidget* owner);
	virtual ~WinDX11CheckBox();

	EUICheckBox* Owner();

	void SetText(const char* txt) override;
	void SetChecked(bool set) override;
	void Draw() override;

	void OnMouseMove(int ms_x, int ms_y) override;
	void OnLeftMouseUp(int ms_x, int ms_y) override;
};
#endif