
#include "EUIComboBox.h"
#include "WinComboBox.h"
#include "UTFConv.h"

WinComboBox::WinComboBox(EUIWidget* owner) : NativeComboBox(owner)
{
	handle = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
							Owner()->x, Owner()->y, Owner()->width, Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), (HMENU)win_id, NULL, NULL);
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
				Owner()->listener->OnComboBoxChange(Owner(), ComboBox_GetCurSel(handle));
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

int WinComboBox::GetCurString()
{
	return ComboBox_GetCurSel(handle);
}