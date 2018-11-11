
#include "EUIListBox.h"
#include "WinDX11ListBox.h"
#include "UTFConv.h"
#include "EUIScrollBar.h"
#include "WinDX11ScrollBar.h"

#ifdef PLATFORM_WIN_DX11

WinDX11ListBox::WinDX11ListBox(EUIWidget* owner, bool abs_sort) : NativeListBox(owner)
{
	Owner()->nativeWidget = this;
	scrollbar = new EUIScrollBar(Owner(), false, Owner()->width - 15, 0, 15, Owner()->height);
	scrollbar->Show(false);
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
	int index = 0;

	for (auto& item : items)
	{
		if (item.ptr == data)
		{
			return index;
		}

		index++;
	}

	return -1;
}

void WinDX11ListBox::InnerSelection(int index)
{
	if (sel_item != index)
	{
		sel_item = index;

		if (Owner()->listener)
		{
			Owner()->listener->OnListBoxSelChange(Owner(), sel_item);
		}
	}
}

void WinDX11ListBox::ClearList()
{
	items.clear();
}

void WinDX11ListBox::AddItem(const char* str, void* data)
{
	items.emplace_back();
	Item& item = items.back();

	item.text = str;
	item.ptr = data;

	CalcThumb();
}

void WinDX11ListBox::ChangeItemNameByIndex(const char* str, int index)
{
	items[index].text = str;
}

void WinDX11ListBox::ChangeItemNameByData(const char* str, void* data)
{
	ChangeItemNameByIndex(str, FindIndexByData(data));
}

int WinDX11ListBox::GetSelectedItemIndex()
{
	return sel_item;
}

void* WinDX11ListBox::GetSelectedItemData()
{
	if (sel_item == -1)
	{
		return nullptr;
	}

	return items[sel_item].ptr;
}

void WinDX11ListBox::SelectItemByIndex(int index)
{
	InnerSelection(index);
}

void WinDX11ListBox::SelectItemByData(void* data)
{
	for (int index = 0; index < items.size(); index++)
	{
		if (items[index].ptr == data)
		{
			sel_item = index;
			break;
		}
	}
}

void WinDX11ListBox::DeleteItemByIndex(int index)
{
	if (sel_item == index)
	{
		InnerSelection(-1);
	}

	items.erase(items.begin() + index);

	CalcThumb();
}

void WinDX11ListBox::DeleteItemByData(void* data)
{
	DeleteItemByIndex(FindIndexByData(data));
}

void WinDX11ListBox::CalcThumb()
{
	int height = (int)items.size() * 15;

	scrollbar->SetPos(Owner()->width - 15, 0);
	scrollbar->SetSize(15, Owner()->height);

	int delta = height - Owner()->height + 6;
	scrollbar->Show(delta > 0);

	if (delta > 0.0f)
	{
		scrollbar->SetLimit(1, delta);
	}
}

void WinDX11ListBox::Resize()
{
	CalcThumb();
	NativeListBox::Resize();
}

void WinDX11ListBox::Draw()
{
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->Draw("ListView", global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->SetClampBorder(global_x + owner->x + 3, global_y + owner->y + 3, owner->width - 6, owner->height - 6);

	int pos_y = global_y + owner->y + 3 - (scrollbar->IsVisible() ? scrollbar->GetPosition() : 0);

	if (sel_item != -1)
	{
		float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		theme->Draw(nullptr, color, global_x + owner->x + 3, pos_y + sel_item * 15, owner->width - 6, 15);
	}

	for (auto& item : items)
	{
		theme->font.Print(global_x + owner->x + 3, pos_y + 2, nullptr, item.text.c_str());
		pos_y += 15;
	}

	NativeListBox::Draw();
}

void WinDX11ListBox::OnLeftMouseDown(int ms_x, int ms_y)
{
	prev_sel_item = sel_item;
	sel_item = (int)(((float)ms_y - 3.0f) / 15.0f);

	if (sel_item > items.size() - 1)
	{
		sel_item = -1;
	}

	NativeListBox::OnLeftMouseDown(ms_x, ms_y);
}

void WinDX11ListBox::OnLeftMouseUp(int ms_x, int ms_y)
{
	if (prev_sel_item != sel_item)
	{
		if (Owner()->listener)
		{
			Owner()->listener->OnListBoxSelChange(Owner(), sel_item);
		}
	}

	NativeListBox::OnLeftMouseUp(ms_x, ms_y);
}

#endif