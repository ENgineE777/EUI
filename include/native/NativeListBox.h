
#pragma once

#ifdef PLATFORM_WIN
#include "native/win/WinWidget.h"
#else
#include "NativeWidget.h"
#define WinWidget NativeWidget
#endif

class NativeListBox : public WinWidget
{
public:

	NativeListBox(EUIWidget* owner) : WinWidget(owner) {};

	virtual void ClearList() = 0;
	virtual void AddItem(const char* str, void* data) = 0;
	virtual int  GetSelectedIndex() = 0;
};
