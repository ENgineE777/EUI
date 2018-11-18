#include "EUITreeView.h"
#include "WinDX11TreeView.h"
#include "UTFConv.h"
#include "EUIScrollBar.h"
#include "WinDX11ScrollBar.h"

#ifdef PLATFORM_WIN_DX11

WinDX11TreeView* WinDX11TreeView::dragged_source_tree = nullptr;
WinDX11Widget* WinDX11TreeView::dragged_target_widget = nullptr;
WinDX11TreeView::Node* WinDX11TreeView::dragged_item = nullptr;
WinDX11TreeView::Node* WinDX11TreeView::dragged_target = nullptr;
bool WinDX11TreeView::drag_into_item = false;

void WinDX11TreeView::Node::DeleteNodeChilds(WinDX11TreeView* tree_view)
{
	for (auto child : childs)
	{
		child->DeleteNodeChilds(tree_view);

		if (tree_view->Owner()->listener)
		{
			tree_view->Owner()->listener->OnTreeDeleteItem(tree_view->Owner(), child, child->ptr);
		}

		delete child;
	}

	childs.clear();
}

void WinDX11TreeView::Node::AddChild(WinDX11TreeView* tree_view, Node* node, int insert_index)
{
	if (!node->abc_sort_childs && insert_index == -1)
	{
		node->child_index = (int)childs.size();
		childs.push_back(node);
		return;
	}

	if (node->abc_sort_childs)
	{
		childs.push_back(node);

		for (int i = (int)childs.size() - 1; i > 0; i--)
		{
			if (!childs[i]->can_have_childs && childs[i - 1]->can_have_childs)
			{
				break;
			}

			if ((childs[i]->can_have_childs && !childs[i - 1]->can_have_childs) ||
				UTFConv::CompareABC(childs[i]->text.c_str(), childs[i - 1]->text.c_str()))
			{
				Node* tmp = childs[i - 1];
				childs[i - 1] = childs[i];
				childs[i] = tmp;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		childs.insert(childs.begin() + insert_index, node);
	}

	int index = 0;
	for (auto child : childs)
	{
		child->child_index = index;
		index++;
	}
}

void WinDX11TreeView::Node::Draw(WinDX11TreeView* tree_view, int pos_x, int& pos_y)
{
	if (tree_view->selected == this)
	{
		float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		theme->Draw(nullptr, color, tree_view->global_x + tree_view->owner->x + 3, pos_y, tree_view->owner->width - 6, 15);
	}
	else
	if (tree_view->target == this)
	{
		float color[] = { 0.75f, 0.75f, 0.75f, 1.0f };

		if (tree_view->drag_into_item)
		{
			theme->Draw(nullptr, color, tree_view->global_x + tree_view->owner->x + 3 + pos_x + 15, pos_y + 13, tree_view->owner->width - 6, 4);
		}
		else
		{
			theme->Draw(nullptr, color, tree_view->global_x + tree_view->owner->x + 3, pos_y, tree_view->owner->width - 6, 15);
		}
	}

	if (childs.size() > 0)
	{
		theme->Draw(opened ? "TreeViewMinus" : "TreeViewPlus", pos_x, pos_y, 15, 15);
	}

	x = pos_x;
	y = pos_y;

	theme->Draw(tree_view->imageList[image].c_str(), pos_x + ((childs.size() > 0) ? 15 : 0), pos_y, 20, 15);
	theme->font.Print(pos_x + ((childs.size() > 0)? 15 : 0) + 20, pos_y + 2, nullptr, text.c_str());

	pos_y += 15;

	if (opened)
	{
		for (auto& child : childs)
		{
			child->Draw(tree_view, pos_x + 15, pos_y);
		}
	}
}

void WinDX11TreeView::Node::CalcHeight(int& height)
{
	height += 15;

	if (opened)
	{
		for (auto& child : childs)
		{
			child->CalcHeight(height);
		}
	}
}

WinDX11TreeView::Node* WinDX11TreeView::Node::CheckSelection(int& pos_y, int ms_y)
{
	if (pos_y <= ms_y && ms_y <= pos_y + 15)
	{
		return this;
	}

	pos_y += 15;

	if (opened)
	{
		for (auto& child : childs)
		{
			Node* res = child->CheckSelection(pos_y, ms_y);

			if (res)
			{
				return res;
			}
		}
	}

	return nullptr;
}

WinDX11TreeView::WinDX11TreeView(EUIWidget* owner, bool set_abs_sort, bool allow_edit_names) : NativeTreeView(owner)
{
	def_abs_sort_childs = set_abs_sort;

	SetTimer(150);

	Owner()->nativeWidget = this;
	scrollbar = new EUIScrollBar(Owner(), false, Owner()->width - 15, 0, 15, Owner()->height);
	scrollbar->Show(false);
}

WinDX11TreeView::~WinDX11TreeView()
{
}

EUITreeView* WinDX11TreeView::Owner()
{
	return (EUITreeView*)owner;
}

void WinDX11TreeView::StartDrag()
{
	drag_on = true;
	dragged_source_tree = this;
	dragged_target_widget = this;
	dragged_item = selected;
	SetCursor(theme->GetCursor("DRAG_CURSOR"));

	CaptureMouse();
}

void WinDX11TreeView::Drag(int ms_x, int ms_y)
{
	dragged_target_widget = ((WinDX11Widget*)(owner->GetRoot()->nativeWidget))->mouse_over;

	if (dragged_target_widget != this)
	{
		target = nullptr;
		if (dragged_target_widget->IsTreeView())
		{
			((WinDX11TreeView*)dragged_target_widget)->Drag(ms_x, ms_y);
		}
		return;
	}

	target = nullptr;
	int selection_y = 3 - (scrollbar->IsVisible() ? scrollbar->GetPosition() : 0);

	ms_at_top = (global_y + owner->y < ms_y && ms_y < global_y + owner->y + 20);
	ms_at_bottom = (global_y + owner->y + owner->height - 20 < ms_y && ms_y < global_y + owner->y + owner->height);

	for (auto& child : root_node.childs)
	{
		target = child->CheckSelection(selection_y, ms_y - global_y - owner->y);

		if (target)
		{
			break;
		}
	}

	if (target)
	{
		drag_into_item = (target->y + 7 < ms_y && target->can_have_childs);
	}
	else
	{
		target = &root_node;
		drag_into_item = false;
	}
}

void WinDX11TreeView::EndDrag()
{
	if (!drag_on)
	{
		return;
	}

	int insert_index = -1;
	Node* parent = nullptr;

	if (dragged_target_widget->IsTreeView())
	{
		WinDX11TreeView* win_target = (WinDX11TreeView*)dragged_target_widget;

		dragged_target = win_target->target;

		if (dragged_target)
		{
			if (dragged_source_tree == dragged_target_widget && ContainNode(dragged_item, dragged_target))
			{
				dragged_item = dragged_target;
			}

			if (dragged_item != dragged_target)
			{
				insert_index = 0;
				parent = dragged_target;
			
				if (!dragged_target->can_have_childs || !drag_into_item)
				{
					insert_index = dragged_target->child_index + 1;
					parent = dragged_target->parent;
				}
			}
		}
		else
		{
			dragged_target_widget = nullptr;
		}
	}
	else
	{
		dragged_target = nullptr;
	}

	if (Owner()->listener && dragged_target_widget)
	{
		if (Owner()->listener->OnTreeViewItemDragged(dragged_source_tree->Owner(), dragged_target_widget->owner, dragged_item, dragged_item->child_index, parent ? parent : nullptr, insert_index))
		{
			MoveDraggedItem();
		}
	}

	drag_on = false;
	target = nullptr;

	if (dragged_target_widget && dragged_target_widget != this && dragged_target_widget->IsTreeView())
	{
		WinDX11TreeView* target = (WinDX11TreeView*)dragged_target_widget;
		target->target = nullptr;
	}

	SetCursor(theme->GetCursor(""));
	ReleaseMouse();
}

bool WinDX11TreeView::ContainNode(Node* parent, Node* node)
{
	for (auto child : parent->childs)
	{
		if (child == node || ContainNode(child, node))
		{
			return true;
		}
	}

	return false;
}

/*
bool WinTreeView::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	NativeTreeView::ProcessWidget(msg, wParam, lParam);

	if (msg == WM_MOUSEMOVE)
	{
		Drag();
	}
	else
	if (msg == WM_LBUTTONUP)
	{
		EndDrag();
	}

	if (msg == WM_NOTIFY)
	{
		if (((LPNMHDR)lParam)->code == TVN_GETINFOTIPA || ((LPNMHDR)lParam)->code == TVN_GETINFOTIPW)
		{
			LPNMTVGETINFOTIPW tip = (LPNMTVGETINFOTIPW)lParam;
			Node* node = FindNode(nullptr, tip->hItem);
			if (node)
			{
				wcscpy(tip->pszText, node->tooltip.c_str());
			}
		}
		else
		if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
		{
			if (Owner()->listener)
			{
				Owner()->listener->OnTreeViewSelChange(Owner(), GetSelectedItem());
			}
		}
		else
		if (((LPNMHDR)lParam)->code == NM_RCLICK)
		{
			LPNMHDR lpnmh = (LPNMHDR)lParam;
			TVHITTESTINFO ht;
			DWORD dwpos = GetMessagePos();
			ht.pt.x = GET_X_LPARAM(dwpos);
			ht.pt.y = GET_Y_LPARAM(dwpos);
			MapWindowPoints(HWND_DESKTOP, lpnmh->hwndFrom, &ht.pt, 1);
			TreeView_HitTest(lpnmh->hwndFrom, &ht);

			if (Owner()->listener)
			{
				Owner()->listener->OnTreeViewRightClick(Owner(), ht.pt.x, ht.pt.y, ht.hItem, GetNode(ht.hItem)->child_index);
			}
		}
		else
		if (((LPNMHDR)lParam)->code == TVN_BEGINDRAGW || ((LPNMHDR)lParam)->code == TVN_BEGINDRAGA)
		{
			StartDrag((LPNMTREEVIEW)lParam);
		}
		else
		if (((LPNMHDR)lParam)->code == TVN_ENDLABELEDIT)// || ((LPNMHDR)lParam)->code == TVN_ENDLABELEDITA)
		{
			LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)lParam;
			Node* node = GetNode(ptvdi->item.hItem);

			if (ptvdi->item.pszText && node)
			{
				node->text = ptvdi->item.pszText;
				//UTFConv::UTF16to8(node->text, ptvdi->item.pszText);

				if (Owner()->listener)
				{
					Owner()->listener->OnTreeViewSelItemTextChanged(Owner(), ptvdi->item.hItem, node->text.c_str());
				}
			}
		}
	}

	return true;
}
*/

void WinDX11TreeView::AddImage(const char* name)
{
	imageList[(int)imageList.size()] = name;
	theme->LoadImage(name);
}

void WinDX11TreeView::DeleteItem(void* item)
{
	Node* node = (Node*)item;

	if (node)
	{
		node->parent->childs.erase(node->parent->childs.begin() + node->child_index);

		int index = 0;
		for (auto child : node->parent->childs)
		{
			child->child_index = index;
			index++;
		}

		node->DeleteNodeChilds(this);

		if (Owner()->listener)
		{
			Owner()->listener->OnTreeDeleteItem(Owner(), item, node->ptr);
		}

		delete node;

		InnerSelection(nullptr);
	}

	CalcThumb();
}

void WinDX11TreeView::ClearTree()
{
	root_node.DeleteNodeChilds(this);
}

void* WinDX11TreeView::AddItem(const char* text, int image, void* ptr, void* parent, int child_index, bool can_have_childs, const char* tooltip)
{
	Node* node = new Node();
	node->abc_sort_childs = def_abs_sort_childs;
	node->ptr = ptr;

	node->text = text;

	node->can_have_childs = can_have_childs;
	node->parent = parent? (Node*)parent : &root_node;
	node->image = image;

	node->parent->AddChild(this, node, child_index);

	CalcThumb();

	return node;
}

void WinDX11TreeView::SetABSortChilds(void* item, bool sort)
{
	Node* node = (Node*)item;

	if (node)
	{
		node->abc_sort_childs = sort;
	}
}

void WinDX11TreeView::SetItemText(void* item, const char* text)
{
	if (!item)
	{
		return;
	}

	Node* node = (Node*)item;

	if (node)
	{
		node->text = text;
		
		if (node->parent->abc_sort_childs)
		{
			node->parent->childs.erase(node->parent->childs.begin() + node->child_index);

			node->parent->AddChild(this, node, -1);

			SelectItem(item);
		}
	}
}

void* WinDX11TreeView::GetSelectedItem()
{
	return selected;
}

void WinDX11TreeView::SelectItem(void* item)
{
	InnerSelection((Node*)item);
}

void WinDX11TreeView::GetItemText(void* item, std::string& text)
{
	Node* node = (Node*)item;

	if (node)
	{
		text = node->text;
	}
}

void* WinDX11TreeView::GetItemPtr(void* item)
{
	Node* node = (Node*)item;

	return node ? node->ptr : nullptr;
}

void* WinDX11TreeView::GetItemParent(void* item)
{
	if (!item)
	{
		return nullptr;
	}

	Node* node = (Node*)item;

	return node->parent ? node->parent : nullptr;
}

int WinDX11TreeView::GetItemChildCount(void* item)
{
	Node* node = (Node*)item;

	return node ? (int)node->childs.size() : (int)root_node.childs.size();
}

void* WinDX11TreeView::GetItemChild(void* item, int index)
{
	Node* node = (Node*)item;

	return node ? node->childs[index] : root_node.childs[index];
}

void WinDX11TreeView::MoveDraggedItem()
{
	if ((dragged_source_tree != dragged_target_widget) || dragged_item == dragged_target)
	{
		return;
	}

	dragged_item->parent->childs.erase(dragged_item->parent->childs.begin() + dragged_item->child_index);

	int index = 0;
	for (auto child : dragged_item->parent->childs)
	{
		child->child_index = index;
		index++;
	}

	int insert_index = 0;

	if (!dragged_target->can_have_childs || !drag_into_item)
	{
		insert_index = dragged_target->child_index + 1;
		dragged_target = dragged_target->parent;
	}

	dragged_item->parent = dragged_target;

	if (dragged_target)
	{
		dragged_target->AddChild(this, dragged_item, insert_index);
		dragged_target->opened = true;
	}
	else
	{
		root_node.AddChild(this, dragged_item, insert_index);
	}

	CalcThumb();
}

void WinDX11TreeView::InnerSelection(Node* node)
{
	if (selected != node)
	{
		selected = node;

		if (Owner()->listener)
		{
			Owner()->listener->OnTreeViewSelChange(Owner(), selected);
		}
	}
}

bool WinDX11TreeView::IsTreeView()
{
	return true;
}

void WinDX11TreeView::CalcThumb()
{
	int height = 0;

	for (auto& child : root_node.childs)
	{
		child->CalcHeight(height);
	}

	scrollbar->SetPos(Owner()->width - 15, 0);
	scrollbar->SetSize(15, Owner()->height);

	int delta = height - Owner()->height + 6;
	scrollbar->Show(delta > 0);

	if (delta > 0)
	{
		scrollbar->SetLimit(1, delta);
	}
}

void WinDX11TreeView::Resize()
{
	CalcThumb();
	NativeTreeView::Resize();
}

void WinDX11TreeView::Draw()
{
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->Draw("TreeView", global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->SetClampBorder(global_x + owner->x + 3, global_y + owner->y + 3, owner->width - 6, owner->height - 6);

	int pos_x = global_x + owner->x + 3;
	int pos_y = global_y + owner->y + 3 - (scrollbar->IsVisible() ? scrollbar->GetPosition() : 0);

	for (auto& child : root_node.childs)
	{
		child->Draw(this, pos_x, pos_y);
	}

	NativeTreeView::Draw();
}

void WinDX11TreeView::OnTimer()
{
	if (drag_on)
	{
		if (scrollbar->IsVisible())
		{
			if (dragged_target_widget->IsTreeView())
			{
				WinDX11TreeView* win_target = (WinDX11TreeView*)dragged_target_widget;

				if (win_target->ms_at_top)
				{
					((WinDX11ScrollBar*)win_target->scrollbar->nativeWidget)->ChangeValue(-20);
				}

				if (win_target->ms_at_bottom)
				{
					((WinDX11ScrollBar*)win_target->scrollbar->nativeWidget)->ChangeValue(20);
				}
			}
		}
	}
}

void WinDX11TreeView::OnMouseMove(int ms_x, int ms_y)
{
	if (mouse_pressed && selected && !drag_on)
	{
		StartDrag();
	}
	else
	if (drag_on)
	{
		Drag(ms_x + global_x + owner->x, ms_y + global_y + owner->y);
	}

	NativeTreeView::OnMouseMove(ms_x, ms_y);
}

void WinDX11TreeView::OnMouseWheel(int delta)
{
	if (scrollbar->IsVisible())
	{
		((WinDX11ScrollBar*)scrollbar->nativeWidget)->ChangeValue(delta);
	}
}

void WinDX11TreeView::OnLeftMouseDown(int ms_x, int ms_y)
{
	prev_selected = selected;
	selected = nullptr;
	int selection_y = 3 - (scrollbar->IsVisible() ? scrollbar->GetPosition() : 0);

	for (auto& child : root_node.childs)
	{
		selected = child->CheckSelection(selection_y, ms_y);

		if (selected)
		{
			break;
		}
	}

	NativeTreeView::OnLeftMouseDown(ms_x, ms_y);
}

void WinDX11TreeView::OnLeftMouseUp(int ms_x, int ms_y)
{
	if (mouse_pressed && selected && drag_on)
	{
		EndDrag();
	}

	if (prev_selected != selected)
	{
		if (Owner()->listener)
		{
			Owner()->listener->OnTreeViewSelChange(Owner(), selected);
		}
	}

	int ms_pos = global_x + owner->x + ms_x;
	if (selected && (selected->childs.size() > 0) && selected->x < ms_pos && ms_pos  < selected->x + 15)
	{
		selected->opened = !selected->opened;
		CalcThumb();
	}

	NativeTreeView::OnLeftMouseUp(ms_x, ms_y);
}

void WinDX11TreeView::OnRightMouseUp(int ms_x, int ms_y)
{
	Node* selected = nullptr;
	int selection_y = 3 - (scrollbar->IsVisible() ? scrollbar->GetPosition() : 0);

	for (auto& child : root_node.childs)
	{
		selected = child->CheckSelection(selection_y, ms_y);

		if (selected)
		{
			break;
		}
	}

	if (Owner()->listener)
	{
		Owner()->listener->OnTreeViewRightClick(Owner(), ms_x, ms_y, selected, selected ? selected->child_index : -1);
	}

	NativeTreeView::OnRightMouseUp(ms_x, ms_y);
}

#endif