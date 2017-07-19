
#include "EUIEditBox.h"
#include "WinEditBox.h"
#include "UTFConv.h"

WinEditBox::WinEditBox(EUIWidget* owner) : NativeEditBox(owner)
{
	std::wstring wtext;
	UTFConv::UTF8to16(wtext, Owner()->text.c_str());

	handle = CreateWindowW(L"EDIT", wtext.c_str(), ES_AUTOHSCROLL | WS_CHILD | WS_BORDER | WS_VISIBLE | WS_TABSTOP,
							Owner()->x, Owner()->y, Owner()->width, Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), NULL, NULL, NULL );

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));
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

	if (msg == WM_CHAR)
	{
		if (wParam == VK_RETURN)
		{
			if (Owner()->listener)
			{
				Owner()->listener->OnEditBoxEnterPressed(Owner());
			}
		}
		else
		if (wParam != VK_RETURN && wParam != VK_BACK)
		{
			GetText();

			if (wParam == '-' &&
				(Owner()->inputType == EUIEditBox::InputInteger || Owner()->inputType == EUIEditBox::InputFloat) &&
				Owner()->text.size() > 0)
			{
				return false;
			}
			else
			if (wParam == '.' &&
				(Owner()->inputType == EUIEditBox::InputUFloat || Owner()->inputType == EUIEditBox::InputFloat) &&
				Owner()->text.find(".") != std::string::npos)
			{
				return false;
			}
			else
			if (wParam < '0' || wParam > '9')
			{
				return false;
			}
		}

		if (Owner()->listener)
		{
			Owner()->listener->OnEditBoxChange(Owner());
		}
	}

	return true;
}

void WinEditBox::SetText(const char* txt)
{
	int pos = LOWORD(Edit_GetSel(handle));

	if (pos > Owner()->text.length())
	{
		pos = Owner()->text.length();
	}

	WinWidget::SetText(txt);

	std::wstring wtext;
	UTFConv::UTF8to16(wtext, txt);

	SetWindowTextW(handle, wtext.c_str());
	Edit_SetSel(handle,pos,pos);
}

const char* WinEditBox::GetText()
{
	wchar_t wtmp[4096];
	GetWindowTextW(handle, wtmp, 4096);
	
	UTFConv::UTF16to8(Owner()->text, wtmp);

	return Owner()->text.c_str();
}