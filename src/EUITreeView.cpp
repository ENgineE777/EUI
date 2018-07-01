#include "EUITreeView.h"
#include "native/win/WinTreeView.h"

EUITreeView::EUITreeView(EUIWidget* prnt, int set_x, int set_y, int set_w, int set_h, bool abs_sort, bool allow_edit_names) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

	nativeWidget = new WinTreeView(this, abs_sort, allow_edit_names);
}

EUITreeView::~EUITreeView()
{
}

NativeTreeView* EUITreeView::Native()
{
	return (NativeTreeView*)nativeWidget;
}

void EUITreeView::AddImage(const char* name)
{
	Native()->AddImage(name);
}

void EUITreeView::DeleteItem(void* item)
{
	Native()->DeleteItem(item);
}

void EUITreeView::ClearTree()
{
	Native()->ClearTree();
}

void* EUITreeView::AddItem(const char* txt, int image, void* ptr, void* parent, int child_index, bool can_have_childs, const char* tooltip)
{
	return Native()->AddItem(txt, image, ptr, parent, child_index, can_have_childs, tooltip);
}

void EUITreeView::SetItemText(void* item, const char* text)
{
	return Native()->SetItemText(item, text);
}

void* EUITreeView::GetSelectedItem()
{
	return Native()->GetSelectedItem();
}

void EUITreeView::SelectItem(void* item)
{
	Native()->SelectItem(item);
}

void EUITreeView::GetItemText(void* item, std::string& text)
{
	Native()->GetItemText(item, text);
}

void* EUITreeView::GetItemPtr(void* item)
{
	return Native()->GetItemPtr(item);
}

void* EUITreeView::GetItemParent(void* item)
{
	return Native()->GetItemParent(item);
}

int EUITreeView::GetItemChildCount(void* item)
{
	return Native()->GetItemChildCount(item);
}

void* EUITreeView::GetItemChild(void* item, int index)
{
	return Native()->GetItemChild(item, index);
}