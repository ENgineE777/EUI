
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
		size = (int)parent->GetHeight();
	}
	else
	{
		size = (int)parent->GetWidth();
	}

	float sz = 0;
	int cnt = 0;

	for (int i = 0; i < (int)childs.size(); i++)
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

	float elem_size = (float)(size - sz) / (float)cnt;

	if (size < 0)
	{
		size = 0;
	}

	float pos = 0;

	for (int i = 0; i < (int)childs.size(); i++)
	{
		float sz = elem_size;

		if (childs_size[i].size > 0)
		{
			sz = (childs_size[i].absolute ? childs_size[i].size * size : childs_size[i].size);
		}
		
		if (vertical)
		{
			childs[i]->SetSize(parent->GetWidth(), (int)sz);
			childs[i]->SetPos(0, (int)pos);
		}
		else
		{
			childs[i]->SetSize((int)sz, parent->GetHeight());
			childs[i]->SetPos((int)pos, 0);
		}

		childs[i]->Redraw();

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
	for (int i = 0; i < (int)childs_size.size(); i++)
	{
		if (childs[i] == child)
		{
			childs_size[i].size = size;
			childs_size[i].absolute = absolute;

			return;
		}
	}
}