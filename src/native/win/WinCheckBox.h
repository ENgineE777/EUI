
#pragma once

#include "WinWidget.h"
#include "native/NativeCheckBox.h"

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
