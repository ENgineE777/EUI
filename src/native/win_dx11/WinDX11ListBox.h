
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeListBox.h"

class EUIListBox;

class WinDX11ListBox : public NativeListBox
{
	int FindIndexByData(void* data);
public:

	WinDX11ListBox(EUIWidget* owner, bool abs_sort);
	~WinDX11ListBox() override;

	EUIListBox* Owner();

	void  ClearList() override;
	void  AddItem(const char* str, void* data) override;
	void  ChangeItemNameByIndex(const char* str, int index) override;
	void  ChangeItemNameByData(const char* str, void* data) override;
	int   GetSelectedItemIndex() override;
	void* GetSelectedItemData() override;
	void  SelectItemByIndex(int index) override;
	void  SelectItemByData(void* data) override;
	void  DeleteItemByIndex(int index) override;
	void  DeleteItemByData(void* data) override;
};
#endif
