
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeTreeView.h"

class EUITreeView;

class WinDX11TreeView : public NativeTreeView
{
	//HIMAGELIST imageList;

	struct Node
	{
		//HTREEITEM item = 0;
		void* ptr = nullptr;
		std::string text;
		std::wstring wtext;
		std::wstring tooltip;
		int image;
		bool  can_have_childs = true;
		Node* parent = nullptr;
		int   child_index = -1;
		std::vector<Node*> childs;
		bool abc_sort_childs = false;

		void ReCreateItem(WinDX11TreeView* tree_view);
		void DeleteNodeChilds(WinDX11TreeView* tree_view);
		void AddChild(WinDX11TreeView* tree_view, Node* node, int insert_index);
	};

	bool def_abs_sort_childs = false;
	HWND selection = 0;

	static WinDX11TreeView* dragged_source_tree;
	static WinDX11Widget* dragged_target_widget;
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
	void  MoveDraggedItem();

public:

	WinDX11TreeView(EUIWidget* owner, bool abs_sort, bool allow_edit_names);
	~WinDX11TreeView() override;

	EUITreeView* Owner();

	void  DrawSelection();
	void  AddImage(const char* name) override;
	void  DeleteItem(void* item) override;
	void  ClearTree() override;
	void* AddItem(const char* text, int image, void* ptr, void* parent, int child_index, bool can_have_childs, const char* tooltip) override;
	void  SetABSortChilds(void* item, bool sort) override;
	void  SetItemText(void* item, const char* text) override;
	void* GetSelectedItem() override;
	void  SelectItem(void* item) override;
	void  GetItemText(void* item, std::string& text) override;
	void* GetItemPtr(void* item) override;
	void* GetItemParent(void* item) override;
	int   GetItemChildCount(void* item) override;
	void* GetItemChild(void* item, int index) override;
	void  NotifyMouseOver() override;
	bool IsTreeView() override;
};
#endif
