
#include "EUIListBox.h"
#include "WinListBox.h"
#include "UTFConv.h"

#ifdef PLATFORM_WIN

WinListBox::WinListBox(EUIWidget* owner, bool abs_sort) : NativeListBox(owner)
{
	int flag = abs_sort ? LBS_SORT : 0;
	handle = CreateWindowW(L"ListBox", L"", WS_CHILD | WS_VSCROLL | WS_VISIBLE | LBS_NOTIFY | WS_BORDER | flag,
	                       (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                       ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

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
 
	if (msg == WM_COMMAND)
	{
		if (HIWORD(wParam) == LBN_SELCHANGE)
		{
			if (Owner()->listener)
			{
				Owner()->listener->OnListBoxSelChange(Owner(), ListBox_GetCurSel(handle));
			}
		}
		else
		if (HIWORD(wParam) == LBN_DBLCLK)
		{
			if (Owner()->listener)
			{
				Owner()->listener->OnListBoxDblClick(Owner(), ListBox_GetCurSel(handle));
			}
		}
	}

	return true;
}

int WinListBox::FindIndexByData(void* data)
{
	int count = ListBox_GetCount(handle);
	for (int i = 0; i < count; i++)
	{
		if ((void*)ListBox_GetItemData(handle, i) == data)
		{
			return i;
		}
	}

	return -1;
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
	std::wstring text;
	UTFConv::UTF8to16(text, str);

	LRESULT sel = SendMessageW(handle, LB_ADDSTRING, 0, (LPARAM)text.c_str());

	SendMessage(handle, LB_SETITEMDATA ,(WPARAM)sel, (LPARAM)data);
}

void WinListBox::ChangeItemNameByIndex(const char* str, int index)
{
	bool needReselect = (index == GetSelectedItemIndex());
	void* data = (void*)ListBox_GetItemData(handle, index);
	DeleteItemByIndex(index);

	ListBox_InsertString(handle, index, (LPCSTR)str);
	SendMessage(handle, LB_SETITEMDATA, (WPARAM)index, (LPARAM)data);

	if (needReselect)
	{
		SelectItemByIndex(index);
	}
}

void WinListBox::ChangeItemNameByData(const char* str, void* data)
{
	ChangeItemNameByIndex(str, FindIndexByData(data));
}

const char* WinListBox::GetSelectedItemText()
{
	return nullptr;
}

int WinListBox::GetSelectedItemIndex()
{
	return ListBox_GetCurSel(handle);
}

void* WinListBox::GetSelectedItemData()
{
	int index = GetSelectedItemIndex();

	if (index == -1)
	{
		return NULL;
	}

	return (void*)ListBox_GetItemData(handle, index);
}

void WinListBox::SelectItemByText(const char* text)
{
}

void WinListBox::SelectItemByIndex(int index)
{
	ListBox_SetCurSel(handle, index);
}

void WinListBox::SelectItemByData(void* data)
{
	SelectItemByIndex(FindIndexByData(data));
}

void WinListBox::DeleteItemByText(const char* text)
{
}

void WinListBox::DeleteItemByIndex(int index)
{
	ListBox_DeleteString(handle, index);
}

void WinListBox::DeleteItemByData(void* data)
{
	int index = FindIndexByData(data);

	if (index != -1)
	{
		ListBox_DeleteString(handle, index);
	}
}
#endif