
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeEditBox.h"

class EUIEditBox;

class WinDX11EditBox : public NativeEditBox
{
	int start_sel = -1;
	int start_sel_offset = 0;
	int offset = 0;
	float time2callback = -1.0f;

	void SetSel(int sel);

public:

	WinDX11EditBox(EUIWidget* owner);
	~WinDX11EditBox() override;

	EUIEditBox* Owner();

	void SetText(const char* txt) override;
	const char* GetText() override;
	void Draw() override;
	void OnKeyDown(int key) override;
	void OnCharDown(int key) override;
	void OnTimer();

	void  OnLeftMouseDown(int ms_x, int ms_y) override;
	void  OnLeftMouseUp(int ms_x, int ms_y) override;
};
#endif