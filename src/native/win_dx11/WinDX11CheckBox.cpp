#include "EUICheckBox.h"
#include "WinDX11CheckBox.h"

#ifdef PLATFORM_WIN_DX11

WinDX11CheckBox::WinDX11CheckBox(EUIWidget* owner) : NativeCheckBox(owner)
{
	/*handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
	                      (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));*/
}

WinDX11CheckBox::~WinDX11CheckBox()
{
}

EUICheckBox* WinDX11CheckBox::Owner()
{
	return (EUICheckBox*)owner;
}

void WinDX11CheckBox::SetText(const char* txt)
{
	Redraw();
}

void WinDX11CheckBox::SetChecked(bool set)
{
	Redraw();
}

void WinDX11CheckBox::Draw()
{
	UINT uState = EUITheme::UISTATE_NORMAL;

	if (!Owner()->IsEnabled())
	{
		uState = EUITheme::UISTATE_DISABLED;
	}

	if (Owner()->checked)
	{
		uState |= EUITheme::UISTATE_PUSHED;
	}

	RECT rc = { 0, 0, (LONG)Owner()->width, (LONG)Owner()->height };

	//theme->DrawCheckBox(GetDC(handle), rc, Owner()->text.c_str(), uState, DT_SINGLELINE);
}
#endif