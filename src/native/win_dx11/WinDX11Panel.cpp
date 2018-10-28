
#include "EUIPanel.h"
#include "WinDX11Panel.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Panel::WinDX11Panel(EUIWidget* owner) : NativePanel(owner)
{
	/*handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
	                      (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();*/
}

WinDX11Panel::~WinDX11Panel()
{
}

EUIPanel* WinDX11Panel::Owner()
{
	return (EUIPanel*)owner;
}

void WinDX11Panel::Draw()
{
	/*RECT rc = { 0, 0, (LONG)Owner()->width, (LONG)Owner()->height };

	UINT uState = EUITheme::UISTATE_NORMAL;

	if (!Owner()->IsEnabled())
	{
		uState = EUITheme::UISTATE_DISABLED;
	}

	COLORREF color = theme->GetColor("LABEL_BACK");

	theme->DrawGradient(GetDC(handle), rc, color, color, false, 2);

	NativePanel::Draw();*/
}
#endif