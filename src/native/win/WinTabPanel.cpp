
#include "EUITabPanel.h"
#include "WinTabPanel.h"

WinTabPanel::WinTabPanel(EUIWidget* owner) : NativeTabPanel(owner)
{
	handle = CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE,
							(int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));

	curTab = -1;
}

WinTabPanel::~WinTabPanel()
{
}

EUITabPanel* WinTabPanel::Owner()
{
	return (EUITabPanel*)owner;
}

bool WinTabPanel::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeTabPanel::ProcessWidget(msg, wParam, lParam);

	if (msg == WM_NOTIFY)
	{
		if (((LPNMHDR)lParam)->code == TCN_SELCHANGE)
		{
			SetCurrentTab(TabCtrl_GetCurSel(handle));

			if (Owner()->listener)
			{
				Owner()->listener->OnTabChange(Owner(), curTab);
			}
		}
	}

	return true;
}

void WinTabPanel::Resize()
{
	for (int i = 0; i < (int)owner->childs.size(); i++)
	{
		owner->childs[i]->SetSize(Owner()->width - 12, Owner()->height - 30);
	}

	NativeTabPanel::Resize();
}

void WinTabPanel::AddTab(const char* txt, HWND hnd)
{
	if (owner->childs.size() == 1)
	{
		curTab = 0;
	}

	TCITEM tie; 
	tie.mask = TCIF_TEXT; 
	tie.iImage = -1; 
	tie.pszText = (LPSTR)txt;
	TabCtrl_InsertItem(handle, owner->childs.size()-1, &tie);
}

int WinTabPanel::GetCurrentTabIndex()
{
	return curTab;
}

void WinTabPanel::DeleteTab(int index)
{
	if (index < 0 || index >= (int)owner->childs.size())
	{
		return;
	}

	//TODO:
	//need delete child
	owner->childs.erase(owner->childs.begin() + index);
	TabCtrl_DeleteItem(handle, index);
}

void WinTabPanel::ClearTabs()
{
	//TODO:
	//need delete childs

	owner->childs.clear();

	curTab = -1;
	TabCtrl_DeleteAllItems(handle);
}

void WinTabPanel::SetTabName(int index, const char* name)
{
	if (index < (int)owner->childs.size())
	{
		TCITEM tie; 
		tie.mask = TCIF_TEXT; 
		tie.iImage = -1; 
		tie.pszText = (LPSTR)name;
		TabCtrl_SetItem(handle, index, &tie);
	}
}

void WinTabPanel::SetCurrentTab(int index)
{
	if (index >= 0)
	{
		if (curTab == index || index < 0 || index >= (int)owner->childs.size())
		{
			return;
		}

		TabCtrl_SetCurSel(handle,index);
		ShowTab(curTab, false);
		curTab = index;
		ShowTab(curTab, true);
	}
}

void WinTabPanel::ShowTab(int index, bool show)
{
	if (index<0 || index>= (int)owner->childs.size()) return;

	owner->childs[index]->Show(show);
}