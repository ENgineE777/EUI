
#include "EUITabPanel.h"
#include "EUITabSheet.h"
#include "native/win/WinTabPanel.h"

EUITabPanel::EUITabPanel(EUIWidget* prnt, float set_x, float set_y, float set_w, float set_h) : EUIWidget(prnt, "")
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

EUITabSheet* EUITabPanel::AddTab(const char* txt)
{
	EUITabSheet* sheet = new EUITabSheet(this, txt);

	if (childs.size() == 1)
	{
		sheet->Show(true);
	}

	return sheet;
}

EUITabSheet* EUITabPanel::GetTab(int index)
{
	return (EUITabSheet*)childs[index];
}

int EUITabPanel::GetCurrentTabIndex()
{
	return Native()->GetCurrentTabIndex();
}

void EUITabPanel::DeleteTab(int index)
{
	Native()->DeleteTab(index);
}

void EUITabPanel::ClearTabs()
{
	Native()->ClearTabs();
}