
#include "EUIEditBox.h"
#include "native/win/WinEditBox.h"

EUIEditBox::EUIEditBox(EUIWidget* prnt, const char* txt, int set_x, int set_y, int w, int h, InputType type): EUIWidget(prnt, txt)
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