#include "EUITreeView.h"
#include "WinTreeView.h"
#include "UTFConv.h"

WinTreeView* WinTreeView::dragged_source_tree = nullptr;
WinTreeView* WinTreeView::dragged_target_tree = nullptr;
WinTreeView::Node* WinTreeView::dragged_item = nullptr;
WinTreeView::Node* WinTreeView::dragged_target = nullptr;
bool WinTreeView::drag_on = false;
bool WinTreeView::drag_into_item = false;

void WinTreeView::Node::ReCreateItem(WinTreeView* tree_view)
{
	TVITEMW tvi;
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvi.pszText = (LPWSTR)wtext.c_str();
	tvi.cchTextMax = (int)wtext.size() + 1;
	tvi.lParam = (LPARAM)this;
	tvi.iImage = image;
	tvi.iSelectedImage = image;
	tvi.state = TVIS_EXPANDED;

	TVINSERTSTRUCTW tvins;
	tvins.item = tvi;
	tvins.hInsertAfter = TVI_LAST;

	tvins.hParent = (parent != &tree_view->root_node) ? (HTREEITEM)parent->item : TVI_ROOT;
	item = (HTREEITEM)SNDMSG(tree_view->handle, TVM_INSERTITEMW, 0, (LPARAM)(LPTV_INSERTSTRUCTW)&tvins);
}

void WinTreeView::Node::DeleteNodeChilds(WinTreeView* tree_view)
{
	for (auto child : childs)
	{
		child->DeleteNodeChilds(tree_view);

		if (tree_view->Owner()->listener)
		{
			tree_view->Owner()->listener->OnTreeDeleteItem(tree_view->Owner(), child->item, child->ptr);
		}

		delete child;
	}

	childs.clear();
}

void WinTreeView::Node::AddChild(WinTreeView* tree_view, Node* node, int insert_index)
{
	if (!tree_view->abs_sort && insert_index == -1)
	{
		node->child_index = (int)node->parent->childs.size();
		node->parent->childs.push_back(node);
		node->ReCreateItem(tree_view);

		return;
	}

	for (auto child : childs)
	{
		TreeView_DeleteItem(tree_view->handle, child->item);
	}

	if (tree_view->abs_sort)
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
		tree_view->ReCreateChilds(child);
	}
}

LRESULT CALLBACK SelectionWidgetProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if (uMsg == WM_PAINT)
	{
		WinTreeView* widget = (WinTreeView*)dwRefData;
		widget->DrawSelection();
	}

	return ::DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

WinTreeView::WinTreeView(EUIWidget* owner, bool set_abs_sort, bool allow_edit_names) : NativeTreeView(owner)
{
	int flag = allow_edit_names ? TVS_EDITLABELS : 0;
	handle = CreateWindowExW(0, WC_TREEVIEWW, L"",
	                        WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_SHOWSELALWAYS | WS_VISIBLE | TVS_EX_DOUBLEBUFFER | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_INFOTIP | flag,
	                        (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height, ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);

	win_id++;
	
	MakeSubClassing();

	selection = CreateWindow("STATIC", " ", SS_LEFT | WS_CHILD | SS_OWNERDRAW, 0, 0, 200, 5, handle, win_id, NULL, NULL);
	SetWindowSubclass(selection, &SelectionWidgetProc, 0, (DWORD_PTR)this);

	win_id++;

	SendMessage(handle, WM_SETFONT, (WPARAM)theme->GetFont("FONT_NORMAL"), MAKELPARAM(TRUE, 0));

	imageList = ImageList_Create(21, 21, ILC_COLOR24, 0, 0);
	TreeView_SetImageList(handle, imageList, TVSIL_NORMAL);

	abs_sort = set_abs_sort;
}

WinTreeView::~WinTreeView()
{
}

EUITreeView* WinTreeView::Owner()
{
	return (EUITreeView*)owner;
}

void WinTreeView::StartDrag(LPNMTREEVIEW lpnmtv)
{
	drag_on = true;
	dragged_source_tree = this;
	dragged_target_tree = this;
	dragged_item = GetNode(lpnmtv->itemNew.hItem);
	SetCursor(theme->GetCursor("DRAG_CURSOR"));
}

void WinTreeView::Drag()
{
	if (!drag_on || !IsHoveredByMouse()) return;

	HTREEITEM hitTarget;
	TVHITTESTINFO tvht;

	if (dragged_target_tree != this)
	{
		if (dragged_target_tree)
		{
			TreeView_SelectDropTarget(dragged_target_tree->GetHandle(), nullptr);
		}

		dragged_target_tree = this;
	}

	POINT point;
	if (GetCursorPos(&point))
	{
		ScreenToClient(handle, &point);
	}

	tvht.pt.x = point.x;
	tvht.pt.y = point.y;

	hitTarget = TreeView_HitTest(handle, &tvht);
	TreeView_SelectDropTarget(handle, hitTarget);

	if (hitTarget)
	{
		RECT rc;
		TreeView_GetItemRect(handle, hitTarget, &rc, true);

		SetWindowPos(selection, 0, rc.left + 30, rc.bottom - 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		int center = (int)((rc.bottom + rc.top) * 0.5f);
		drag_into_item = (center < point.y && point.y < rc.bottom && GetNode(hitTarget)->can_have_childs);
	}

	ShowWindow(selection, (hitTarget != nullptr && drag_into_item));
	SetCursor(theme->GetCursor("DRAG_CURSOR"));
}

void WinTreeView::EndDrag()
{
	if (!drag_on)
	{
		return;
	}

	dragged_target = GetNode(TreeView_GetDropHilight(dragged_target_tree->handle));

	if (dragged_source_tree == dragged_target_tree && ContainNode(dragged_item, dragged_target))
	{
		dragged_item = dragged_target;
	}

	if (Owner()->listener && dragged_item != dragged_target)
	{
		int insert_index = 0;
		Node* parent = dragged_target;
		if (!dragged_target->can_have_childs || !drag_into_item)
		{
			insert_index = dragged_target->child_index + 1;
			parent = dragged_target->parent;
		}

		if (Owner()->listener->OnTreeViewItemDragged(dragged_source_tree->Owner(), dragged_target_tree->Owner(), dragged_item->item, dragged_item->child_index, parent->item, insert_index))
		{
			MoveDraggedItem();
		}
	}

	ShowWindow(selection, false);
	TreeView_SelectDropTarget(handle, nullptr);
	SetCursor(theme->GetCursor(""));
	drag_on = false;
}

WinTreeView::Node* WinTreeView::FindNode(Node* root, void* item)
{
	if (!root)
	{
		root = &root_node;
	}

	if (root->item == item)
	{
		return root;
	}

	for (auto child : root->childs)
	{
		WinTreeView::Node* node = FindNode(child, item);

		if (node)
		{
			return node;
		}
	}

	return nullptr;
}

WinTreeView::Node* WinTreeView::GetNode(HTREEITEM item)
{
	if (!item)
	{
		return &root_node;
	}

	TVITEMW tvItem;
	tvItem.hItem = (HTREEITEM)item;
	tvItem.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_PARAM;

	SNDMSG(handle, TVM_GETITEMW, 0, (LPARAM)&tvItem);

	return (WinTreeView::Node*)tvItem.lParam;
}

void WinTreeView::ReCreateChilds(Node* node)
{
	node->ReCreateItem(this);

	if (Owner()->listener)
	{
		Owner()->listener->OnTreeReCreateItem(Owner(), node->item, node->ptr);
	}

	for (auto child : node->childs)
	{
		ReCreateChilds(child);
	}
}

bool WinTreeView::ContainNode(Node* parent, Node* node)
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

void WinTreeView::DrawSelection()
{
	RECT rc = { 0, 0, 200, 5 };
	COLORREF color = theme->GetColor("FONT_EDITBOX_CHANGED");

	theme->DrawGradient(GetDC(selection), rc, color, color, false, 2);
}

bool WinTreeView::ProcessWidget(long msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_MOUSEMOVE)
	{
		Drag();
	}
	else
	if (msg == WM_LBUTTONUP)
	{
		EndDrag();
	}

	if (msg == WM_COMMAND && HIWORD(wParam) == 0)
	{
		if (Owner()->listener)
		{
			Owner()->listener->OnTreeViewPopupItem(Owner(), LOWORD(wParam));
		}
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

			if (popUpMenu)
			{
				DestroyMenu(popUpMenu);
				popUpMenu = 0;
			}

			if (Owner()->listener)
			{
				Owner()->listener->OnTreeViewRightClick(Owner(), ht.hItem, GetNode(ht.hItem)->child_index);
			}

			if (popUpMenu)
			{
				POINT p = ht.pt;
				ClientToScreen(handle, &p);
				SetForegroundWindow(handle);
				TrackPopupMenuEx(popUpMenu, TPM_TOPALIGN | TPM_LEFTALIGN, p.x, p.y, handle, 0);
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

void WinTreeView::AddImage(const char* name)
{
	HBITMAP hbmp = (HBITMAP)LoadImage(0, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	ImageList_Add(imageList, hbmp, nullptr);
	DeleteObject(hbmp);
}

void WinTreeView::DeleteItem(void* item)
{
	Node* node = FindNode(nullptr, item);

	if (node)
	{
		TreeView_DeleteItem(handle, node->item);
		node->parent->childs.erase(node->parent->childs.begin() + node->child_index);
		node->DeleteNodeChilds(this);

		if (Owner()->listener)
		{
			Owner()->listener->OnTreeDeleteItem(Owner(), node->item, node->ptr);
		}

		delete node;
	}
}

void WinTreeView::ClearTree()
{
	TreeView_DeleteAllItems(handle);
	root_node.DeleteNodeChilds(this);
}

void* WinTreeView::AddItem(const char* text, int image, void* ptr, void* parent, int child_index, bool can_have_childs, const char* tooltip)
{
	Node* node = new Node();
	node->ptr = ptr;

	node->text = text;
	UTFConv::UTF8to16(node->wtext, text);

	if (tooltip)
	{
		UTFConv::UTF8to16(node->tooltip, tooltip);
	}

	node->can_have_childs = can_have_childs;
	node->parent = FindNode(nullptr, parent);
	node->image = image;

	node->parent->AddChild(this, node, child_index);

	Redraw();

	return node->item;
}

void WinTreeView::SetItemText(void* item, const char* text)
{
	if (!item)
	{
		return;
	}

	Node* node = GetNode((HTREEITEM)item);

	if (node)
	{
		node->text = text;
		UTFConv::UTF8to16(node->wtext, text);

		if (abs_sort)
		{
			TreeView_DeleteItem(handle, node->item);
			node->parent->childs.erase(node->parent->childs.begin() + node->child_index);

			node->parent->AddChild(this, node, -1);
		}
		else
		{
			TVITEMW tvitem;

			tvitem.mask = TVIF_TEXT;
			tvitem.hItem = (HTREEITEM)item;

			tvitem.pszText = (LPWSTR)node->wtext.c_str();
			tvitem.cchTextMax = (int)node->wtext.size() + 1;

			SNDMSG(handle, TVM_SETITEMW, 0, (LPARAM)&tvitem);
		}
	}
}

void* WinTreeView::GetSelectedItem()
{
	return TreeView_GetSelection(handle);
}

void WinTreeView::SelectItem(void* item)
{
	TreeView_SelectItem(handle, item);
}

void WinTreeView::GetItemText(void* item, std::string& text)
{
	Node* node = FindNode(nullptr, item);

	if (node)
	{
		text = node->text;
	}
}

void* WinTreeView::GetItemPtr(void* item)
{
	Node* node = FindNode(nullptr, item);

	return node ? node->ptr : nullptr;
}

void* WinTreeView::GetItemParent(void* item)
{
	if (!item)
	{
		return nullptr;
	}

	Node* node = FindNode(nullptr, item);

	return node->parent ? node->parent->item : nullptr;
}

int WinTreeView::GetItemChildCount(void* item)
{
	Node* node = FindNode(nullptr, item);

	return node ? (int)node->childs.size() : 0;
}

void* WinTreeView::GetItemChild(void* item, int index)
{
	Node* node = FindNode(nullptr, item);

	return node ? node->childs[index]->item : nullptr;
}

void WinTreeView::MoveDraggedItem()
{
	if ((dragged_source_tree != dragged_target_tree) || dragged_item == dragged_target)
	{
		return;
	}

	TreeView_DeleteItem(handle, dragged_item->item);

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

	dragged_target->AddChild(this, dragged_item, insert_index);

	Redraw();
}

void WinTreeView::NotifyMouseOver()
{
	NativeTreeView::NotifyMouseOver();

	Drag();
}

void WinTreeView::StartPopupMenu()
{
	popUpMenu = CreatePopupMenu();
	
	cur_depth = 0;
	depth_menu[cur_depth] = popUpMenu;
}

void WinTreeView::PopupMenuAddItem(int id, const char* name)
{
	AppendMenu(depth_menu[cur_depth], MF_STRING, id, name);
}

void WinTreeView::PopupMenuAddSeparator()
{
	AppendMenu(depth_menu[cur_depth], MF_SEPARATOR, 7, 0);
}

void WinTreeView::PopupMenuStartSubMenu(const char* name)
{
	cur_depth++;
	depth_menu[cur_depth] = CreateMenu();
	sub_menu_name[cur_depth] = name;
}

void WinTreeView::PopupMenuEndSubMenu()
{
	if (cur_depth == 0) return;

	AppendMenu(depth_menu[cur_depth - 1], MF_POPUP, (UINT_PTR)depth_menu[cur_depth], sub_menu_name[cur_depth].c_str());
	cur_depth--;
}