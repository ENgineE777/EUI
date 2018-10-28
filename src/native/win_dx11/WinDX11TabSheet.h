
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeTabSheet.h"

class EUITabSheet;
class WinTabPanel;

class WinDX11TabSheet : public NativeTabSheet
{
	WinDX11TabPanel* parent;

public:

	WinDX11TabSheet(EUIWidget* owner);
	~WinDX11TabSheet() override;

	EUITabSheet* Owner();

	void SetText(const char* text) override;
	void MakeAsCurrent() override;
};
#endif