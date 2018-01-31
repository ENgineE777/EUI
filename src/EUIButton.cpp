
#include "EUIButton.h"
#include "native/win/WinButton.h"

EUIButton::EUIButton(EUIWidget* prnt, const char* txt, int set_x, int set_y, int w, int h) : EUIWidget(prnt, txt)
{
	x = set_x;
	y = set_y;
	width = w;
	height = h;

	is_pushed = false;
	is_pushable = false;
	is_howered = false;

	nativeWidget = new WinButton(this);
}

EUIButton::~EUIButton()
{
}

NativeButton* EUIButton::Native()
{
	return (NativeButton*)nativeWidget;
}

void EUIButton::SetText(const char* txt)
{
	EUIWidget::SetText(txt);
	nativeWidget->SetText(txt);
}

void EUIButton::SetImage(Image img, const char* image_name)
{
	Native()->SetImage(img, image_name);
}

bool EUIButton::IsPushable()
{
	return is_pushable;
}

void EUIButton::SetPushable(bool set)
{
	is_pushable = set;
	
	if (!is_pushable) is_pushed = false;

	Native()->Redraw();
}

bool EUIButton::IsPushed()
{
	return is_pushed;
}

void EUIButton::SetPushed(bool set)
{
	if (!is_pushable) return;

	is_pushed = set;

	Native()->Redraw();
}
