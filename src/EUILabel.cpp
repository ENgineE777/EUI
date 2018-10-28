
#include "EUILabel.h"

#ifdef PLATFORM_WIN
#include "native/win/WinLabel.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11Label.h"
#endif

EUILabel::EUILabel(EUIWidget* prnt, const char* txt, int set_x, int set_y, int set_w, int set_h) : EUIWidget(prnt, txt)
{
	bck_use = false;
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

#ifdef PLATFORM_WIN
	nativeWidget = new WinLabel(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11Label(this);
#endif
}

EUILabel::~EUILabel()
{
}

NativeLabel* EUILabel::Native()
{
	return (NativeLabel*)nativeWidget;
}

void EUILabel::SetText(const char* txt)
{
	EUIWidget::SetText(txt);
	nativeWidget->SetText(txt);
}

void EUILabel::SetBackgroundColor(bool use, int* color)
{
	bck_use = use;
	memcpy(bck_color, color, sizeof(int) * 3);
	nativeWidget->Redraw();
}