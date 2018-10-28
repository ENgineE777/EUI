
#pragma once

#include "native/NativeTabSheet.h"

#ifdef PLATFORM_WIN

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
#endif