
#include "EUIPanel.h"
#include "native/win/WinPanel.h"

EUIPanel::EUIPanel(EUIWidget* prnt, int set_x, int set_y, int set_w, int set_h) : EUIWidget(prnt, "")
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