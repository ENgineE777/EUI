
#include "EUIWidget.h"
#include "WinWidget.h"

#ifdef PLATFORM_WIN

WinWidget* WinWidget::mouse_over = nullptr;
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

void WinWidget::SetPos(int set_x, int set_y)
{
	SetWindowPos(handle, 0, set_x, set_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void WinWidget::SetSize(int w, int h)
{
	SetWindowPos(handle, 0, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
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

	HWND sender = (HWND)lParam;

	if (msg == WM_NOTIFY)
	{
		sender = ((LPNMHDR)lParam)->hwndFrom;
	}

	if (sender != handle)
	{
		for (auto child : owner->childs)
		{
			WinWidget* win_wgt = (WinWidget*)child->nativeWidget;

			if (win_wgt && win_wgt->handle == sender)
			{
				if (!((WinWidget*)child->nativeWidget)->ProcessWidget(msg, wParam, lParam))
				{
					return false;
				}

				break;
			}
		}
	}

	if (msg == WM_COMMAND && (HIWORD(wParam) == 0) && menu_wiget)
	{
		menu_wiget->ProcessWidget(msg, wParam, lParam);
	}

	switch (msg)
	{
		case WM_KEYDOWN:
		{
			if (owner->listener)
			{
				owner->listener->OnKey(owner, (int)wParam);
			}
			break;
		}
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
			NotifyMouseOver();

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
	for (auto child : owner->childs)
	{
		((WinWidget*)child->nativeWidget)->Draw();
	}
}

void WinWidget::Redraw()
{
	InvalidateRect(handle, NULL, true);
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

void WinWidget::NotifyMouseOver()
{
	if (mouse_over != this)
	{
		if (mouse_over) mouse_over->OnMouseLeave();
		mouse_over = this;
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
		RECT rect;
		GetWindowRect(handle, &rect);

		return PtInRect(&rect, point) ? true : false;
	}

	return false;
}

WinWidget* WinWidget::GetHoveredWidget()
{
	if (!IsHoveredByMouse())
	{
		return nullptr;
	}

	for (auto child : owner->childs)
	{
		WinWidget* res = ((WinWidget*)child->nativeWidget)->GetHoveredWidget();
		
		if (res)
		{
			return res;
		}
	}

	return this;
}

void WinWidget::GetMousePos(int& x, int& y)
{
	POINT point;

	if (GetCursorPos(&point))
	{
		RECT rect;
		GetWindowRect(handle, &rect);

		x = point.x - rect.left;
		y = point.y - rect.top;
	}
	else
	{
		x = 0;
		y = 0;
	}
}

void WinWidget::MakeSubClassing()
{
	SetWindowSubclass(handle, &WinWidgetProc, 0, (DWORD_PTR)this);
}

bool WinWidget::IsTreeView()
{
	return false;
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
#endif