
#include "EUITabPanel.h"
#include "WinDX11TabPanel.h"

#ifdef PLATFORM_WIN_DX11

WinDX11TabPanel::WinDX11TabPanel(EUIWidget* owner) : NativeTabPanel(owner)
{
}

WinDX11TabPanel::~WinDX11TabPanel()
{
}

EUITabPanel* WinDX11TabPanel::Owner()
{
	return (EUITabPanel*)owner;
}

void WinDX11TabPanel::Draw()
{
	int capption_height = 27;

	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->Draw("TabPanelLine", global_x + owner->x, global_y + owner->y + capption_height - 6, owner->width, 6);

	for (int i = 0; i < (int)owner->childs.size(); i++)
	{
		const char* elem = (i == curTab) ? "TabPanelCaptionActive" : "TabPanelCaption";

		if (i == howeredTab)
		{
			elem = (i == curTab || mouse_pressed) ? "TabPanelCaptionActiveSelected" : "TabPanelCaptionSelected";
		}

		theme->Draw(elem, global_x + owner->x + 3 + 50 * i, global_y + owner->y, 48, capption_height);
		theme->SetClampBorder(global_x + owner->x + 3 + 50 * i, global_y + owner->y, 48, capption_height);
		theme->font.Print(global_x + owner->x + 3 + 50 * i + theme->font.CalcOffset(tab_names[i].c_str(), 48), global_y + owner->y + 7, nullptr, tab_names[i].c_str());
		theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	}

	theme->Draw("TabPanelSheet", global_x + owner->x, global_y + owner->y + capption_height, owner->width, owner->height - capption_height);

	NativeTabPanel::Draw();
}

void WinDX11TabPanel::Resize()
{
	for (int i = 0; i < (int)owner->childs.size(); i++)
	{
		owner->childs[i]->SetPos(4, 31);
		owner->childs[i]->SetSize(Owner()->width - 8, Owner()->height - 35);
	}

	NativeTabPanel::Resize();
}

void WinDX11TabPanel::AddTab(const char* txt)
{
	if (owner->childs.size() == 1)
	{
		curTab = 0;
	}

	tab_names.push_back(txt);
}

int WinDX11TabPanel::GetCurrentTabIndex()
{
	return curTab;
}

void WinDX11TabPanel::DeleteTab(int index)
{
	tab_names.erase(tab_names.begin() + index);
}

void WinDX11TabPanel::ClearTabs()
{
	tab_names.clear();

	curTab = -1;
}

void WinDX11TabPanel::SetTabName(int index, const char* name)
{
	tab_names[index] = name;
}

void WinDX11TabPanel::SetCurrentTab(int index)
{
	ShowTab(curTab, false);
	curTab = index;
	ShowTab(curTab, true);
}

void WinDX11TabPanel::ShowTab(int index, bool show)
{
	owner->childs[index]->Show(show);
}

void WinDX11TabPanel::OnMouseMove(int ms_x, int ms_y)
{
	howeredTab = -1;

	if (0 < ms_y && ms_y < 27)
	{
		for (int i = 0; i < (int)owner->childs.size(); i++)
		{
			if (3 + 50 * i < ms_x && ms_x < 3 + 50 * i + 48)
			{
				howeredTab = i;
			}
		}
	}

	NativeTabPanel::OnMouseMove(ms_x, ms_y);
}

void WinDX11TabPanel::OnMouseLeave()
{
	howeredTab = -1;
	NativeTabPanel::OnMouseLeave();
}

void WinDX11TabPanel::OnLeftMouseUp(int ms_x, int ms_y)
{
	if (howeredTab != -1)
	{
		SetCurrentTab(howeredTab);

		if (Owner()->listener)
		{
			Owner()->listener->OnTabChange(Owner(), curTab);
		}
	}

	NativeTabPanel::OnLeftMouseUp(ms_x, ms_y);
}

#endif