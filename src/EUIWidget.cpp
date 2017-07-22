
#include "EUIWidget.h"

EUIWidget::EUIWidget(EUIWidget* prnt, const char* txt)
{
	x = 0;
	y = 0;
	width = 1;
	height = 1;
	visible = true;
	id = 0;
	parent = prnt;
	text = txt;
	focused = NULL;
	enabled = true;

	nativeWidget = NULL;
	listener = NULL;

	listenerFlag = 0;

	if (parent)
	{
		parent->AddChild(this);
	}
}

EUIWidget::~EUIWidget()
{
	nativeWidget->Release();

	if (parent)
	{
		parent->DelChild(this);
	}

	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->Release();
		i--;
	}
}

void EUIWidget::SetID(int set_id)
{
	id = set_id;
}

int EUIWidget::GetID()
{
	return id;
}

void EUIWidget::SetListener(Listener* set_listener, uint32_t flag)
{
	listener = set_listener;
	listenerFlag = flag;
}

void EUIWidget::Show(bool set)
{
	visible = set;
	nativeWidget->Show(set);
}

bool EUIWidget::IsVisible()
{
	if (!visible) return false;

	if (parent)
	{
		if (!parent->IsVisible()) return false;

		return parent->IsVisible();
	}

	return visible;
}

void EUIWidget::Enable(bool set)
{
	enabled = set;
	nativeWidget->Enable(set);
}

bool EUIWidget::IsEnabled()
{
	return enabled;
}

void EUIWidget::SetPos(float set_x, float set_y)
{
	x = set_x;
	y = set_y;

	nativeWidget->SetPos(set_x, set_y);
}

float EUIWidget::GetX()
{
	return x;
}

float EUIWidget::GetY()
{
	return y;
}

void EUIWidget::SetSize(float w, float h)
{
	width = w;
	height = h;

	nativeWidget->SetSize(width, height);
}

float EUIWidget::GetWidth()
{
	return width;
}

float EUIWidget::GetHeight()
{
	return height;
}

void EUIWidget::SetText(const char* txt)
{
	text = txt;
	nativeWidget->SetText(txt);
}

const char* EUIWidget::GetText()
{
	return text.c_str();
}

void EUIWidget::AddChild(EUIWidget* child)
{
	childs.push_back(child);
}

void EUIWidget::DelChild(EUIWidget* child)
{
	for (int i = 0; i < childs.size(); i++)
	{
		if (childs[i] == child)
		{
			childs.erase(childs.begin() + i);
			break;
		}
	}
}

void EUIWidget::Update()
{
	if (listener && (listenerFlag & listenerFlag))
	{
		listener->OnUpdate(this);
	}

	for (int i = 0; i < childs.size(); i++)
	{
		if (childs[i]->IsVisible())
		{
			childs[i]->Update();
		}
	}
}

void* EUIWidget::GetNative()
{
	return nativeWidget->GetNative();
}

EUIWidget* EUIWidget::GetParent()
{
	return parent;
}

int	EUIWidget::GetChildsCount()
{
	return childs.size();
}

EUIWidget* EUIWidget::GetChild(int index)
{
	return childs[index];
}

void EUIWidget::Release()
{
	delete this;
}

void EUIWidget::SetFocused()
{
	nativeWidget->SetFocused();
}

bool EUIWidget::IsFocused()
{
	return nativeWidget->IsFocused();
}

bool EUIWidget::IsHoveredByMouse()
{
	if (!IsVisible()) return false;

	return nativeWidget->IsHoveredByMouse();
}