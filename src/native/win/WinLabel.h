
#pragma once

#include "native/NativeLabel.h"

#ifdef PLATFORM_WIN

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
#endif
