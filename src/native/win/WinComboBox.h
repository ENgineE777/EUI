
#pragma once

#include "WinWidget.h"
#include "native/NativeLabel.h"

class EUIComboBox;

class WinComboBox : public NativeComboBox
{

public:

	WinComboBox(EUIWidget* owner);
	virtual ~WinComboBox();

	EUIComboBox* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);

	virtual void ClearList();
	virtual void AddItem(const char* str);
	virtual void SetCurString(int index);
	virtual void SetCurString(const char* str);
	virtual int  GetCurString();
};
