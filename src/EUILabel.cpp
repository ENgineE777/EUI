
#include "EUILabel.h"
#include "native/win/WinLabel.h"

EUILabel::EUILabel(int set_id, EUIWidget* prnt, const char* txt, float set_x, float set_y, float set_w, float set_h) : EUIWidget(set_id, prnt, txt)
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
}