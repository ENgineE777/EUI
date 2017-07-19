
#pragma once

#include "EUIWidget.h"
#include "Native/NativeTabPanel.h"

class EUITabPanel : public EUIWidget
{
	friend class WinTabPanel;

	NativeTabPanel* Native();

public:

	EUITabPanel(EUIWidget* parent, float x, float y, float w, float h);
	virtual ~EUITabPanel();

	virtual void AddTab(const char* txt);
	virtual void SetTabName(int index, const char* name);
	virtual void DelTab(int index);
	virtual void ClearTabs();

	virtual void AddWidget2Tab(int index, EUIWidget* widget);

	virtual void SetCurrentTab(int index);
	virtual int  GetCurrentTab();
};
