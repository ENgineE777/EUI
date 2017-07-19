
#include "EUILayout.h"
#include "native/win/WinLayout.h"

EUILayout::EUILayout(EUIWidget* prnt, bool set_vertical) : EUIWidget(prnt, "")
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;

	vertical = set_vertical;
	nativeWidget = new WinLayout(this);
}

EUILayout::~EUILayout()
{

}

NativeLayout* EUILayout::Native()
{
	return (NativeLayout*)nativeWidget;
}

void EUILayout::Resize()
{
	int size = 0;

	if (vertical)
	{
		size = parent->GetHeight();
	}
	else
	{
		size = parent->GetWidth();
	}

	int sz = 0;
	int cnt = 0;

	for (int i = 0; i < childs.size(); i++)
	{
		if (childs_size[i].size > 0)
		{
			sz += (childs_size[i].absolute ? childs_size[i].size * size : childs_size[i].size);
		}
		else
		{
			cnt++;
		}
	}

	int elem_size = (float)(size - sz) / (float)cnt;

	if (size < 0)
	{
		size = 0;
	}

	int pos = 0;

	for (int i = 0; i < childs.size(); i++)
	{
		int sz = elem_size;

		if (childs_size[i].size > 0)
		{
			sz = (childs_size[i].absolute ? childs_size[i].size * size : childs_size[i].size);
		}
		
		if (vertical)
		{
			childs[i]->SetSize(parent->GetWidth(), sz);
			childs[i]->SetPos(0, pos);
		}
		else
		{
			childs[i]->SetSize(sz, parent->GetHeight());
			childs[i]->SetPos(pos, 0);
		}

		pos += sz;
	}

	SetSize(parent->GetWidth(), parent->GetHeight());
	SetPos(0, 0);
}

void EUILayout::AddChild(EUIWidget* child)
{
	EUIWidget::AddChild(child);
	childs_size.push_back(ChildSize());
}

void EUILayout::DelChild(EUIWidget* child)
{
	for (unsigned i = 0; i < childs.size(); i++)
	{
		if (childs[i] == child)
		{
			childs_size.erase(childs_size.begin() + i);
			childs.erase(childs.begin() + i);
			break;
		}
	}
}

void EUILayout::SetChildSize(EUIWidget* child, float size, bool absolute)
{
	for (int i = 0; i < childs_size.size(); i++)
	{
		if (childs[i] == child)
		{
			childs_size[i].size = size;
			childs_size[i].absolute = absolute;

			return;
		}
	}
}