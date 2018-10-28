
#include "EUILabel.h"
#include "WinLabel.h"

#ifdef PLATFORM_WIN

WinLabel::WinLabel(EUIWidget* owner) : NativeLabel(owner)
{
	handle = CreateWindow("STATIC", "Static", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
	                      (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();
}

WinLabel::~WinLabel()
{
}

EUILabel* WinLabel::Owner()
{
	return (EUILabel*)owner;
}

void WinLabel::SetText(const char* txt)
{
	WinWidget::SetText(txt);
	Static_SetText(handle, txt);
	Redraw();
}

void WinLabel::Draw()
{
	RECT rc = { 0, 0, (LONG)Owner()->width, (LONG)Owner()->height };

	UINT uState = EUITheme::UISTATE_NORMAL;

	if (!Owner()->IsEnabled())
	{
		uState = EUITheme::UISTATE_DISABLED;
	}

	COLORREF color = theme->GetColor("LABEL_BACK");

	if (Owner()->bck_use)
	{
		color = RGB(Owner()->bck_color[0], Owner()->bck_color[1], Owner()->bck_color[2]);
	}

	theme->DrawGradient(GetDC(handle), rc, color, color, false, 2);
	theme->DrawLabel(GetDC(handle), rc, Owner()->text.c_str(), uState, DT_SINGLELINE);
}
#endif