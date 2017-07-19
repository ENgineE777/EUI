
#include "EUIListBox.h"
#include "WinListBox.h"
#include "UTFConv.h"

WinListBox::WinListBox(EUIWidget* owner) : NativeListBox(owner)
{
	handle = CreateWindowW(L"ListBox", L"", WS_CHILD | WS_VSCROLL | WS_VISIBLE | LBS_NOTIFY | WS_BORDER,
							Owner()->x, Owner()->y, Owner()->width, Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), NULL, NULL, NULL);

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));
}

WinListBox::~WinListBox()
{
}

EUIListBox* WinListBox::Owner()
{
	return (EUIListBox*)owner;
}

bool WinListBox::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeListBox::ProcessWidget(msg, wParam, lParam);
 
	if (msg == 513)
	{
		if (Owner()->listener)
		{
			Owner()->listener->OnListBoxChange(Owner(), ListBox_GetCurSel(handle));
		}
	}
	else
	if (msg == 515)
	{
		if (Owner()->listener)
		{
			Owner()->listener->OnListBoxDblClick(Owner(), ListBox_GetCurSel(handle));
		}
	}

	return true;
}

void WinListBox::ClearList()
{
	while (SendMessage(handle, LB_GETCOUNT, 0, 0) != 0)
	{
		SendMessage(handle, LB_DELETESTRING, 0, 0);
	} 
}

void WinListBox::AddItem(const char* str, void* data)
{
	std::wstring wtext;
	UTFConv::UTF8to16(wtext, str);

	int sel = SendMessageW(handle, LB_ADDSTRING, 0, (LPARAM)wtext.c_str()); 

	SendMessage(handle, LB_SETITEMDATA ,(WPARAM) sel, (LPARAM)data);
	SendMessage(handle, LB_SETTOPINDEX ,(WPARAM) sel, 0);
}

int WinListBox::GetSelectedIndex()
{
	return ListBox_GetCurSel(handle);
}
