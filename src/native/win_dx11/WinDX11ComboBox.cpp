
#include "EUIComboBox.h"
#include "WinDX11ComboBox.h"
#include "UTFConv.h"

#ifdef PLATFORM_WIN_DX11

WinDX11ComboBox::WinDX11ComboBox(EUIWidget* owner) : NativeComboBox(owner)
{
	/*handle = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
	                       (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                       ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));*/
}

WinDX11ComboBox::~WinDX11ComboBox()
{
}

EUIComboBox* WinDX11ComboBox::Owner()
{
	return (EUIComboBox*)owner;
}

void WinDX11ComboBox::ClearList()
{
	//ComboBox_ResetContent(handle);
}

void WinDX11ComboBox::AddItem(const char* str)
{
	/*std::wstring wtext;
	UTFConv::UTF8to16(wtext, str);

	SendMessageW(handle, CB_ADDSTRING, (WPARAM)0, (LPARAM)wtext.c_str());

	if (SendMessage(handle, CB_GETCOUNT, (WPARAM)0, (LPARAM)0) == 1)
	{
		SetCurString(0);
	}*/
}

void WinDX11ComboBox::SetCurString(int index)
{
	//ComboBox_SetCurSel(handle, index);
}

void WinDX11ComboBox::SetCurString(const char* str)
{
	/*std::wstring wtext;
	UTFConv::UTF8to16(wtext, str);

	SendMessageW(handle, CB_SELECTSTRING, (WPARAM)0, (LPARAM)wtext.c_str());*/
}

const char* WinDX11ComboBox::GetCurString()
{
	/*int index = GetCurStringIndex();

	if (index != -1)
	{
		Owner()->text.resize(ComboBox_GetLBTextLen(handle, index) + 1);
		ComboBox_GetLBText(handle, index, &Owner()->text[0]);

		return Owner()->text.c_str();
	}*/

	return "";
}

int WinDX11ComboBox::GetCurStringIndex()
{
	return -1;// ComboBox_GetCurSel(handle);
}
#endif