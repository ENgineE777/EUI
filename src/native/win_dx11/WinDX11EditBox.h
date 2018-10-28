
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeEditBox.h"

class EUIEditBox;

class WinDX11EditBox : public NativeEditBox
{
public:

	static int timerID; 
	int cur_timerID;
	float time2callback = -1.0f;

	WinDX11EditBox(EUIWidget* owner);
	~WinDX11EditBox() override;

	EUIEditBox* Owner();

	void SetText(const char* txt) override;
	const char* GetText() override;
};
#endif