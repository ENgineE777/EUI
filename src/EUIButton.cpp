
#include "EUIButton.h"

#ifdef PLATFORM_WIN
#include "native/win/WinButton.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11Button.h"
#endif

EUIButton::EUIButton(EUIWidget* prnt, const char* txt, int set_x, int set_y, int w, int h) : EUIWidget(prnt, txt)
{
	x = set_x;
	y = set_y;
	width = w;
	height = h;

	is_pushed = false;
	is_pushable = false;
	is_howered = false;

#ifdef PLATFORM_WIN
	nativeWidget = new WinButton(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11Button(this);
#endif
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
