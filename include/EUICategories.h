
#pragma once

#include "EUIWidget.h"
#include "Native/NativeCategories.h"

class EUICategories : public EUIWidget
{
	friend class WinCategories;

	struct Category
	{
		bool  opened;
		char  name[128];
		std::vector<EUIWidget*> childs;
		float y;

		Category()
		{
			opened = false;
			name[0] = 0;
			y = 0;
		};
	};

	std::vector<Category> categories;

	NativeCategories* Native();

public:

	EUICategories(int id, EUIWidget* parent, float x, float y, float w, float h);
	virtual ~EUICategories();

	virtual void RegisterChildInCategory(const char* name, EUIWidget* widget);
};
