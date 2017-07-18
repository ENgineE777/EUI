
#include "EUITabPanel.h"
#include "native/win/WinTabPanel.h"

EUITabPanel::EUITabPanel(int set_id, EUIWidget* prnt, float set_x, float set_y, float set_w, float set_h) : EUIWidget(set_id, prnt, "")
{
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

	nativeWidget = new WinTabPanel(this);
}

EUITabPanel::~EUITabPanel()
{
}

NativeTabPanel* EUITabPanel::Native()
{
	return (NativeTabPanel*)nativeWidget;
}

void EUITabPanel::AddTab(const char* txt)
{
	Native()->AddTab(txt);
}

void EUITabPanel::SetTabName(int index, const char* name)
{
	Native()->SetTabName(index, name);
}

void EUITabPanel::DelTab(int index)
{
	Native()->DelTab(index);
}

void EUITabPanel::ClearTabs()
{
	Native()->ClearTabs();
}

void EUITabPanel::AddWidget2Tab(int index, EUIWidget* widget)
{
	Native()->AddWidget2Tab(index, widget);
}

void EUITabPanel::SetCurrentTab(int index)
{
	Native()->SetCurrentTab(index);
}

int EUITabPanel::GetCurrentTab()
{
	return Native()->GetCurrentTab();
}