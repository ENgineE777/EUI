
#include "EUITabPanel.h"
#include "WinTabPanel.h"

WinTabPanel::WinTabPanel(EUIWidget* owner) : NativeTabPanel(owner)
{
	handle = CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE,
							Owner()->x, Owner()->y, Owner()->width, Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), NULL, NULL, NULL);

	SetWindowLong(handle, GWL_ID, Owner()->id);

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

	if (msg == 15)
	{
		SetCurrentTab(TabCtrl_GetCurSel(handle));

		if (Owner()->listiner)
		{
			Owner()->listiner->OnTabChange(Owner(), curTab);
		}
	}

	return true;
}

void WinTabPanel::Resize()
{
	for (int i = 0; i < tabs.size(); i++)
	{
		SetWindowPos(tabs[i], 0, 0, 0, Owner()->width - 8, Owner()->height - 35, SWP_NOMOVE | SWP_NOZORDER);
	}

	NativeTabPanel::Resize();
}

void WinTabPanel::AddTab(const char* txt)
{
	HWND hnd = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | SS_OWNERDRAW | SS_NOTIFY,
							5, 30, Owner()->width - 8, Owner()->height - 35,
							handle, NULL, NULL, NULL);

	SetWindowLong(hnd, GWL_ID, Owner()->id);

	SetWindowSubclass(hnd, &WinWidgetProc, 0, (DWORD_PTR)this);

	tabs.push_back(hnd);

	if (tabs.size() == 1)
	{
		curTab = 0;

		ShowTab(0, true);
	}

	TCITEM tie; 
	tie.mask = TCIF_TEXT; 
	tie.iImage = -1; 
	tie.pszText = (LPSTR)txt;
	TabCtrl_InsertItem(handle, tabs.size()-1, &tie);
}

void WinTabPanel::SetTabName(int index, const char* name)
{
	if (index < tabs.size())
	{
		TCITEM tie; 
		tie.mask = TCIF_TEXT; 
		tie.iImage = -1; 
		tie.pszText = (LPSTR)name;
		TabCtrl_SetItem(handle, index, &tie);
	}
}

void WinTabPanel::DelTab(int index)
{
	if (index < 0 || index >= tabs.size())
	{
		return;
	}

	TabCtrl_DeleteItem(handle, index);
	tabs.erase(tabs.begin() + index);
}

void WinTabPanel::ClearTabs()
{
	tabs.clear();
	curTab = -1;
	TabCtrl_DeleteAllItems(handle);
}

void WinTabPanel::AddWidget2Tab(int index, EUIWidget* widget)
{
	if (index < 0 || index >= tabs.size())
	{
		return;
	}

	SetParent(((WinWidget*)widget->nativeWidget)->GetHandle(), tabs[index]);
}

void WinTabPanel::SetCurrentTab(int index)
{
	if(index >= 0)
	{
		if (curTab == index || index < 0 || index >= tabs.size())
		{
			return;
		}

		TabCtrl_SetCurSel(handle,index);
		ShowTab(curTab, false);
		curTab = index;
		ShowTab(curTab, true);
	}
}

int WinTabPanel::GetCurrentTab()
{
	return curTab;
}

void WinTabPanel::ShowTab(int index, bool show)
{
	if (index<0 || index>=tabs.size()) return;

	ShowWindow(tabs[index], show);
}