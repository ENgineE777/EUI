
#include "EUIEditBox.h"
#include "WinEditBox.h"
#include "UTFConv.h"

int WinEditBox::timerID = 0;

WinEditBox::WinEditBox(EUIWidget* owner) : NativeEditBox(owner)
{
	std::wstring wtext;
	UTFConv::UTF8to16(wtext, Owner()->text.c_str());

	handle = CreateWindowW(L"EDIT", wtext.c_str(), ES_AUTOHSCROLL | WS_CHILD | WS_BORDER | WS_VISIBLE | WS_TABSTOP,
							(int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL );
	win_id++;

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));
	cur_timerID = timerID;
	SetTimer(handle, cur_timerID, 10, (TIMERPROC)NULL);
	timerID++;
}

WinEditBox::~WinEditBox()
{
}

EUIEditBox* WinEditBox::Owner()
{
	return (EUIEditBox*)owner;
}

bool WinEditBox::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeEditBox::ProcessWidget(msg, wParam, lParam);

	if (msg == WM_TIMER && (wParam == cur_timerID))
	{
		if (time2callback > 0.0f)
		{
			time2callback -= 0.01f;

			if (time2callback< 0.0f)
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

	if (msg == WM_COMMAND)
	{
		if (HIWORD(wParam) == EN_CHANGE)
		{
			time2callback = 1.0f;
			Redraw();
		}
	}

	if (msg == WM_CTLCOLOREDIT)
	{
		if (time2callback > 0.0f)
		{
			SetTextColor((HDC)wParam, RGB(255, 0, 0));
		}
		else
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
		}

		processRes = true;
		return false;
	}

	if (msg == WM_CHAR)
	{
		if (wParam == VK_RETURN)
		{
			time2callback = -1.0f;

			if (Owner()->listener)
			{
				Owner()->listener->OnEditBoxStopEditing(Owner());
			}

			Redraw();
		}
		else
		if (wParam != VK_RETURN && wParam != VK_BACK)
		{
			GetText();

			if (wParam == '-')
			{
				if (Owner()->text.find('-') != std::string::npos)
				{
					return false;
				}

				if (Owner()->inputType == EUIEditBox::InputUInteger || Owner()->inputType == EUIEditBox::InputUFloat)
				{
					return false;
				}

				if ((Owner()->inputType == EUIEditBox::InputInteger || Owner()->inputType == EUIEditBox::InputFloat) && (LOWORD(Edit_GetSel(handle) != 0)))
				{
					return false;
				}
			}
			else
			if (wParam == '.')
			{
				if (Owner()->text.find('.') != std::string::npos)
				{
					return false;
				}

				if (Owner()->inputType == EUIEditBox::InputInteger || Owner()->inputType == EUIEditBox::InputUInteger)
				{
					return false;
				}
			}
			else
			if ((wParam < '0' || wParam > '9') &&
				(Owner()->inputType != EUIEditBox::InputText))
			{
				return false;
			}

			time2callback = 1.0f;
			Redraw();
		}
	}

	return true;
}

void WinEditBox::SetText(const char* txt)
{
	int pos = LOWORD(Edit_GetSel(handle));

	if (pos > (int)Owner()->text.length())
	{
		pos = (int)Owner()->text.length();
	}

	WinWidget::SetText(txt);

	std::wstring wtext;
	UTFConv::UTF8to16(wtext, txt);

	SetWindowTextW(handle, wtext.c_str());
	Edit_SetSel(handle,pos,pos);

	time2callback = -1.0f;
	Redraw();
}

const char* WinEditBox::GetText()
{
	wchar_t wtmp[4096];
	GetWindowTextW(handle, wtmp, 4096);
	
	UTFConv::UTF16to8(Owner()->text, wtmp);

	return Owner()->text.c_str();
}