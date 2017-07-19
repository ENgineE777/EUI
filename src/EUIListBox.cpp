
#include "EUIListBox.h"
#include "native/win/WinListBox.h"

EUIListBox::EUIListBox(EUIWidget* prnt, float set_x, float set_y, float set_w, float set_h) : EUIWidget(prnt, "")
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

int EUIListBox::GetSelectedIndex()
{
	return Native()->GetSelectedIndex();
}