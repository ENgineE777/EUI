
#include "EUIButton.h"
#include "WinDX11Button.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Button::WinDX11Button(EUIWidget* owner) : NativeButton(owner)
{
	is_howered = false;
}

WinDX11Button::~WinDX11Button()
{
}

EUIButton* WinDX11Button::Owner()
{
	return (EUIButton*)owner;
}

void WinDX11Button::SetText(const char* txt)
{
	//WinWidget::SetText(txt);
	//Button_SetText(handle, txt);
	//InvalidateRect(handle, NULL, false);
}

void WinDX11Button::SetImage(int img, const char* image_name)
{
	/*if (images[img])
	{
		DeleteObject(images[img]);
	}

	images[img] = (HBITMAP)LoadImage (0, image_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	InvalidateRect(handle, NULL, false);*/
}

void WinDX11Button::Draw()
{
	/*UINT uState = EUITheme::UISTATE_NORMAL;
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
	}*/
}

void WinDX11Button::NotifyMouseOver()
{
	if (!is_howered)
	{
		is_howered = true;
		Redraw();
	}

	NativeButton::NotifyMouseOver();
}

void WinDX11Button::OnMouseLeave()
{
	/*if (is_howered)
	{
		if (!Owner()->is_pushable)
		{
			Owner()->is_pushed = false;
		}

		is_howered = false;
		InvalidateRect(handle, NULL, false);
	}*/
}

void WinDX11Button::Enable(bool set)
{
	WinDX11Widget::Enable(set);

	//InvalidateRect(handle, NULL, false);
}
#endif
