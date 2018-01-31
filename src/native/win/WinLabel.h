
#pragma once

#include "WinWidget.h"
#include "native/NativeLabel.h"

class EUILabel;

class WinLabel : public NativeLabel
{
public:

	WinLabel(EUIWidget* owner);
	virtual ~WinLabel();

	EUILabel* Owner();

	virtual void SetText(const char* txt);
	virtual void Draw();
};
