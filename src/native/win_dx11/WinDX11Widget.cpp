
#include "EUIWidget.h"
#include "WinDX11Widget.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Widget* WinDX11Widget::mouse_over = nullptr;

WinDX11Widget::WinDX11Widget(EUIWidget* set_owner) : NativeWidget(set_owner)
{
}

WinDX11Widget::~WinDX11Widget()
{
    //DestroyWindow(handle);
}

void* WinDX11Widget::GetNative()
{
	return nullptr;
}

void WinDX11Widget::Show(bool set)
{
	/*ShowWindow(handle, set);

	if (set)
	{
		Redraw();
	}*/
}

void WinDX11Widget::Enable(bool set)
{
	//EnableWindow(handle, set);
}

void WinDX11Widget::SetPos(int set_x, int set_y)
{
	//SetWindowPos(handle, 0, set_x, set_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void WinDX11Widget::SetSize(int w, int h)
{
	//SetWindowPos(handle, 0, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
}

void WinDX11Widget::Release()
{
	delete this;
}

void WinDX11Widget::Draw()
{
	for (auto child : owner->childs)
	{
		((WinDX11Widget*)child->nativeWidget)->Draw();
	}
}

void WinDX11Widget::Redraw()
{
	//InvalidateRect(handle, NULL, true);
}

void WinDX11Widget::CaptureMouse()
{
	//SetCapture(handle);
}

void WinDX11Widget::ReleaseMouse()
{
	ReleaseCapture();
}

void WinDX11Widget::Resize()
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

void WinDX11Widget::NotifyMouseOver()
{
	if (mouse_over != this)
	{
		if (mouse_over) mouse_over->OnMouseLeave();
		mouse_over = this;
	}
}

void WinDX11Widget::OnMouseLeave()
{
}

void WinDX11Widget::SetFocused()
{
	//SetFocus(handle);
}

bool WinDX11Widget::IsFocused()
{
	return false;// (GetFocus() == handle);
}

bool WinDX11Widget::IsHoveredByMouse()
{
	/*POINT point;

	if (GetCursorPos(&point))
	{
		RECT rect;
		GetWindowRect(handle, &rect);

		return PtInRect(&rect, point) ? true : false;
	}*/

	return false;
}

WinDX11Widget* WinDX11Widget::GetHoveredWidget()
{
	if (!IsHoveredByMouse())
	{
		return nullptr;
	}

	for (auto child : owner->childs)
	{
		WinDX11Widget* res = ((WinDX11Widget*)child->nativeWidget)->GetHoveredWidget();
		
		if (res)
		{
			return res;
		}
	}

	return this;
}

void WinDX11Widget::GetMousePos(int& x, int& y)
{
	/*POINT point;

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
	}*/
}

bool WinDX11Widget::IsTreeView()
{
	return false;
}
#endif