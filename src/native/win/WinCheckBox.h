
#pragma once

#include "native/NativeCheckBox.h"

#ifdef PLATFORM_WIN

class EUICheckBox;

class WinCheckBox : public NativeCheckBox
{
public:
	
	WinCheckBox(EUIWidget* owner);
	virtual ~WinCheckBox();

	EUICheckBox* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	virtual void SetText(const char* txt);
	virtual void SetChecked(bool set);
	virtual void Draw();
};
#endif