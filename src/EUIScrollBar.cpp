
#include "EUIScrollBar.h"
#include "native/win/WinScrollBar.h"

EUIScrollBar::EUIScrollBar(EUIWidget* prnt, bool set_horiz, int set_x, int set_y, int set_w, int set_h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

	horiz = set_horiz;
	cur_pos = 0;
	max_pos = 100;
	page_size = 10;

	nativeWidget = new WinScrollBar(this);
}

EUIScrollBar::~EUIScrollBar()
{

}

NativeScrollBar* EUIScrollBar::Native()
{
	return (NativeScrollBar*)nativeWidget;
}

void EUIScrollBar::SetPosition(int pos)
{
	Native()->SetPosition(pos);
}

int EUIScrollBar::GetPosition()
{
	return cur_pos;
}

void EUIScrollBar::SetLimit(int page_sz, int max_ps)
{
	page_size = page_sz;
	max_pos = max_ps;

	if (cur_pos>max_pos)
	{
		cur_pos = max_pos - 1;
	}

	Native()->SetLimit();
}