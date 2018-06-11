
#include "EUIButton.h"
#include "WinButton.h"

WinButton::WinButton(EUIWidget* owner) : NativeButton(owner)
{
	is_howered = false;

	for (int i = 0; i < 4; i++)
	{
		images[i] = 0;
	}

	handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
	                      (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	CreateTooltip();
}

WinButton::~WinButton()
{
}

EUIButton* WinButton::Owner()
{
	return (EUIButton*)owner;
}

bool WinButton::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeButton::ProcessWidget(msg, wParam, lParam);

	if (msg == WM_LBUTTONDOWN)
	{
		Owner()->is_pushed = !Owner()->is_pushed;
		InvalidateRect(handle, NULL, false);
	}

	if (!Owner()->is_pushable && msg == WM_LBUTTONUP)
	{
		Owner()->is_pushed = false;
		InvalidateRect(handle, NULL, false);
	}

	return true;
}

void WinButton::SetText(const char* txt)
{
	WinWidget::SetText(txt);
	Button_SetText(handle, txt);
	InvalidateRect(handle, NULL, false);
}

void WinButton::SetImage(int img, const char* image_name)
{
	if (images[img])
	{
		DeleteObject(images[img]);
	}

	images[img] = (HBITMAP)LoadImage (0, image_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	InvalidateRect(handle, NULL, false);
}

void WinButton::Draw()
{
	UINT uState = EUITheme::UISTATE_NORMAL;
	EUIButton::Image img = EUIButton::Normal;

	if (!Owner()->IsEnabled())
	{
		uState = EUITheme::UISTATE_DISABLED;
		img = EUIButton::Disabled;
	}
	else
	{
		if (is_howered)
		{
			uState = EUITheme::UISTATE_HOWERED;
			img = EUIButton::Howered;
		}

		if (Owner()->is_pushed)
		{
			uState = EUITheme::UISTATE_PUSHED;
			img = EUIButton::Pushed;
		}

		if (IsFocused())
		{
			uState |= EUITheme::UISTATE_FOCUSED;
		}
	}

	if (images[img])
	{
		HDC hDC = GetDC(handle);
		HDC memDC = CreateCompatibleDC(hDC);

		SelectObject(memDC, images[img]);
		BitBlt(hDC, 0, 0, (int)Owner()->width, (int)Owner()->height, memDC, 0, 0, SRCCOPY);

		DeleteDC(memDC);
	}
	else
	{
		RECT rcItem = { 0, 0,(LONG)Owner()->width, (LONG)Owner()->height };
		theme->DrawButton(GetDC(handle), rcItem, Owner()->text.c_str(), uState, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
}

void WinButton::NotifyMouseOver()
{
	if (!is_howered)
	{
		is_howered = true;
		Redraw();
	}

	NativeButton::NotifyMouseOver();
}

void WinButton::OnMouseLeave()
{
	if (is_howered)
	{
		if (!Owner()->is_pushable)
		{
			Owner()->is_pushed = false;
		}

		is_howered = false;
		InvalidateRect(handle, NULL, false);
	}
}

void WinButton::Enable(bool set)
{
	WinWidget::Enable(set);

	InvalidateRect(handle, NULL, false);
}

