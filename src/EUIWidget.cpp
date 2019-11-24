
#include "EUIWidget.h"

#include "native/win_dx11/WinDX11Widget.h"

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
	focused = nullptr;
	enabled = true;

	nativeWidget = nullptr;
	listener = nullptr;

	listenerFlag = 0;

	if (parent)
	{
		parent->AddChild(this);
	}
}

EUIWidget::~EUIWidget()
{
	DeleteChilds();

	nativeWidget->Release();

	if (parent)
	{
		parent->DelChild(this);
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

void EUIWidget::SetUserData(void* set_user_data)
{
	user_data = set_user_data;
}

void* EUIWidget::GetUserData()
{
	return user_data;
}

void EUIWidget::SetListener(int set_id, Listener* set_listener, uint32_t flag)
{
	SetID(set_id);
	listener = set_listener;
	listenerFlag = flag;
}

void EUIWidget::OnChildShow(int index, bool set)
{

}

void EUIWidget::Show(bool set)
{
	if (parent)
	{
		parent->OnChildShow(GetIndexAsChild(), set);
	}

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

void EUIWidget::SetPos(int set_x, int set_y)
{
	x = set_x;
	y = set_y;

	nativeWidget->SetPos(set_x, set_y);
}

int EUIWidget::GetX()
{
	return x;
}

int EUIWidget::GetY()
{
	return y;
}

void EUIWidget::SetSize(int w, int h)
{
	width = w;
	height = h;

	nativeWidget->SetSize(width, height);
}

int EUIWidget::GetWidth()
{
	return width;
}

int EUIWidget::GetHeight()
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
	for (int i = 0; i < (int)childs.size(); i++)
	{
		if (childs[i] == child)
		{
			childs.erase(childs.begin() + i);
			break;
		}
	}
}

void EUIWidget::DeleteChilds()
{
	for (int i = 0; i < (int)childs.size(); i++)
	{
		childs[i]->DeleteChilds();
		childs[i]->Release();
		i--;
	}

	childs.clear();
}

int EUIWidget::GetIndexAsChild()
{
	if (!parent)
	{
		return -1;
	}

	for (int i = 0; i < (int)parent->childs.size(); i++)
	{
		if (parent->childs[i] == this)
		{
			return i;
		}
	}

	return -1;
}

void EUIWidget::GetMousePos(int& x, int& y)
{
	nativeWidget->GetMousePos(x, y);
}

void EUIWidget::Update()
{
	if (listener && (listenerFlag & OnUpdate))
	{
		listener->OnUpdate(this);
	}

	for (int i = 0; i < (int)childs.size(); i++)
	{
		if (childs[i]->IsVisible() && childs[i]->IsEnabled())
		{
			childs[i]->Update();
		}
	}
}

void EUIWidget::Redraw()
{
	nativeWidget->Redraw();
}

void EUIWidget::CaptureMouse()
{
	nativeWidget->CaptureMouse();
}

void EUIWidget::ReleaseMouse()
{
	nativeWidget->ReleaseMouse();
}

void* EUIWidget::GetNative()
{
	return nativeWidget->GetNative();
}

EUIWidget* EUIWidget::GetRoot()
{
	if (parent)
	{
		return parent->GetRoot();
	}

	return this;
}

EUIWidget* EUIWidget::GetParent()
{
	return parent;
}

int	EUIWidget::GetChildsCount()
{
	return (int)childs.size();
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

bool EUIWidget::IsHowered()
{
	return nativeWidget->IsHowered();
}