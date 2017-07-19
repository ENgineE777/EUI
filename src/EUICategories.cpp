
#include "EUICategories.h"
#include "native/win/WinCategories.h"

EUICategories::EUICategories(EUIWidget* prnt, float set_x, float set_y, float w, float h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = w;
	height = h;

	nativeWidget = new WinCategories(this);
}

EUICategories::~EUICategories()
{
}

NativeCategories* EUICategories::Native()
{
	return (NativeCategories*)nativeWidget;
}

void EUICategories::RegisterChildInCategory(const char* name, EUIWidget* widget)
{
	Category* category = NULL;

	for (int i = 0; i < categories.size(); i++)
	{
		if (stricmp(categories[i].name, name) == 0)
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
		category->y = (categories.size() - 1) * 25;
		category->opened = true;
	}
	
	category->childs.push_back(widget);
	
	Native()->UpdateChildPos();
}
