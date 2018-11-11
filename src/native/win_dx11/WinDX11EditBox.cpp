
#include "EUIEditBox.h"
#include "WinDX11EditBox.h"
#include "UTFConv.h"
#include "EUIWindow.h"
#include "WinDX11Window.h"

#ifdef PLATFORM_WIN_DX11

int WinDX11EditBox::timerID = 0;

std::vector<WinDX11EditBox*> win_dx11_boxes;

VOID CALLBACK WinDX11EditBoxTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	for (auto edit_box : win_dx11_boxes)
	{
		if (edit_box->cur_timerID == idTimer)
		{
			edit_box->OnTimer();
			break;
		}
	}
}

WinDX11EditBox::WinDX11EditBox(EUIWidget* owner) : NativeEditBox(owner)
{
	cur_timerID = timerID;
	SetTimer(((WinDX11Window*)owner->GetRoot()->nativeWidget)->handle, cur_timerID, 10, (TIMERPROC)WinDX11EditBoxTimerProc);
	timerID++;

	win_dx11_boxes.push_back(this);
}

WinDX11EditBox::~WinDX11EditBox()
{
}

EUIEditBox* WinDX11EditBox::Owner()
{
	return (EUIEditBox*)owner;
}

void WinDX11EditBox::SetText(const char* txt)
{
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

	theme->font.Print(global_x + owner->x + 3, global_y + owner->y + 5, font_color, owner->GetText());

	if (is_focused)
	{
		int offset = theme->font.CalcWidth(owner->GetText());

		theme->Draw(nullptr, font_color, global_x + owner->x + 3 + offset + 1, global_y + owner->y + 2, 1, owner->height - 4);
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
		Owner()->text += " ";

		time2callback = 1.0f;
		Redraw();
	}
	else
	if (key == VK_BACK)
	{
		if (Owner()->text.size() > 0)
		{
			Owner()->text.pop_back();
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

			Owner()->text += str;

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
#endif