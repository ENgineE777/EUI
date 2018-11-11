
#include "EUIEditBox.h"

#ifdef PLATFORM_WIN
#include "native/win/WinEditBox.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11EditBox.h"
#endif

EUIEditBox::EUIEditBox(EUIWidget* prnt, const char* txt, int set_x, int set_y, int w, int h, InputType type): EUIWidget(prnt, txt)
{
	x = set_x;
	y = set_y;
	width = w;
	height = h;

	inputType = type;

#ifdef PLATFORM_WIN
	nativeWidget = new WinEditBox(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11EditBox(this);
#endif
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
	sprintf(str, "%.5f", value);

	if (strstr(str, "."))
	{
		int index = (int)strlen(str) - 1;

		while (str[index] == '0')
		{
			str[index] = 0;
			index--;
		}

		if (str[index] == '.')
		{
			str[index + 1] = '0';
			str[index + 2] = 0;
		}
	}

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