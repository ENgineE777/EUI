
#pragma once

#ifdef PLATFORM_WIN
#include "native/win/WinWidget.h"
#else
#include "NativeWidget.h"
#define WinWidget NativeWidget
#endif

class NativeTabPanel : public WinWidget
{
public:

	NativeTabPanel(EUIWidget* owner) : WinWidget(owner) {};

	virtual void AddTab(const char* txt) = 0;
	virtual void SetTabName(int index, const char* name) = 0;
	virtual void DelTab(int index) = 0;
	virtual void ClearTabs() = 0;

	virtual void AddWidget2Tab(int index, EUIWidget* widget) = 0;

	virtual void SetCurrentTab(int index) = 0;
	virtual int  GetCurrentTab() = 0;
};
