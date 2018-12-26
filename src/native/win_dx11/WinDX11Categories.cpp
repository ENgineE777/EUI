
#include "EUICategories.h"
#include "WinDX11Categories.h"
#include "EUIScrollBar.h"
#include "WinDX11ScrollBar.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Categories::WinDX11Categories(EUIWidget* owner) : NativeCategories(owner)
{
	overallHeight = 0;

	if (!Owner()->auto_size)
	{
		Owner()->nativeWidget = this;
		scrollbar = new EUIScrollBar(Owner(), false, Owner()->width - 15, 0, 15, Owner()->height);
		scrollbar->Show(false);
		scrollbar->SetListener(0, this, 0);
	}
}

WinDX11Categories::~WinDX11Categories()
{
}

EUICategories* WinDX11Categories::Owner()
{
	return (EUICategories*)owner;
}

void WinDX11Categories::OnSrollerPosChange(EUIScrollBar* sender, int pos)
{
	UpdateChildPos();
	Redraw();
}

void WinDX11Categories::CalcThumb()
{
	if (!scrollbar)
	{
		return;
	}

	overallHeight = 0;

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		if (!category.visible)
		{
			continue;
		}

		overallHeight += theme->categoryHeight;

		for (int j = 0; j < (int)category.childs.size(); j++)
		{
			if (!category.childsVis[j])
			{
				continue;
			}

			if (category.opened)
			{
				overallHeight += category.childs[j]->GetHeight();
			}
		}
	}

	if (!Owner()->auto_size)
	{
		scrollbar->SetPos(Owner()->width - 15, 0);
		scrollbar->SetSize(15, Owner()->height);

		float delta = overallHeight - Owner()->height;
		scrollbar->Show(delta > 0.0f);

		if (delta > 0.0f)
		{
			scrollbar->SetLimit(1, (int)delta);
		}
	}
}

void WinDX11Categories::UpdateChildPos()
{
	Owner()->allowCallOnChildShow = false;

	int pos = 0;

	if (scrollbar && scrollbar->IsVisible())
	{
		pos = -scrollbar->GetPosition();
	}

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		category.y = pos;
		pos += theme->categoryHeight;

		category.visible = false;

		for (int j = 0; j < (int)category.childs.size(); j++)
		{
			if (!category.childsVis[j])
			{
				continue;
			}

			category.visible = true;
			category.childs[j]->Show(category.opened);

			if (category.opened)
			{
				category.childs[j]->SetPos(category.childs[j]->GetX(), (int)pos);
				((WinDX11Widget*)category.childs[j]->nativeWidget)->CalcGlopalPos();
				pos += category.childs[j]->GetHeight();
			}
		}

		if (!category.visible)
		{
			pos -= theme->categoryHeight;
		}
	}

	if (Owner()->auto_size)
	{
		Owner()->SetSize(Owner()->width, (int)pos);

		if (Owner()->parent)
		{
			EUICategories* cat = dynamic_cast<EUICategories*>(Owner()->parent);

			if (cat)
			{
				cat->Native()->UpdateChildPos();
			}
		}
	}

	Owner()->allowCallOnChildShow = true;
}

void WinDX11Categories::Resize()
{
	CalcThumb();
	UpdateChildPos();
	NativeCategories::Resize();
}

void WinDX11Categories::Draw()
{
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->Draw("CategoriesView", global_x + owner->x, global_y + owner->y, owner->width, owner->height);

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		if (!category.visible)
		{
			continue;
		}

		theme->Draw(i == howered_category ? "CategoriesCategoryHowered" : "CategoriesCategory", global_x + owner->x + 3, global_y + owner->y + category.y, owner->width - 6, theme->categoryHeight);
		theme->Draw(category.opened ? "CategoriesCategoryMinus" : "CategoriesCategoryPlus", global_x + owner->x + 5, global_y + owner->y + category.y + 3, 15, 15);

		theme->SetClampBorder(global_x + owner->x + 3, global_y + owner->y + category.y, owner->width - 6, theme->categoryHeight);
		theme->font.Print(global_x + owner->x + 3 + 22, global_y + owner->y + category.y + 4, nullptr, category.name);
		theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	}

	NativeCategories::Draw();
}

void WinDX11Categories::OnMouseMove(int ms_x, int ms_y)
{
	howered_category = -1;

	int index = 0;
	for (auto& category : Owner()->categories)
	{
		if (!category.visible)
		{
			index++;
			continue;
		}

		if (category.y < ms_y && ms_y < category.y + theme->categoryHeight)
		{
			howered_category = index;

			break;
		}

		index++;
	}

	NativeCategories::OnMouseMove(ms_x, ms_y);
}

void WinDX11Categories::OnMouseLeave()
{
	howered_category = -1;
	NativeCategories::OnMouseLeave();
}

void WinDX11Categories::OnLeftMouseDown(int ms_x, int ms_y)
{
	NativeCategories::OnLeftMouseDown(ms_x, ms_y);
}

void WinDX11Categories::OnLeftMouseUp(int ms_x, int ms_y)
{
	if (howered_category != -1)
	{
		Owner()->categories[howered_category].opened = !Owner()->categories[howered_category].opened;
		CalcThumb();
		UpdateChildPos();
	}

	NativeCategories::OnLeftMouseUp(ms_x, ms_y);
}
#endif
