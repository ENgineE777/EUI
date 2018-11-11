
#include "EUILayout.h"

#ifdef PLATFORM_WIN
#include "native/win/WinLayout.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11Layout.h"
#endif

EUILayout::EUILayout(EUIWidget* prnt, bool set_vertical) : EUIWidget(prnt, "")
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;

	vertical = set_vertical;

#ifdef PLATFORM_WIN
	nativeWidget = new WinLayout(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11Layout(this);
#endif
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
			sz += (int)(childs_size[i].absolute ? childs_size[i].size * size : childs_size[i].size);
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

	int pos = 0;

	for (int i = 0; i < (int)childs.size(); i++)
	{
		int sz = (int)elem_size;

		if (childs_size[i].size > 0)
		{
			sz = (int)(childs_size[i].absolute ? childs_size[i].size * size : childs_size[i].size);
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