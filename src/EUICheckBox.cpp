
#include "EUICheckBox.h"
#include "native/win/WinCheckBox.h"

EUICheckBox::EUICheckBox(EUIWidget* prnt, const char* txt, float set_x, float set_y, float w, float h) : EUIWidget(prnt, txt)
{	
	x = set_x;
	y = set_y;
	width = w;
	height = h;

	checked = false;

	nativeWidget = new WinCheckBox(this);
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