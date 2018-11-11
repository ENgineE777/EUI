
#include "EUIPanel.h"

#ifdef PLATFORM_WIN
#include "native/win/WinPanel.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11Panel.h"
#endif

EUIPanel::EUIPanel(EUIWidget* prnt, int set_x, int set_y, int set_w, int set_h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

#ifdef PLATFORM_WIN
	nativeWidget = new WinPanel(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11Panel(this);
#endif
}

EUIPanel::~EUIPanel()
{
}

NativePanel* EUIPanel::Native()
{
	return (NativePanel*)nativeWidget;
}

void EUIPanel::SetTexture(void* texture)
{
	Native()->SetTexture(texture);
}