
#include "EUIWindow.h"
#include "EUI.h"
#include "WinDX11Window.h"
#include "WinDX11Menu.h"

#ifdef PLATFORM_WIN_DX11

bool WinDX11Window::wndClassReg = false;

LRESULT CALLBACK WinDX11Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _WIN64
	WinDX11Window* wnd = (WinDX11Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#else
	WinDX11Window* wnd = (WinDX11Window*)GetWindowLong(hwnd, GWL_USERDATA);
#endif

	if (wnd)
	{
		return wnd->Proc(hwnd, msg, wParam, lParam);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

WinDX11Window::WinDX11Window(EUIWidget* owner, const char* icon, EUIWindow::Style style, bool adjust) : NativeWindow(owner)
{
	EUI::wnds.push_back(Owner());

	need_strict_size = false;

	active = false;

	const char* class_name = "ENgineWindow";

	if (!wndClassReg)
	{
		WNDCLASS	winClass;
	
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hbrBackground = ( HBRUSH )GetStockObject( GRAY_BRUSH );
		winClass.hCursor = LoadCursor( NULL, IDC_ARROW );
		winClass.hIcon = 0;
		winClass.hInstance = NULL;
		winClass.lpfnWndProc = &WndProc;

		winClass.lpszClassName = class_name;
		winClass.lpszMenuName = NULL;
		winClass.style = CS_CLASSDC | CS_DBLCLKS;

		winClass.hIcon = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
			NULL,             // hInstance must be NULL when loading from a file
			icon,   // the icon file name
			IMAGE_ICON,       // specifies that the file is an icon
			0,                // width of the image (we'll specify default later on)
			0,                // height of the image
			LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
			LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
			LR_SHARED         // let the system release the handle when it's no longer used
		);

		RegisterClass(&winClass);

		wndClassReg = true;
	}

	int flag = 0;

	if (style == EUIWindow::Normal)
	{
		flag = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
	}
	else
	{
		flag = WS_DLGFRAME | WS_BORDER | WS_CAPTION | WS_SIZEBOX;

		if (style == EUIWindow::PopupWithCloseBtn)
		{
			flag |= WS_SYSMENU;
		}
	}

	RECT rect;

	rect.left = (LONG)Owner()->x;
	rect.top = (LONG)Owner()->y;

	rect.right = (LONG)(Owner()->x + Owner()->width);
	rect.bottom = (LONG)(Owner()->y + Owner()->height);

	if (adjust)
	{
		AdjustWindowRectEx(&rect, flag, false, NULL);
	}

	hack_wnd_width = rect.right - rect.left;
	hack_wnd_height = rect.bottom - rect.top;

	handle = CreateWindow(class_name, Owner()->text.c_str(), flag,
	                      rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
	                      NULL, NULL, NULL, NULL );


#ifdef _WIN64
	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)this);
#else
	SetWindowLong(handle, GWL_USERDATA, (LONG)this);
#endif

	if (!adjust)
	{
		RECT rect;
		
		GetClientRect(handle, &rect);
		Owner()->width = rect.right;
		Owner()->height = rect.bottom;
	}

	is_minimazed = false;
}

WinDX11Window::~WinDX11Window()
{
}

EUIWindow* WinDX11Window::Owner()
{
	return (EUIWindow*)owner;
}

void* WinDX11Window::GetNative()
{
	return &handle;
}

void WinDX11Window::Draw()
{
	theme->SetScreenSize(window_data, Owner()->width, Owner()->height);

	//if (GetFocus() == handle)
	{
		WinDX11Widget::Draw();

		if (over_widget)
		{
			over_widget->Draw();
		}
	}

	theme->Present(window_data);
}

LRESULT WinDX11Window::Proc( HWND hwnd, long msg, WPARAM wParam, LPARAM lParam )
{
	if (msg == WM_COMMAND && (HIWORD(wParam) == 0) && menu_wiget /*&& (LOWORD(wParam) == (int)menu_wiget->menu)*/)
	{
		menu_wiget->Process(LOWORD(wParam));
	}

	/*if (msg == WM_COMMAND && (HIWORD(wParam) == 0) && popup_menu_wiget && (LOWORD(wParam) == (int)popup_menu_wiget->menu))
	{
		popup_menu_wiget->Process(LOWORD(wParam));
	}*/

	int pos_x = GET_X_LPARAM(lParam);
	int pos_y = GET_Y_LPARAM(lParam);

	ms_x = pos_x;
	ms_y = pos_y;

	if (left_moused)
	{
		pos_x -= (left_moused->global_x + left_moused->owner->x);
		pos_y -= (left_moused->global_y + left_moused->owner->y);
	}
	else
	if (mouse_over)
	{
		pos_x -= (mouse_over->global_x + mouse_over->owner->x);
		pos_y -= (mouse_over->global_y + mouse_over->owner->y);
	}

	WinDX11Widget* prev_mouse_over = mouse_over;

	switch( msg )
	{
		case WM_CHAR:
		{
			if (focused_widget && (int)wParam >= 30)
			{
				focused_widget->OnCharDown((int)wParam);
			}

			break;
		}
		case WM_KEYDOWN:
		{
			if (focused_widget)
			{
				focused_widget->OnKeyDown((int)wParam);
			}

			if (mouse_over && mouse_over->owner->listener)
			{
				mouse_over->owner->listener->OnKey(mouse_over->owner, (int)wParam);
			}

			skip_on_char = true;

			break;
		}
		case WM_MOUSEWHEEL:
		{
			if (mouse_over)
			{
				mouse_over->OnMouseWheel( -(int)(GET_WHEEL_DELTA_WPARAM(wParam) / 20));
			}

			break;
		}
		case WM_LBUTTONDOWN:
		{
			mouse_over->SetFocused();

			if (mouse_over)
			{
				mouse_over->OnLeftMouseDown(pos_x, pos_y);
			}

			left_moused = mouse_over;

			CaptureMouse();
		}
		break;
		case WM_LBUTTONUP:
		{
			if (left_moused)
			{
				if (mouse_over == left_moused || left_moused->drag_on)
				{
					left_moused->OnLeftMouseUp(pos_x, pos_y);
				}
				else
				{
					left_moused->mouse_pressed = false;
				}

				left_moused = nullptr;
			}
			else
			if (mouse_over)
			{
				mouse_over->OnLeftMouseUp(pos_x, pos_y);
			}

			ReleaseMouse();
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			if (mouse_over && mouse_over->owner->listener)
			{
				mouse_over->owner->listener->OnLeftDoubliClick(mouse_over->owner, pos_x, pos_y);
			}
		}
		break;
		case WM_RBUTTONDOWN:
		{
			if (mouse_over && mouse_over->owner->listener)
			{
				mouse_over->owner->listener->OnRightMouseDown(mouse_over->owner, pos_x, pos_y);
			}

			CaptureMouse();
		}
		break;
		case WM_RBUTTONUP:
		{
			if (mouse_over)
			{
				mouse_over->OnRightMouseUp(pos_x, pos_y);
			}

			ReleaseMouse();
		}
		break;
		case WM_MBUTTONDOWN:
		{
			if (mouse_over && mouse_over->owner->listener)
			{
				mouse_over->owner->listener->OnMiddleMouseDown(mouse_over->owner, pos_x, pos_y);
			}

			CaptureMouse();
		}
		break;
		case WM_MBUTTONUP:
		{
			if (mouse_over && mouse_over->owner->listener)
			{
				mouse_over->owner->listener->OnMiddleMouseUp(mouse_over->owner, pos_x, pos_y);
			}

			ReleaseMouse();
		}
		break;
		case WM_MOUSEMOVE:
		{
			GetMouseOver(this, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			if (left_moused)
			{
				if (prev_mouse_over != mouse_over)
				{
					if (prev_mouse_over == left_moused) left_moused->OnMouseLeave();
					if (mouse_over == left_moused) left_moused->NotifyMouseOver();
				}

				left_moused->OnMouseMove(pos_x, pos_y);
			}
			else
			{
				if (prev_mouse_over != mouse_over)
				{
					if (prev_mouse_over) prev_mouse_over->OnMouseLeave();
					mouse_over->NotifyMouseOver();

					pos_x = GET_X_LPARAM(lParam) - (mouse_over->global_x + mouse_over->owner->x);
					pos_y = GET_Y_LPARAM(lParam) - (mouse_over->global_y + mouse_over->owner->y);
				}

				if (mouse_over)
				{
					mouse_over->OnMouseMove(pos_x, pos_y);
				}
			}
		}
		case WM_SYSCOMMAND:
		{
			if ((wParam & 0xFFF0) == SC_MINIMIZE)
			{
				Owner()->is_minimazed = true;
			}

			if ((wParam & 0xFFF0) == SC_RESTORE)
			{
				Owner()->is_minimazed = false;
			}
		}
		break;
		case WM_SIZE:
		{
			if (wParam != 1)
			{
				Owner()->width = GET_X_LPARAM(lParam);
				Owner()->height = GET_Y_LPARAM(lParam);

				theme->SetOutputWnd(window_data, handle, Owner()->width, Owner()->height);

				Resize();
				CalcGlopalPos();
			}
		};
		break;
		case WM_CLOSE:
		{
			if (owner->listener)
			{
				owner->listener->OnWinClose(owner);
			}
		}
		break;
		case WM_SHOWWINDOW:
		{
			if (owner->listener)
			{
				owner->listener->OnWinShow(owner);
			}
		}
		break;
		case WM_ACTIVATE:
		{
			active = (LOWORD(wParam) > 0);
		}
		break;
		case WM_DESTROY:
		{
			for (int i = 0; i < (int)EUI::wnds.size(); i++)
			{
				if (EUI::wnds[i] == owner)
				{
					if (i == 0)
					{
						for (int i = 1; i < (int)EUI::wnds.size(); i++)
						{
							EUI::wnds[i]->Close();
						}
					}

					EUI::wnds.erase(EUI::wnds.begin() + i);
					break;
				}
			}
		}
		break;
		case WM_GETMINMAXINFO:
		{
			if (need_strict_size)
			{
				MINMAXINFO* pmmi = (MINMAXINFO*)lParam;
			
				pmmi->ptMaxSize.x = hack_wnd_width;
				pmmi->ptMaxSize.y = hack_wnd_height;

				pmmi->ptMinTrackSize.x = hack_wnd_width;
				pmmi->ptMinTrackSize.y = hack_wnd_height;

				pmmi->ptMaxTrackSize.x = hack_wnd_width;
				pmmi->ptMaxTrackSize.y = hack_wnd_height;
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}

void WinDX11Window::GetMouseOver(WinDX11Widget* widget, int ms_x, int ms_y)
{
	if (over_widget && over_widget->IsHitted(ms_x, ms_y))
	{
		mouse_over = over_widget;
		return;
	}

	if (widget->IsHitted(ms_x, ms_y))
	{
		mouse_over = widget;
	}

	for (auto& child : widget->owner->childs)
	{
		if (!child->IsEnabled() || !child->IsVisible())
		{
			continue;
		}

		GetMouseOver((WinDX11Widget*)(child->nativeWidget), ms_x, ms_y);
	}
}

void WinDX11Window::Show(bool set)
{
	ShowWindow(handle, set);

	if (set)
	{
		Redraw();
	}
}

void WinDX11Window::Enable(bool set)
{
	EnableWindow(handle, set);
}

void WinDX11Window::SetPos(int set_x, int set_y)
{
	SetWindowPos(handle, 0, set_x, set_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void WinDX11Window::SetSize(int w, int h)
{
	SetWindowPos(handle, 0, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
}

void WinDX11Window::Maximaze()
{
	ShowWindow(handle, SW_MAXIMIZE);
}

void WinDX11Window::SetAtScreenCenter()
{
	HWND desktop = GetDesktopWindow();

	RECT desktopRect;
	GetWindowRect(desktop, &desktopRect);
	
	RECT rect;
	GetWindowRect(handle, &rect);

	SetPos((int)(0.5f * (desktopRect.right- rect.right)),
	       (int)(0.5f * (desktopRect.bottom - rect.bottom)));

}

void WinDX11Window::Close()
{
	DestroyWindow(handle);
}
#endif