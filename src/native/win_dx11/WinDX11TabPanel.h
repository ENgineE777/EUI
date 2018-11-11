
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeTabPanel.h"

class EUITabPanel;

class WinDX11TabPanel : public NativeTabPanel
{
	friend class WinDX11TabSheet;
	
	void AddTab(const char* txt);
	void SetTabName(int index, const char* name);
	void ShowTab(int index, bool show);
	void SetCurrentTab(int index);

	int howeredTab = -1;
	int curTab = -1;

	std::vector<std::string> tab_names;

public:

	WinDX11TabPanel(EUIWidget* owner);
	virtual ~WinDX11TabPanel();

	EUITabPanel* Owner();

	void Draw() override;
	void Resize() override;

	int  GetCurrentTabIndex() override;
	void DeleteTab(int index) override;
	void ClearTabs() override;

	void OnMouseMove(int ms_x, int ms_y) override;
	void OnMouseLeave() override;
	void OnLeftMouseUp(int ms_x, int ms_y) override;
};
#endif
