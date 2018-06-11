
#pragma once

#include "WinWidget.h"
#include "native/NativeTreeView.h"

class EUITreeView;

class WinTreeView : public NativeTreeView
{
	HIMAGELIST imageList;

	struct Node
	{
		HTREEITEM item = 0;
		void* ptr = nullptr;
		std::string text;
		std::string tooltip;
		int image;
		bool  can_have_childs = true;
		Node* parent = nullptr;
		int   child_index = -1;
		std::vector<Node*> childs;

		void ReCreateItem(WinTreeView* tree_view)
		{
			TVITEM tvi;
			tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			tvi.pszText = (LPSTR)text.c_str();
			tvi.cchTextMax = (int)text.size() + 1;
			tvi.lParam = (LPARAM)this;
			tvi.iImage = image;
			tvi.iSelectedImage = image;
			tvi.state = TVIS_EXPANDED;

			TVINSERTSTRUCT tvins;
			tvins.item = tvi;
			tvins.hInsertAfter = TVI_LAST;

			tvins.hParent = (parent != &tree_view->root_node) ? (HTREEITEM)parent->item : TVI_ROOT;
			item = (HTREEITEM)TreeView_InsertItem(tree_view->handle, &tvins);
		}

		void  DeleteNodeChilds()
		{
			for (auto child : childs)
			{
				child->DeleteNodeChilds();
				delete child;
			}

			childs.clear();
		}
	};

	HWND selection = 0;

	HMENU popUpMenu = 0;

	int cur_depth = 0;
	HMENU depth_menu[16];

	std::string sub_menu_name[16];

	static WinTreeView* dragged_source_tree;
	static WinTreeView* dragged_target_tree;
	static Node* dragged_item;
	static Node* dragged_target;
	static bool  drag_on;
	static bool  drag_into_item;

	Node root_node;

	void StartDrag(LPNMTREEVIEW lpnmtv);
	void Drag();
	void EndDrag();

	Node* FindNode(Node* root, void* ptr);
	Node* GetNode(HTREEITEM item);
	void  ReCreateChilds(Node* parent);
	bool  ContainNode(Node* parent, Node* node);

public:

	WinTreeView(EUIWidget* owner);
	virtual ~WinTreeView();

	EUITreeView* Owner();

	void  DrawSelection();
	bool  ProcessWidget(long msg, WPARAM wParam, LPARAM lParam) override;
	void  AddImage(const char* name) override;
	void  DeleteItem(void* item) override;
	void  ClearTree() override;
	void  SortItems(void* root, bool recursive) override;
	void* AddItem(const char* text, int image, void* ptr, void* parent, int child_index, bool can_have_childs, const char* tooltip) override;
	void  SetItemText(void* item, const char* text) override;
	void* GetSelectedItem() override;
	void  SelectItem(void* item) override;
	void* GetItemPtr(void* item) override;
	void  MoveDraggedItem() override;
	void  NotifyMouseOver() override;

	void StartPopupMenu() override;
	void PopupMenuAddItem(int id, const char* name) override;
	void PopupMenuAddSeparator() override;
	void PopupMenuStartSubMenu(const char* name) override;
	void PopupMenuEndSubMenu() override;
};

