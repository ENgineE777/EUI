
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

void EUIEditBox::SetText(int value)
{
	char str[128];
	sprintf(str, "%i", value);
	SetText(str);
}

void EUIEditBox::SetText(float value)
{
	char str[128];
	sprintf(str, "%4.3f", value);
	SetText(str);
}

const char* EUIEditBox::GetText()
{
	text = Native()->GetText();

	return EUIWidget::GetText();
}

int EUIEditBox::GetAsInt()
{
	text = Native()->GetText();
	int value = atoi(text.c_str());
	return value;
}

float EUIEditBox::GetAsFloat()
{
	text = Native()->GetText();
	float value = (float)atof(text.c_str());
	return value;
}