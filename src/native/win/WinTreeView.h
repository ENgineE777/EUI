
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
		std::wstring wtext;
		std::wstring tooltip;
		int image;
		bool  can_have_childs = true;
		Node* parent = nullptr;
		int   child_index = -1;
		std::vector<Node*> childs;

		void ReCreateItem(WinTreeView* tree_view);
		void DeleteNodeChilds(WinTreeView* tree_view);
		void AddChild(WinTreeView* tree_view, Node* node, int insert_index);
	};

	bool abs_sort = false;
	HWND selection = 0;

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
	void  MoveDraggedItem();

public:

	WinTreeView(EUIWidget* owner, bool abs_sort, bool allow_edit_names);
	virtual ~WinTreeView();

	EUITreeView* Owner();

	void  DrawSelection();
	bool  ProcessWidget(long msg, WPARAM wParam, LPARAM lParam) override;
	void  AddImage(const char* name) override;
	void  DeleteItem(void* item) override;
	void  ClearTree() override;
	void* AddItem(const char* text, int image, void* ptr, void* parent, int child_index, bool can_have_childs, const char* tooltip) override;
	void  SetItemText(void* item, const char* text) override;
	void* GetSelectedItem() override;
	void  SelectItem(void* item) override;
	void  GetItemText(void* item, std::string& text) override;
	void* GetItemPtr(void* item) override;
	void* GetItemParent(void* item) override;
	int   GetItemChildCount(void* item) override;
	void* GetItemChild(void* item, int index) override;
	void  NotifyMouseOver() override;
};

