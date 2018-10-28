
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeTabPanel.h"

class EUITabPanel;

class WinDX11TabPanel : public NativeTabPanel
{
	friend class WinDX11TabSheet;
	
	void AddTab(const char* txt, HWND hnd);
	void SetTabName(int index, const char* name);
	void ShowTab(int index, bool show);
	void SetCurrentTab(int index);

	int curTab;

public:

	WinDX11TabPanel(EUIWidget* owner);
	virtual ~WinDX11TabPanel();

	EUITabPanel* Owner();

	void Resize() override;

	int  GetCurrentTabIndex() override;
	void DeleteTab(int index) override;
	void ClearTabs() override;
};
#endif
