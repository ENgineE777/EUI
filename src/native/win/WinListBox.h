
#pragma once

#include "WinWidget.h"
#include "native/NativeListBox.h"

class EUIListBox;

class WinListBox : public NativeListBox
{
	int FindIndexByData(void* data);
public:

	WinListBox(EUIWidget* owner, bool abs_sort);
	virtual ~WinListBox();

	EUIListBox* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);

	virtual void  ClearList();
	virtual void  AddItem(const char* str, void* data);
	virtual void  ChangeItemNameByIndex(const char* str, int index);
	virtual void  ChangeItemNameByData(const char* str, void* data);
	virtual int   GetSelectedItemIndex();
	virtual void* GetSelectedItemData();
	virtual void  SelectItemByIndex(int index);
	virtual void  SelectItemByData(void* data);
	virtual void  DeleteItemByIndex(int index);
	virtual void  DeleteItemByData(void* data);
};
