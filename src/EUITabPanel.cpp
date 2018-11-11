
#include "EUITabPanel.h"
#include "EUITabSheet.h"

#ifdef PLATFORM_WIN
#include "native/win/WinTabPanel.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11TabPanel.h"
#endif

EUITabPanel::EUITabPanel(EUIWidget* prnt, int set_x, int set_y, int set_w, int set_h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

#ifdef PLATFORM_WIN
	nativeWidget = new WinTabPanel(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11TabPanel(this);
#endif
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

	sheet->Show(childs.size() == 1);

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
	DelChild(childs[index]);
}

void EUITabPanel::ClearTabs()
{
	Native()->ClearTabs();
	DeleteChilds();
}