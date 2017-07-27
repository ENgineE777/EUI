
#include "EUILabel.h"
#include "WinLabel.h"

WinLabel::WinLabel(EUIWidget* owner) : NativeLabel(owner)
{
	handle = CreateWindow("STATIC", "Static", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
							Owner()->x, Owner()->y, Owner()->width, Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), (HMENU)win_id, NULL, NULL);
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
	RECT m_rcItem = { 0, 0, Owner()->width, Owner()->height };

	UINT uState = EUITheme::UISTATE_NORMAL;

	if (!Owner()->IsEnabled())
	{
		uState = EUITheme::UISTATE_DISABLED;
	}

	if (Owner()->bck_use)
	{
		COLORREF color = RGB(Owner()->bck_color[0], Owner()->bck_color[1], Owner()->bck_color[2]);
		theme->DrawGradient(GetDC(handle), m_rcItem, color, color, false, 2);
	}

	theme->DrawLabel(GetDC(handle), m_rcItem, Owner()->text.c_str(), uState, DT_SINGLELINE);
}