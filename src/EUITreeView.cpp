#include "EUITreeView.h"
#include "native/win/WinTreeView.h"

EUITreeView::EUITreeView(EUIWidget* prnt, int set_x, int set_y, int set_w, int set_h) : EUIWidget(prnt, "")
{
	x = set_x;
	y = set_y;
	width = set_w;
	height = set_h;

	nativeWidget = new WinTreeView(this);
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

void EUITreeView::SortItems(void* root, bool sorted)
{
	Native()->SortItems(root, sorted);
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

void* EUITreeView::GetItemPtr(void* item)
{
	return Native()->GetItemPtr(item);
}

void EUITreeView::MoveDraggedItem()
{
	Native()->MoveDraggedItem();
}

void EUITreeView::StartPopupMenu()
{
	Native()->StartPopupMenu();
}

void EUITreeView::PopupMenuAddItem(int id, const char* name)
{
	Native()->PopupMenuAddItem(id, name);
}

void EUITreeView::PopupMenuAddSeparator()
{
	Native()->PopupMenuAddSeparator();
}

void EUITreeView::PopupMenuStartSubMenu(const char* name)
{
	Native()->PopupMenuStartSubMenu(name);
}

void EUITreeView::PopupMenuEndSubMenu()
{
	Native()->PopupMenuEndSubMenu();
}