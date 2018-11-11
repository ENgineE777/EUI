
#include "EUIScrollBar.h"
#include "WinDX11ScrollBar.h"

#ifdef PLATFORM_WIN_DX11

WinDX11ScrollBar::WinDX11ScrollBar(EUIWidget* owner) : NativeScrollBar(owner)
{
	SetLimitImpl();
}

WinDX11ScrollBar::~WinDX11ScrollBar()
{
}

EUIScrollBar* WinDX11ScrollBar::Owner()
{
	return (EUIScrollBar*)owner;
}

void WinDX11ScrollBar::SetPosition(int pos)
{
	float k = (float)pos / (float)Owner()->max_pos;
	thumb_pos = (int)(k * thumb_max_pos);
}

void WinDX11ScrollBar::SetLimitImpl()
{
	if (Owner()->horiz)
	{
		thumb_size = (int)((owner->width - 30) * 0.33f);
		thumb_max_pos = (owner->width - 30) - thumb_size;
	}
	else
	{
		thumb_size = (int)((owner->height - 30) * 0.33f);
		thumb_max_pos = (owner->height - 30) - thumb_size;
	}

	SetPosition((int)Owner()->cur_pos);
}

void WinDX11ScrollBar::SetLimit()
{
	SetLimitImpl();
}

void WinDX11ScrollBar::Draw()
{
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);

	if (((EUIScrollBar*)owner)->horiz)
	{
		theme->Draw("ScrollerHorz", global_x + owner->x, global_y + owner->y, owner->width, owner->height);

		const char* elem = "ScrollerLeftArrow";

		if (sel_elem == 0)
		{
			if (mouse_pressed)
			{
				elem = "ScrollerLeftArrowActive";
			}
			else
			if (is_howered)
			{
				elem = "ScrollerLeftArrowHowered";
			}
		}

		theme->Draw(elem, global_x + owner->x, global_y + owner->y, 15, 15);

		elem = "ScrollerThumbHorz";

		if (sel_elem == 1)
		{
			if (mouse_pressed)
			{
				elem = "ScrollerThumbHorzActive";
			}
			else
			if (is_howered)
			{
				elem = "ScrollerThumbHorzHowered";
			}
		}

		theme->Draw(elem, global_x + owner->x + 15 + thumb_pos, global_y + owner->y + 2, thumb_size, 11);

		elem = "ScrollerRightArrow";

		if (sel_elem == 2)
		{
			if (mouse_pressed)
			{
				elem = "ScrollerRightArrowActive";
			}
			else
			if (is_howered)
			{
				elem = "ScrollerRightArrowHowered";
			}
		}

		theme->Draw(elem, global_x + owner->x + owner->width - 15, global_y + owner->y, 15, 15);
	}
	else
	{
		theme->Draw("ScrollerVert", global_x + owner->x, global_y + owner->y, owner->width, owner->height);

		const char* elem = "ScrollerUpArrow";

		if (sel_elem == 0)
		{
			if (mouse_pressed)
			{
				elem = "ScrollerUpArrowActive";
			}
			else
			if (is_howered)
			{
				elem = "ScrollerUpArrowHowered";
			}
		}

		theme->Draw(elem, global_x + owner->x, global_y + owner->y, 15, 15);

		elem = "ScrollerThumbVert";

		if (sel_elem == 1)
		{
			if (mouse_pressed)
			{
				elem = "ScrollerThumbVertActive";
			}
			else
			if (is_howered)
			{
				elem = "ScrollerThumbVertHowered";
			}
		}

		theme->Draw(elem, global_x + owner->x + 2, global_y + owner->y + 15 + thumb_pos, 11, thumb_size);

		elem = "ScrollerDownArrow";

		if (sel_elem == 2)
		{
			if (mouse_pressed)
			{
				elem = "ScrollerDownArrowActive";
			}
			else
			if (is_howered)
			{
				elem = "ScrollerDownArrowHowered";
			}
		}

		theme->Draw(elem, global_x + owner->x, global_y + owner->y + owner->height - 15, 15, 15);
	}

	
	NativeScrollBar::Draw();
}

void WinDX11ScrollBar::OnMouseMove(int ms_x, int ms_y)
{
	if (!mouse_pressed)
	{
		int ms = Owner()->horiz ? ms_x : ms_y;
		sel_elem = -1;

		if (ms < 15)
		{
			sel_elem = 0;
		}
		else
		if (thumb_pos + 15 < ms && ms < thumb_pos + 15 + thumb_size)
		{
			sel_elem = 1;

			thumb_delta = ms - (thumb_pos + 15);
		}
		else
		if (owner->height - 15 < ms)
		{
			sel_elem = 2;
		}
	}
	else
	if (mouse_pressed && sel_elem == 1)
	{
		int prev_thumb_pos = thumb_pos;

		thumb_pos = (Owner()->horiz ? ms_x : ms_y) - 15 - thumb_delta;

		if (thumb_pos < 0)
		{
			thumb_pos = 0;
		}
		else
		if (thumb_pos > thumb_max_pos)
		{
			thumb_pos = thumb_max_pos;
		}

		if (prev_thumb_pos != thumb_pos)
		{
			float k = (float)thumb_pos / (float)thumb_max_pos;

			Owner()->cur_pos = (int)(k * (float)Owner()->max_pos);

			if (Owner()->listener)
			{
				Owner()->listener->OnSrollerPosChange(Owner(), Owner()->cur_pos);
			}
		}
	}

	NativeScrollBar::OnMouseMove(ms_x, ms_y);
}

void WinDX11ScrollBar::OnMouseLeave()
{
	if (!mouse_pressed)
	{
		sel_elem = -1;
	}
	NativeScrollBar::OnMouseLeave();
}

void WinDX11ScrollBar::OnLeftMouseUp(int ms_x, int ms_y)
{
	if (!is_howered)
	{
		sel_elem = -1;
	}

	NativeScrollBar::OnLeftMouseUp(ms_x, ms_y);
}
#endif