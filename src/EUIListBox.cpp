
#include "EUIListBox.h"
#include "native/win/WinListBox.h"

EUIListBox::EUIListBox(EUIWidget* prnt, int set_x, int set_y, int set_w, int set_h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

	nativeWidget = new WinListBox(this);
}

EUIListBox::~EUIListBox()
{
}

NativeListBox* EUIListBox::Native()
{
	return (NativeListBox*)nativeWidget;
}

void EUIListBox::ClearList()
{
	Native()->ClearList();
}

void EUIListBox::AddItem(const char* str, void* data)
{
	Native()->AddItem(str, data);
}

void EUIListBox::ChangeItemNameByIndex(const char* str, int index)
{
	Native()->ChangeItemNameByIndex(str, index);
}

void EUIListBox::ChangeItemNameByData(const char* str, void* data)
{
	Native()->ChangeItemNameByData(str, data);
}

int EUIListBox::GetSelectedItemIndex()
{
	return Native()->GetSelectedItemIndex();
}

void* EUIListBox::GetSelectedItemData()
{
	return Native()->GetSelectedItemData();
}

void EUIListBox::SelectItemByIndex(int index)
{
	Native()->SelectItemByIndex(index);
}

void EUIListBox::SelectItemByData(void* data)
{
	Native()->SelectItemByData(data);
}

void EUIListBox::DeleteItemByIndex(int index)
{
	Native()->DeleteItemByIndex(index);
}

void EUIListBox::DeleteItemByData(void* data)
{
	Native()->DeleteItemByData(data);
}