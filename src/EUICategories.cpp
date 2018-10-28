
#include "EUICategories.h"

#ifdef PLATFORM_WIN
#include "native/win/WinCategories.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11Categories.h"
#endif

EUICategories::EUICategories(EUIWidget* prnt, int set_x, int set_y, int w, int h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = w;

	if (h == -1)
	{
		auto_size = true;
		height = 5;
	}
	else
	{
		height = h;
	}

	allowCallOnChildShow = true;

#ifdef PLATFORM_WIN
	nativeWidget = new WinCategories(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11Categories(this);
#endif
}

EUICategories::~EUICategories()
{
}

NativeCategories* EUICategories::Native()
{
	return (NativeCategories*)nativeWidget;
}

void EUICategories::OnChildShow(int index, bool set)
{
	if (!allowCallOnChildShow)
	{
		return;
	}

	for (int i = 0; i < (int)categories.size(); i++)
	{
		Category& category = categories[i];

		for (int j = 0; j < (int)category.childs.size(); j++)
		{
			if (category.childs[j] == childs[index])
			{
				category.childsVis[j] = set;
				break;
			}
		}
	}

	Native()->UpdateChildPos();
}

void EUICategories::RegisterChildInCategory(const char* name, EUIWidget* widget)
{
	Category* category = NULL;

	for (int i = 0; i < (int)categories.size(); i++)
	{
		if (_stricmp(categories[i].name, name) == 0)
		{
			category = &categories[i];
			break;
		}
	}

	if (!category)
	{
		categories.push_back(Category());
		category = &categories[categories.size() - 1];
		strcpy(category->name, name);
		category->y = ((float)categories.size() - 1.0f) * 25.0f;
		category->opened = true;
	}
	
	category->childs.push_back(widget);
	category->childsVis.push_back(widget->visible);
	
	Native()->UpdateChildPos();
}
