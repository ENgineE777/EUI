
#pragma once

#include "EUIWidget.h"
#include "Native/NativeListBox.h"

class EUIListBox : public EUIWidget
{
	friend class WinListBox;

	NativeListBox* Native();

public:

	EUIListBox(int id, EUIWidget* parent, float x, float y, float w, float h);
	virtual ~EUIListBox();

	virtual void ClearList();
	virtual void AddItem(const char* str, void* data);
	virtual int  GetSelectedIndex();
};
