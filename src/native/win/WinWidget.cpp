
#include "EUIWidget.h"
#include "WinWidget.h"

WinWidget* WinWidget::mouse_over = NULL;
HMENU WinWidget::win_id = (HMENU)500;

WinWidget::WinWidget(EUIWidget* set_owner) : NativeWidget(set_owner)
{
	handle = 0;
	tooltip = 0;
	tooltip_inited = false;
	processRes = 0;
}

WinWidget::~WinWidget()
{
    DestroyWindow(handle);
}

HWND WinWidget::GetHandle()
{
	return handle;
}

void* WinWidget::GetNative()
{
	return &handle;
}

void WinWidget::Show(bool set)
{
	ShowWindow(handle, set);

	if (set)
	{
		Redraw();
	}
}

void WinWidget::Enable(bool set)
{
	EnableWindow(handle, set);
}

void WinWidget::SetPos(float set_x, float set_y)
{
	SetWindowPos(handle, 0, (int)set_x, (int)set_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void WinWidget::SetSize(float w, float h)
{
	SetWindowPos(handle, 0, 0, 0, (int)w, (int)h, SWP_NOMOVE | SWP_NOZORDER);
}

void WinWidget::SetText(const char* txt)
{
	CreateTooltip();
}

void WinWidget::Release()
{
	delete this;
}

bool WinWidget::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	switch (msg)
	{
		case WM_NOTIFY:
		case WM_COMMAND:
		case WM_HSCROLL:
		case WM_VSCROLL:
		{
			HWND sender = (HWND)lParam;
			
			if (msg == WM_NOTIFY)
			{
				sender = ((LPNMHDR)lParam)->hwndFrom;
			}

			if (sender != handle)
			{
				for (int i = 0; i < (int)owner->childs.size(); i++)
				{
					WinWidget* win_wgt = (WinWidget*)owner->childs[i]->nativeWidget;

					if (win_wgt && win_wgt->handle == sender)
					{
						((WinWidget*)owner->childs[i]->nativeWidget)->ProcessWidget(msg, wParam, lParam);
					}
				}
			}
		}
		break;
		case WM_PAINT:
		{
			Draw();

			if (owner->listener && (owner->listenerFlag & EUIWidget::OnDraw))
			{
				owner->listener->OnDraw(owner);
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if (owner->listener)
			{
				owner->listener->OnLeftMouseDown(owner, xPos, yPos);
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if (owner->listener)
			{
				owner->listener->OnLeftMouseUp(owner, xPos, yPos);
			}
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			if (owner->listener)
			{
				owner->listener->OnLeftDoubliClick(owner, xPos, yPos);
			}
		}
		break;
		case WM_RBUTTONDOWN:
		{
			if (owner->listener)
			{
				owner->listener->OnRightMouseDown(owner, xPos, yPos);
			}
		}
		break;
		case WM_RBUTTONUP:
		{
			if (owner->listener)
			{
				owner->listener->OnRightMouseUp(owner, xPos, yPos);
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			NotifyMouseOver(this);

			if (owner->listener)
			{
				owner->listener->OnMouseMove(owner, xPos, yPos);
			}
		}
		break;
	}

	return true;
}

void WinWidget::Draw()
{
}

void WinWidget::Redraw()
{
	if (owner->parent)
	{
		InvalidateRect(((WinWidget*)owner->parent->nativeWidget)->GetHandle(), NULL, true);
	}
	else
	{
		InvalidateRect(handle, NULL, true);
	}
}

void WinWidget::CaptureMouse()
{
	SetCapture(handle);
}

void WinWidget::ReleaseMouse()
{
	ReleaseCapture();
}

void WinWidget::Resize()
{
	if (owner->listener && (owner->listenerFlag & EUIWidget::OnResize))
	{
		owner->listener->OnResize(owner);
	}

	for (int i = 0; i<(int)owner->childs.size(); i++)
	{
		owner->childs[i]->nativeWidget->Resize();
	}
}

void WinWidget::NotifyMouseOver(WinWidget* widget)
{
	if (mouse_over != widget)
	{
		if (mouse_over) mouse_over->OnMouseLeave();
		mouse_over = widget;
	}
}

void WinWidget::OnMouseLeave()
{
}

void WinWidget::SetFocused()
{
	SetFocus(handle);
}

bool WinWidget::IsFocused()
{
	return (GetFocus() == handle);
}

void WinWidget::CreateTooltip()
{
	if (tooltip_inited)
	{
		ti.lpszText = (char*)owner->GetText();
		SendMessage(tooltip, TTM_SETTOOLINFO , 0, (LPARAM) (LPTOOLINFO) &ti);
		return;
	}

	RECT rect;

	tooltip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
							 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  handle,
							 NULL, GetModuleHandle(NULL), NULL);
 
	SetWindowPos(tooltip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); 
	GetClientRect (handle, &rect);

	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = handle;
	ti.hinst = GetModuleHandle(NULL);
	ti.uId = 0;
	ti.lpszText = (char*)owner->GetText();
	ti.rect.left = rect.left;
	ti.rect.top = rect.top;
	ti.rect.right = rect.right;
	ti.rect.bottom = rect.bottom;

	SendMessage(tooltip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	tooltip_inited = true;
}

bool WinWidget::IsHoveredByMouse()
{
	POINT point;

	if (GetCursorPos(&point))
	{
		RECT windowRect;

		GetWindowRect( handle, &windowRect );

		return PtInRect( &windowRect, point ) ? true : false;
	}

	return false;
}

void WinWidget::MakeSubClassing()
{
	SetWindowSubclass(handle, &WinWidgetProc, 0, (DWORD_PTR)this);
}

LRESULT CALLBACK WinWidget::WinWidgetProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	WinWidget* widget = (WinWidget*)dwRefData;

	if (!widget->ProcessWidget(uMsg, wParam, lParam))
	{
		int res = widget->processRes;
		widget->processRes = 0;

		return res;
	}

	return ::DefSubclassProc(hWnd, uMsg, wParam, lParam);
}