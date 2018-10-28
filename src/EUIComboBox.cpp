
#include "EUIComboBox.h"

#ifdef PLATFORM_WIN
#include "native/win/WinComboBox.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11ComboBox.h"
#endif

EUIComboBox::EUIComboBox(EUIWidget* prnt, int set_x, int set_y, int w, int h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = w;
	height = h;

#ifdef PLATFORM_WIN
	nativeWidget = new WinComboBox(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11ComboBox(this);
#endif
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