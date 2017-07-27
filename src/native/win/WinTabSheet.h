
#pragma once

#include "WinWidget.h"
#include "native/NativeTabSheet.h"

class EUITabSheet;
class WinTabPanel;

class WinTabSheet : public NativeTabSheet
{
	WinTabPanel* parent;

public:

	WinTabSheet(EUIWidget* owner);
	virtual ~WinTabSheet();

	EUITabSheet* Owner();

	virtual void SetText(const char* text);
	virtual void MakeAsCurrent();
};
