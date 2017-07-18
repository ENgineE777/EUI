
#include "EUIPanel.h"
#include "native/win/WinPanel.h"

EUIPanel::EUIPanel(int set_id, EUIWidget* prnt, float set_x, float set_y, float set_w, float set_h) : EUIWidget(set_id, prnt, "")
{
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

	nativeWidget = new WinPanel(this);
}

EUIPanel::~EUIPanel()
{
}

NativePanel* EUIPanel::Native()
{
	return (NativePanel*)nativeWidget;
}