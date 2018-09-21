
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

	void SetText(const char* txt) override;
	void Draw() override;
};
