
#pragma once

#include "WinWidget.h"
#include "native/NativeTabPanel.h"

class EUITabPanel;

class WinTabPanel : public NativeTabPanel
{
	void ShowTab(int index, bool show);
	std::vector<HWND> tabs;
	int curTab;

public:

	WinTabPanel(EUIWidget* owner);
	virtual ~WinTabPanel();

	EUITabPanel* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	virtual void Resize();

	virtual void AddTab(const char* txt);
	virtual void SetTabName(int index, const char* name);
	virtual void DelTab(int index);
	virtual void ClearTabs();

	virtual void AddWidget2Tab(int index, EUIWidget* widget);

	virtual void SetCurrentTab(int index);
	virtual int  GetCurrentTab();
};
