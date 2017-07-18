
#pragma once

#include "WinWidget.h"
#include "native/NativeEditBox.h"

class EUIEditBox;

class WinEditBox : public NativeEditBox
{
public:
	WinEditBox(EUIWidget* owner);
	virtual ~WinEditBox();

	EUIEditBox* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	virtual void SetText(const char* txt);
	virtual const char* GetText();
};
