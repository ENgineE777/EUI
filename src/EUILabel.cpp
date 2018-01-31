
#include "EUILabel.h"
#include "native/win/WinLabel.h"

EUILabel::EUILabel(EUIWidget* prnt, const char* txt, int set_x, int set_y, int set_w, int set_h) : EUIWidget(prnt, txt)
{
	bck_use = false;
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

	nativeWidget = new WinLabel(this);
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