
#include "EUIComboBox.h"
#include "WinComboBox.h"
#include "UTFConv.h"

#ifdef PLATFORM_WIN

WinComboBox::WinComboBox(EUIWidget* owner) : NativeComboBox(owner)
{
	handle = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
	                       (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                       ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));
}

WinComboBox::~WinComboBox()
{
}

EUIComboBox* WinComboBox::Owner()
{
	return (EUIComboBox*)owner;
}

bool WinComboBox::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeComboBox::ProcessWidget(msg, wParam, lParam);

	if (msg == WM_COMMAND)
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			if (Owner()->listener)
			{
				Owner()->listener->OnComboBoxSelChange(Owner(), ComboBox_GetCurSel(handle));
			}
		}
	}

	return true;
}

void WinComboBox::ClearList()
{
	ComboBox_ResetContent(handle);
}

void WinComboBox::AddItem(const char* str)
{
	std::wstring wtext;
	UTFConv::UTF8to16(wtext, str);

	SendMessageW(handle, CB_ADDSTRING, (WPARAM)0, (LPARAM)wtext.c_str());

	if (SendMessage(handle, CB_GETCOUNT, (WPARAM)0, (LPARAM)0) == 1)
	{
		SetCurString(0);
	}
}

void WinComboBox::SetCurString(int index)
{
	ComboBox_SetCurSel(handle, index);
}

void WinComboBox::SetCurString(const char* str)
{
	std::wstring wtext;
	UTFConv::UTF8to16(wtext, str);

	SendMessageW(handle, CB_SELECTSTRING, (WPARAM)0, (LPARAM)wtext.c_str());
}

const char* WinComboBox::GetCurString()
{
	int index = GetCurStringIndex();

	if (index != -1)
	{
		Owner()->text.resize(ComboBox_GetLBTextLen(handle, index) + 1);
		ComboBox_GetLBText(handle, index, &Owner()->text[0]);

		return Owner()->text.c_str();
	}

	return "";
}

int WinComboBox::GetCurStringIndex()
{
	return ComboBox_GetCurSel(handle);
}
#endif