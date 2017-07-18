
#include "EUILabel.h"
#include "native/win/WinLabel.h"

EUILabel::EUILabel(int set_id, EUIWidget* prnt, const char* txt, bool set_color_box, float set_x, float set_y, float set_w, float set_h) : EUIWidget(set_id, prnt, txt)
{
	color_box = set_color_box;

	color[0] = 255;
	color[1] = 255;
	color[2] = 255;

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