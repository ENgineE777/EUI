#include "EUICheckBox.h"
#include "WinCheckBox.h"

#ifdef PLATFORM_WIN

WinCheckBox::WinCheckBox(EUIWidget* owner) : NativeCheckBox(owner)
{
	handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
	                      (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));
}

WinCheckBox::~WinCheckBox()
{
}

EUICheckBox* WinCheckBox::Owner()
{
	return (EUICheckBox*)owner;
}

bool WinCheckBox::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeCheckBox::ProcessWidget(msg, wParam, lParam);

	if (msg == WM_LBUTTONUP)
	{
		Owner()->checked = !Owner()->checked;
		Redraw();
	}

	return true;
}

void WinCheckBox::SetText(const char* txt)
{
	Redraw();
}

void WinCheckBox::SetChecked(bool set)
{
	Redraw();
}

void WinCheckBox::Draw()
{
	UINT uState = WinTheme::UISTATE_NORMAL;

	if (!Owner()->IsEnabled())
	{
		uState = WinTheme::UISTATE_DISABLED;
	}

	if (Owner()->checked)
	{
		uState |= WinTheme::UISTATE_PUSHED;
	}

	RECT rc = { 0, 0, (LONG)Owner()->width, (LONG)Owner()->height };

	theme->DrawCheckBox(GetDC(handle), rc, Owner()->text.c_str(), uState, DT_SINGLELINE);
}
#endif