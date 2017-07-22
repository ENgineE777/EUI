
#pragma once

#include "EUIWidget.h"
#include "Native/NativeListBox.h"

class EUIListBox : public EUIWidget
{
	friend class WinListBox;

	NativeListBox* Native();

public:

	EUIListBox(EUIWidget* parent, float x, float y, float w, float h);
	virtual ~EUIListBox();

	virtual void  ClearList();
	virtual void  AddItem(const char* str, void* data);
	virtual int   GetSelectedItemIndex();
	virtual void* GetSelectedItemData();
	virtual void  SelectItemByIndex(int index);
	virtual void  SelectItemByData(void* data);
	virtual void  DeleteItemByIndex(int index);
	virtual void  DeleteItemByData(void* data);
};
