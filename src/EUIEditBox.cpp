
#include "EUIEditBox.h"
#include "native/win/WinEditBox.h"

EUIEditBox::EUIEditBox(int set_id, EUIWidget* prnt, const char* txt, float set_x, float set_y, float w, float h, InputType type): EUIWidget(set_id, prnt, txt)
{
	x = set_x;
	y = set_y;
	width = w;
	height = h;

	inputType = type;

	nativeWidget = new WinEditBox(this);
}

EUIEditBox::~EUIEditBox()
{
}

NativeEditBox* EUIEditBox::Native()
{
	return (NativeEditBox*)nativeWidget;
}

void EUIEditBox::SetText(const char* txt)
{
	EUIWidget::SetText(txt);
	nativeWidget->SetText(txt);
}

const char* EUIEditBox::GetText()
{
	text = Native()->GetText();

	return EUIWidget::GetText();
}