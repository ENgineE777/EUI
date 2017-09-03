
#include "EUICategories.h"
#include "WinCategories.h"

WinCategories::WinCategories(EUIWidget* owner) : NativeCategories(owner)
{
	overallHeight = 0;
	thumbHeight = 0;
	thumbPos = 0;
	thumbMaxPos = 50;
	thumbDelta = 0.0f;
	thumbDraged = false;
	thumbPressed = 0;

	handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
							(int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();
}

WinCategories::~WinCategories()
{
}

EUICategories* WinCategories::Owner()
{
	return (EUICategories*)owner;
}

bool WinCategories::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeCategories::ProcessWidget(msg, wParam, lParam);

	if (msg == WM_LBUTTONDOWN)
	{
		POINT point;

		if (GetCursorPos(&point))
		{
			ScreenToClient(handle, &point);
		}
		
		prev_point = point;

		if (point.x >= owner->width - theme->scrollbarThin && thumbHeight > 0)
		{
			if (point.y < theme->scrollbarThin)
			{
				thumbPressed = -1;
			}
			else
			if (point.y >= owner->height - theme->scrollbarThin)
			{
				thumbPressed = 1;
			}
			else
			{
				thumbDraged = true;
			}

			thumbPos += thumbPressed;

			if (thumbPos < 0)
			{
				thumbPos = 0;
			}

			if (thumbPos > thumbMaxPos)
			{
				thumbPos = thumbMaxPos;
			}

			UpdateChildPos();
			Redraw();
		}
	}
	else
	if (msg == WM_LBUTTONUP)
	{
		if (thumbDraged)
		{
			thumbDraged = false;
			thumbPressed = 0;
		}
		else
		{
			POINT point;

			if (GetCursorPos(&point))
			{
				ScreenToClient(handle, &point);
			}

			if (point.x < owner->width - theme->scrollbarThin || thumbHeight > 0)
			{
				for (int i = 0; i < (int)Owner()->categories.size(); i++)
				{
					if (Owner()->categories[i].y < point.y &&
						point.y < Owner()->categories[i].y + theme->categoryHeight)
					{
						Owner()->categories[i].opened = !Owner()->categories[i].opened;
						CalcThumb();
						UpdateChildPos();
						Redraw();

						break;
					}
				}
			}
		}
	}
	else
	if (msg == WM_MOUSEMOVE && thumbDraged)
	{
		POINT point;

		if (GetCursorPos(&point))
		{
			ScreenToClient(handle, &point);
		}

		thumbPos += point.y - prev_point.y;

		if (thumbPos < 0)
		{
			thumbPos = 0;
		}

		if (thumbPos > thumbMaxPos)
		{
			thumbPos = thumbMaxPos;
		}

		prev_point = point;

		UpdateChildPos();
		Redraw();
	}

	return true;
}

void WinCategories::OnMouseLeave()
{
	thumbDraged = false;
	thumbPressed = 0;
}

void WinCategories::CalcThumb()
{
	overallHeight = 0;

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		overallHeight += theme->categoryHeight;

		for (int j = 0; j < (int)category.childs.size(); j++)
		{
			if (category.opened)
			{
				overallHeight += category.childs[j]->GetHeight();
			}
		}
	}

	if (overallHeight > owner->height)
	{
		thumbMaxPos = overallHeight - owner->height;
		thumbHeight = owner->height - (theme->scrollbarThin + theme->scrollbarPaddingY) * 2 - thumbMaxPos;

		if (thumbHeight < 30)
		{
			thumbHeight = 30;
			thumbMaxPos = owner->height - (theme->scrollbarThin + theme->scrollbarPaddingY) * 2 - thumbHeight;
			thumbDelta = (overallHeight - owner->height) / thumbMaxPos;
		}
		else
		{
			thumbDelta = 1.0f;
		}

		if (thumbPos > thumbMaxPos)
		{
			thumbPos = thumbMaxPos;
		}
	}
	else
	{
		thumbHeight = 0;
		thumbPos = 0;
		thumbMaxPos = 0;
	}
}

void WinCategories::UpdateChildPos()
{
	Owner()->allowCallOnChildShow = false;

	float pos = -thumbPos * thumbDelta;

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		category.y = pos;
		pos += theme->categoryHeight;

		for (int j = 0; j < (int)category.childs.size(); j++)
		{
			if (!category.childsVis[j])
			{
				continue;
			}

			category.childs[j]->Show(category.opened);

			if (category.opened)
			{
				category.childs[j]->SetPos(category.childs[j]->GetX(), pos);
				pos += category.childs[j]->GetHeight();
			}
		}
	}

	Owner()->allowCallOnChildShow = true;
}

void WinCategories::Resize()
{
	CalcThumb();
	UpdateChildPos();
	NativeCategories::Resize();
}

void WinCategories::Draw()
{
	UINT state = EUITheme::UISTATE_NORMAL;

	if (!Owner()->IsEnabled())
	{
		state = EUITheme::UISTATE_DISABLED;
	}

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		RECT m_rcItem = { 0, (LONG)category.y, (LONG)Owner()->width, (LONG)(category.y + theme->categoryHeight) };

		UINT sub_state = 0;

		if (category.opened)
		{
			sub_state = EUITheme::UISTATE_PUSHED;
		}

		theme->DrawCategory(GetDC(handle), m_rcItem, category.name, state | sub_state, DT_SINGLELINE);
	}

	if (thumbHeight > 0)
	{
		RECT m_rcItem = { 0, 0, (LONG)Owner()->width, (LONG)Owner()->height };
		theme->DrawScrollBar(GetDC(handle), m_rcItem, (int)thumbPos, (int)thumbHeight, state);
	}
}

