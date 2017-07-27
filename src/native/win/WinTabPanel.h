
#pragma once

#include "WinWidget.h"
#include "native/NativeTabPanel.h"

class EUITabPanel;

class WinTabPanel : public NativeTabPanel
{
	friend class WinTabSheet;
	
	void AddTab(const char* txt, HWND hnd);
	void SetTabName(int index, const char* name);
	void ShowTab(int index, bool show);
	void SetCurrentTab(int index);

	int curTab;

public:

	WinTabPanel(EUIWidget* owner);
	virtual ~WinTabPanel();

	EUITabPanel* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	virtual void Resize();

	virtual int  GetCurrentTabIndex();
	virtual void DeleteTab(int index);
	virtual void ClearTabs();
};
