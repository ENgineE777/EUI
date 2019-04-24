
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

	end_sel = -1;

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

void WinDX11EditBox::SetEndSel(int sel)
{
	end_sel = sel;
	end_sel_offset = theme->font.CalcWidth(owner->GetText(), end_sel);

	if (end_sel_offset > owner->width - 6 + offset)
	{
		offset += 30;
	}
	else
	if (end_sel_offset < -6 + offset)
	{
		offset -= 30;
	}
}

bool WinDX11EditBox::DeselectEnd()
{
	bool res = false;

	if (end_sel != -1)
	{
		if (end_sel > start_sel)
		{
			Owner()->text.erase(start_sel, end_sel - start_sel);
			res = true;
		}
		else
		if (end_sel < start_sel)
		{
			Owner()->text.erase(end_sel, start_sel - end_sel);
			SetSel(end_sel);
			res = true;
		}

		end_sel = -1;
	}

	return res;
}

void WinDX11EditBox::SetText(const char* txt)
{
	NativeEditBox::SetText(txt);
	SetSel((int)strlen(txt));

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

	if (is_focused && end_sel != -1)
	{
		float font_color[4];
		font_color[0] = 0.67f;
		font_color[1] = 0.554f;
		font_color[2] = 0.378f;
		font_color[3] = 1.0f;

		int width = end_sel_offset - start_sel_offset;
		int pos = start_sel_offset;

		if (width < 0)
		{
			width = -width;
			pos = end_sel_offset;
		}

		theme->Draw(nullptr, font_color, global_x + owner->x + 3 + pos + 1 - offset, global_y + owner->y + 2, 1 + width, owner->height - 4);
	}

	theme->font.Print(global_x + owner->x + 3 - offset, global_y + owner->y + 5, font_color, owner->GetText());

	if (is_focused && start_sel != -1)
	{
		theme->Draw(nullptr, font_color, global_x + owner->x + 3 + (end_sel != -1 ? end_sel_offset : start_sel_offset) + 1 - offset, global_y + owner->y + 2, 1, owner->height - 4);
	}

	NativeEditBox::Draw();
}

void WinDX11EditBox::OnTextCopy()
{
	if (start_sel != -1 && end_sel != -1 && start_sel != end_sel)
	{
		if (!OpenClipboard(NULL)) return;

		EmptyClipboard();

		int pos = start_sel;
		int len = end_sel - start_sel;

		if (len < 0)
		{
			pos = end_sel;
			len = -len;
		}

		HGLOBAL hglb = GlobalAlloc(GMEM_MOVEABLE, len + 1);

		LPTSTR lptstr = (LPTSTR)GlobalLock(hglb);

		memcpy(lptstr, &Owner()->text[pos], len);
		lptstr[len] = 0;
		GlobalUnlock(hglb);

		SetClipboardData(CF_TEXT, hglb);

		CloseClipboard();
	}
}

void WinDX11EditBox::OnTextPaste()
{
	if (start_sel != -1)
	{
		DeselectEnd();

		if (IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard(nullptr))
		{
			HGLOBAL hglb = GetClipboardData(CF_TEXT);

			if (hglb != NULL)
			{
				LPTSTR lptstr = (LPTSTR)GlobalLock(hglb);
				if (lptstr != NULL)
				{
					int len = strlen(lptstr);

					for (int i = 0; i < len; i++)
					{
						OnCharDown(lptstr[i]);
					}

					GlobalUnlock(hglb);
				}
			}

			CloseClipboard();
		}
	}
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
	if (key == VK_LEFT)
	{
		if (GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_RSHIFT))
		{
			if (end_sel != -1)
			{
				if (end_sel > 0)
				{
					SetEndSel(end_sel - 1);
				}
			}
			else
			{
				SetEndSel(start_sel - 1);
			}
		}
		else
		{
			if (end_sel != -1)
			{
				SetSel(start_sel < end_sel ? start_sel : end_sel);
			}
			else
			{
				if (start_sel > 0)
				{
					SetSel(start_sel - 1);
				}
			}
		}
	}
	else
	if (key == VK_RIGHT)
	{
		if (GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_RSHIFT))
		{
			if (end_sel == -1)
			{
				if (start_sel < strlen(Owner()->text.c_str()))
				{
					SetEndSel(start_sel + 1);
				}
			}
			else
			{
				if (end_sel < strlen(Owner()->text.c_str()))
				{
					SetEndSel(end_sel + 1);
				}
			}
		}
		else
		{
			if (end_sel != -1)
			{
				SetSel(start_sel > end_sel ? start_sel : end_sel);
			}
			else
			{
				if (start_sel < strlen(Owner()->text.c_str()))
				{
					SetSel(start_sel + 1);
				}
			}
		}
	}
	else
	if (key == VK_DELETE)
	{
		if (strlen(Owner()->text.c_str()) > start_sel)
		{
			if (!DeselectEnd())
			{
				Owner()->text.erase(start_sel, 1);
			}

			time2callback = 1.0f;
		}
	}
	else
	if (key == VK_BACK)
	{
		if (start_sel > 0)
		{
			if (!DeselectEnd())
			{
				Owner()->text.erase(start_sel - 1, 1);
				SetSel(start_sel - 1);
			}

			time2callback = 1.0f;
		}
	}
}

void WinDX11EditBox::OnCharDown(int key)
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

		DeselectEnd();

		Owner()->text.insert(start_sel, str);
		SetSel(start_sel + (int)strlen(str.c_str()));

		time2callback = 1.0f;
		Redraw();
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
	NativeEditBox::OnLeftMouseDown(ms_x, ms_y);

	mouse_pressed = true;
}

void WinDX11EditBox::OnLeftMouseUp(int ms_x, int ms_y)
{
	mouse_pressed = false;

	NativeEditBox::OnLeftMouseUp(ms_x, ms_y);
}

void WinDX11EditBox::OnMouseMove(int ms_x, int ms_y)
{
	if (mouse_pressed)
	{
		SetEndSel(theme->font.GetIndex(ms_x - 3 + offset, owner->GetText()));
	}
}

#endif