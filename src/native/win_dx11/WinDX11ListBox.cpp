
#include "EUIListBox.h"
#include "WinDX11ListBox.h"
#include "UTFConv.h"

#ifdef PLATFORM_WIN_DX11

WinDX11ListBox::WinDX11ListBox(EUIWidget* owner, bool abs_sort) : NativeListBox(owner)
{
	/*int flag = abs_sort ? LBS_SORT : 0;
	handle = CreateWindowW(L"ListBox", L"", WS_CHILD | WS_VSCROLL | WS_VISIBLE | LBS_NOTIFY | WS_BORDER | flag,
	                       (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                       ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));*/
}

WinDX11ListBox::~WinDX11ListBox()
{
}

EUIListBox* WinDX11ListBox::Owner()
{
	return (EUIListBox*)owner;
}

int WinDX11ListBox::FindIndexByData(void* data)
{
	/*int count = ListBox_GetCount(handle);
	for (int i = 0; i < count; i++)
	{
		if ((void*)ListBox_GetItemData(handle, i) == data)
		{
			return i;
		}
	}*/

	return -1;
}

void WinDX11ListBox::ClearList()
{
	/*while (SendMessage(handle, LB_GETCOUNT, 0, 0) != 0)
	{
		SendMessage(handle, LB_DELETESTRING, 0, 0);
	} */
}

void WinDX11ListBox::AddItem(const char* str, void* data)
{
	/*std::wstring text;
	UTFConv::UTF8to16(text, str);

	LRESULT sel = SendMessageW(handle, LB_ADDSTRING, 0, (LPARAM)text.c_str());

	SendMessage(handle, LB_SETITEMDATA ,(WPARAM)sel, (LPARAM)data);*/
}

void WinDX11ListBox::ChangeItemNameByIndex(const char* str, int index)
{
	/*bool needReselect = (index == GetSelectedItemIndex());
	void* data = (void*)ListBox_GetItemData(handle, index);
	DeleteItemByIndex(index);

	ListBox_InsertString(handle, index, (LPCSTR)str);
	SendMessage(handle, LB_SETITEMDATA, (WPARAM)index, (LPARAM)data);

	if (needReselect)
	{
		SelectItemByIndex(index);
	}*/
}

void WinDX11ListBox::ChangeItemNameByData(const char* str, void* data)
{
	ChangeItemNameByIndex(str, FindIndexByData(data));
}

int WinDX11ListBox::GetSelectedItemIndex()
{
	return -1;// ListBox_GetCurSel(handle);
}

void* WinDX11ListBox::GetSelectedItemData()
{
	/*int index = GetSelectedItemIndex();

	if (index == -1)
	{
		return NULL;
	}*/

	return nullptr;// (void*)ListBox_GetItemData(handle, index);
}

void WinDX11ListBox::SelectItemByIndex(int index)
{
	//ListBox_SetCurSel(handle, index);
}

void WinDX11ListBox::SelectItemByData(void* data)
{
	//SelectItemByIndex(FindIndexByData(data));
}

void WinDX11ListBox::DeleteItemByIndex(int index)
{
	//ListBox_DeleteString(handle, index);
}

void WinDX11ListBox::DeleteItemByData(void* data)
{
	/*int index = FindIndexByData(data);

	if (index != -1)
	{
		ListBox_DeleteString(handle, index);
	}*/
}
#endif