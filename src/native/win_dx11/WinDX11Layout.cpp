
#include "EUILayout.h"
#include "WinDX11Layout.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Layout::WinDX11Layout(EUIWidget* owner) : NativeLayout(owner)
{
	/*handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_NOTIFY,
	                      (int)Owner()->parent->x, (int)Owner()->parent->y, (int)Owner()->parent->width, (int)Owner()->parent->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();*/
}

WinDX11Layout::~WinDX11Layout()
{

}

EUILayout* WinDX11Layout::Owner()
{
	return (EUILayout*)owner;
}

void WinDX11Layout::Resize()
{
	if (in_resize)
	{
		return;
	}

	in_resize = true;

	Owner()->Resize();
	NativeLayout::Resize();

	in_resize = false;
}
#endif

