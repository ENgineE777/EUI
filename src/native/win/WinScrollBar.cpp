
#include "EUIScrollBar.h"
#include "WinScrollBar.h"

#ifdef PLATFORM_WIN

WinScrollBar::WinScrollBar(EUIWidget* owner) : NativeScrollBar(owner)
{
	int flag = WS_CHILD | WS_VISIBLE;

	if (((EUIScrollBar*)owner)->horiz)
	{
		flag |= SBS_HORZ;
	}
	else
	{
		flag |= SBS_VERT;
	}


	handle = CreateWindow("SCROLLBAR", "", flag,
	                      (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                      ((WinWidget*)(Owner()->parent->nativeWidget))->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	SetLimitImpl();
}

WinScrollBar::~WinScrollBar()
{
}

EUIScrollBar* WinScrollBar::Owner()
{
	return (EUIScrollBar*)owner;
}

bool WinScrollBar::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeScrollBar::ProcessWidget(msg, wParam, lParam);

	if ((Owner()->horiz && msg != WM_HSCROLL) ||
		(!Owner()->horiz && msg != WM_VSCROLL))
	{
		return true;
	}

	int last_pos = Owner()->cur_pos;

	switch (LOWORD(wParam))
	{
		case SB_TOP:
			Owner()->cur_pos = 0;
		break;

		case SB_LINEUP:
			if (Owner()->cur_pos > 0)
			{
				Owner()->cur_pos--;
			}
		break;

		case SB_PAGEUP:
			Owner()->cur_pos -= 10;
			if (Owner()->cur_pos < 0) Owner()->cur_pos = 0;
		break;

		case SB_THUMBPOSITION:
			Owner()->cur_pos = HIWORD(wParam);
		break;

		case SB_THUMBTRACK:
			Owner()->cur_pos = HIWORD(wParam);
		break;

		case SB_LINEDOWN:
			if (Owner()->cur_pos < Owner()->max_pos) Owner()->cur_pos++;
		break;

		case SB_PAGEDOWN:
			Owner()->cur_pos += 10;
			if (Owner()->cur_pos > Owner()->max_pos) Owner()->cur_pos = Owner()->max_pos;
		break;

		case SB_BOTTOM:
			Owner()->cur_pos = Owner()->max_pos;
		break;

		case SB_ENDSCROLL:
		break;
	}

	if (last_pos != Owner()->cur_pos)
	{
		if (Owner()->listener)
		{
			Owner()->listener->OnSrollerPosChange(Owner(), last_pos);
		}
	}

	SetScrollPos(handle, SB_CTL, Owner()->cur_pos, true);

	return true;
}

void WinScrollBar::SetPosition(int pos)
{
	SetScrollPos(handle, SB_CTL, Owner()->cur_pos, true);
}

void WinScrollBar::SetLimitImpl()
{
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = Owner()->max_pos;
	si.nPage = Owner()->page_size;
	si.nPos = Owner()->cur_pos;
	SetScrollInfo(handle, SB_CTL, &si, true);
}

void WinScrollBar::SetLimit()
{
	SetLimitImpl();
}
#endif