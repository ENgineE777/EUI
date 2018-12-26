
#include "EUIComboBox.h"
#include "WinDX11ComboBox.h"
#include "UTFConv.h"

#ifdef PLATFORM_WIN_DX11

WinDX11ComboBox::WinDX11ComboBox(EUIWidget* owner) : NativeComboBox(owner)
{
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
	items.clear();
	cur_string = -1;
}

void WinDX11ComboBox::AddItem(const char* str)
{
	items.push_back(str);
}

void WinDX11ComboBox::SetCurString(int index)
{
	cur_string = index;

	if (index != -1)
	{
		Owner()->text = items[cur_string];
	}
}

void WinDX11ComboBox::SetCurString(const char* str)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (strcmp(items[i].c_str(), str) == 0)
		{
			cur_string = i;
			Owner()->text = items[cur_string];

			break;
		}
	}
}

const char* WinDX11ComboBox::GetCurString()
{
	return Owner()->text.c_str();
}

int WinDX11ComboBox::GetCurStringIndex()
{
	return cur_string;
}

void WinDX11ComboBox::Draw()
{
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->Draw(is_howered ? "ComboBoxHowered" : "ComboBoxNormal", global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width - 20, owner->height);
	theme->font.Print(global_x + owner->x + 3, global_y + owner->y + 4, nullptr, Owner()->text.c_str());

	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->Draw("ComboBoxDownArrow", global_x + owner->x + owner->width - 20, global_y + owner->y + 3, 15, 15);

	if (is_opened)
	{
		theme->SetClampBorder(global_x + owner->x, global_y + owner->y + owner->height, owner->width, (int)fmin(6 + items.size() * 15, Owner()->list_height));
		theme->Draw("ComboBoxList", global_x + owner->x, global_y + owner->y + owner->height, owner->width, (int)fmin(6 + items.size() * 15, Owner()->list_height));

		int pos_y = global_y + owner->y + owner->height + 3;

		if (pre_sel_item != -1)
		{
			float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
			theme->Draw(nullptr, color, global_x + owner->x + 3, pos_y + pre_sel_item * 15, owner->width - 6, 15);
		}

		for (auto& item : items)
		{
			theme->font.Print(global_x + owner->x + 3, pos_y + 2, nullptr, item.c_str());
			pos_y += 15;
		}
	}

	NativeComboBox::Draw();
}

void WinDX11ComboBox::SetText(const char* txt)
{
	cur_string = -1;

	for (int i = 0; i < items.size(); i++)
	{
		if (strcmp(items[i].c_str(), txt) == 0)
		{
			cur_string = i;
			break;
		}
	}
}

bool WinDX11ComboBox::IsHitted(int ms_x, int ms_y)
{
	int list_height = is_opened ? (int)fmin(6 + items.size() * 15, Owner()->list_height) : 0;

	if (global_x + owner->x < ms_x && ms_x < global_x + owner->x + owner->width &&
		global_y + owner->y < ms_y && ms_y < global_y + owner->y + owner->height + list_height)
	{
		return true;
	}

	return false;
}

void WinDX11ComboBox::OnFocusLost()
{
	is_focused = false;

	if (is_opened)
	{
		is_opened = false;
		((WinDX11Widget*)owner->GetRoot()->nativeWidget)->over_widget = nullptr;
	}
}

void WinDX11ComboBox::OnMouseMove(int ms_x, int ms_y)
{
	pre_sel_item = -1;

	if (is_opened)
	{
		if (ms_y > owner->height)
		{
			pre_sel_item = (int)(((float)ms_y - 3.0f - owner->height) / 15.0f);

			if (pre_sel_item > items.size() - 1)
			{
				pre_sel_item = -1;
			}
		}
	}

	NativeComboBox::OnMouseMove(ms_x, ms_y);
}

void WinDX11ComboBox::OnLeftMouseDown(int ms_x, int ms_y)
{
	is_opened = !is_opened;

	if (is_opened)
	{
		if (Owner()->listener)
		{
			Owner()->listener->OnComboBoxOpened(Owner());
		}
	}

	((WinDX11Widget*)owner->GetRoot()->nativeWidget)->over_widget = is_opened ? this : nullptr;

	NativeComboBox::OnLeftMouseDown(ms_x, ms_y);
}

void WinDX11ComboBox::OnLeftMouseUp(int ms_x, int ms_y)
{
	if (pre_sel_item != -1 && pre_sel_item != cur_string)
	{
		cur_string = pre_sel_item;

		if (cur_string != -1)
		{
			Owner()->text = items[cur_string];
		}

		if (Owner()->listener)
		{
			Owner()->listener->OnComboBoxSelChange(Owner(), cur_string);
		}
	}

	NativeComboBox::OnLeftMouseUp(ms_x, ms_y);
}

#endif