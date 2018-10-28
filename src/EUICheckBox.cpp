
#include "EUICheckBox.h"

#ifdef PLATFORM_WIN
#include "native/win/WinCheckBox.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11CheckBox.h"
#endif

EUICheckBox::EUICheckBox(EUIWidget* prnt, const char* txt, int set_x, int set_y, int w, int h) : EUIWidget(prnt, txt)
{	
	x = set_x;
	y = set_y;
	width = w;
	height = h;

	checked = false;

#ifdef PLATFORM_WIN
	nativeWidget = new WinCheckBox(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11CheckBox(this);
#endif
}

EUICheckBox::~EUICheckBox()
{
}

NativeCheckBox* EUICheckBox::Native()
{
	return (NativeCheckBox*)nativeWidget;
}

void EUICheckBox::SetText(const char* txt)
{
	EUIWidget::SetText(txt);
}

void EUICheckBox::SetChecked(bool set)
{
	checked = set;
	Native()->SetChecked(checked);
}

bool EUICheckBox::IsChecked()
{
	return checked;
}