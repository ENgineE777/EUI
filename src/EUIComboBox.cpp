
#include "EUIComboBox.h"
#include "native/win/WinComboBox.h"

EUIComboBox::EUIComboBox(EUIWidget* prnt, float set_x, float set_y, float w, float h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = w;
	height = h;

	nativeWidget = new WinComboBox(this);
}

EUIComboBox::~EUIComboBox()
{
}

NativeComboBox* EUIComboBox::Native()
{
	return (NativeComboBox*)nativeWidget;
}

void EUIComboBox::ClearList()
{
	Native()->ClearList();
}

void EUIComboBox::AddItem(const char* str)
{
	Native()->AddItem(str);
}

void EUIComboBox::SetCurString(int index)
{
	Native()->SetCurString(index);
}

void EUIComboBox::SetCurString(const char* str)
{
	Native()->SetCurString(str);
}

const char* EUIComboBox::GetCurString()
{
	return Native()->GetCurString();
}

int EUIComboBox::GetCurStringIndex()
{
	return Native()->GetCurStringIndex();
}