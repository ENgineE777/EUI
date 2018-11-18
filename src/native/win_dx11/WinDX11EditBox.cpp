
#include "EUIEditBox.h"
#include "WinDX11EditBox.h"
#include "UTFConv.h"

#ifdef PLATFORM_WIN_DX11

WinDX11EditBox::WinDX11EditBox(EUIWidget* owner) : NativeEditBox(owner)
{
	SetTimer(10);
}

WinDX11EditBox::~WinDX11EditBox()
{
}

EUIEditBox* WinDX11EditBox::Owner()
{
	return (EUIEditBox*)owner;
}

void WinDX11EditBox::SetSel(int sel)
{
	start_sel = sel;
	start_sel_offset = theme->font.CalcWidth(owner->GetText(), start_sel);

	if (start_sel_offset > owner->width - 6 + offset)
	{
		offset += 30;
	}
	else
	if (start_sel_offset < - 6 + offset)
	{
		offset -= 30;
	}
}

void WinDX11EditBox::SetText(const char* txt)
{
	start_sel = -1;
	start_sel_offset = 0;
	offset = 0;

	/*int pos = LOWORD(Edit_GetSel(handle));

	if (pos > (int)Owner()->text.length())
	{
		pos = (int)Owner()->text.length();
	}*/

	NativeEditBox::SetText(txt);

	//Edit_SetSel(handle,pos,pos);

	time2callback = -1.0f;
	Redraw();
}

const char* WinDX11EditBox::GetText()
{
	return Owner()->text.c_str();
}

void WinDX11EditBox::Draw()
{
	const char* elem = is_howered ? "EditBoxHowered" : "EditBoxNormal";

	if (is_focused)
	{
		elem = is_howered ? "EditBoxFocusedHowered" : "EditBoxFocused";
	}

	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);

	theme->Draw(elem, global_x + owner->x, global_y + owner->y, owner->width, owner->height);

	float font_color[4];
	font_color[0] = 1.0f;
	font_color[1] = (time2callback < -0.01f) ? 1.0f : 0.9f;
	font_color[2] = (time2callback < -0.01f) ? 1.0f : 0.0f;
	font_color[3] = 1.0f;

	theme->font.Print(global_x + owner->x + 3 - offset, global_y + owner->y + 5, font_color, owner->GetText());

	if (is_focused)
	{
		theme->Draw(nullptr, font_color, global_x + owner->x + 3 + start_sel_offset + 1 - offset, global_y + owner->y + 2, 1, owner->height - 4);
	}

	NativeEditBox::Draw();
}

void WinDX11EditBox::OnKeyDown(int key)
{
	if (key == VK_RETURN)
	{
		time2callback = -1.0f;

		if (Owner()->listener)
		{
			Owner()->listener->OnEditBoxStopEditing(Owner());
		}

		Redraw();
	}
	else
	if (key == VK_SPACE)
	{
		Owner()->text.insert(start_sel, " ");
		SetSel(start_sel+1);

		time2callback = 1.0f;
		Redraw();
	}
	else
	if (key == VK_LEFT)
	{
		if (start_sel > 0)
		{
			SetSel(start_sel - 1);
		}
	}
	else
	if (key == VK_RIGHT)
	{
		if (strlen(Owner()->text.c_str()) > start_sel)
		{
			SetSel(start_sel + 1);
		}
	}
	else
	if (key == VK_DELETE)
	{
		if (strlen(Owner()->text.c_str()) > start_sel)
		{
			Owner()->text.erase(start_sel, 1);
			time2callback = 1.0f;
		}
	}
	else
	if (key == VK_BACK)
	{
		if (start_sel > 0)
		{
			Owner()->text.erase(start_sel - 1, 1);
			SetSel(start_sel-1);

			time2callback = 1.0f;
		}
	}
	else
	if (key != VK_RETURN && key != VK_BACK)
	{
		GetText();

		bool add = true;

		if (key == '-')
		{
			if (Owner()->text.find('-') != std::string::npos)
			{
				add = false;
			}

			if (Owner()->inputType == EUIEditBox::InputUInteger || Owner()->inputType == EUIEditBox::InputUFloat)
			{
				add = false;
			}

			if ((Owner()->inputType == EUIEditBox::InputInteger || Owner()->inputType == EUIEditBox::InputFloat) && strlen(owner->GetText()) > 0)
			{
				add = false;
			}
		}
		else
		if (key == '.')
		{
			if (Owner()->text.find('.') != std::string::npos)
			{
				add = false;
			}

			if (Owner()->inputType == EUIEditBox::InputInteger || Owner()->inputType == EUIEditBox::InputUInteger)
			{
				add = false;
			}
		}
		else
		if ((key < '0' || key > '9') &&
			(Owner()->inputType != EUIEditBox::InputText))
		{
			add = false;
		}

		if (add)
		{
			wchar_t add[2];
			add[0] = key;
			add[1] = 0;

			std::string str;
			UTFConv::UTF16to8(str, add);

			Owner()->text.insert(start_sel, str);
			SetSel(start_sel + (int)strlen(str.c_str()));

			time2callback = 1.0f;
			Redraw();
		}
	}
}

void WinDX11EditBox::OnTimer()
{
	if (time2callback > 0.0f)
	{
		time2callback -= 0.01f;

		if (time2callback < 0.0f)
		{
			time2callback = -1.0f;

			if (Owner()->listener)
			{
				Owner()->listener->OnEditBoxStopEditing(Owner());
			}

			Redraw();
		}
	}
}

void WinDX11EditBox::OnLeftMouseDown(int ms_x, int ms_y)
{
	SetSel(theme->font.GetIndex(ms_x - 3 + offset, owner->GetText()));
	start_sel_offset = theme->font.CalcWidth(owner->GetText(), start_sel);
	NativeEditBox::OnLeftMouseDown(ms_x, ms_y);
}

void WinDX11EditBox::OnLeftMouseUp(int ms_x, int ms_y)
{
	NativeEditBox::OnLeftMouseUp(ms_x, ms_y);
}

#endif