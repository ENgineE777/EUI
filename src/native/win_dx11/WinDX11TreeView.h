
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeTreeView.h"

class EUITreeView;

class WinDX11TreeView : public NativeTreeView
{
	std::map<int, std::string> imageList;

	struct Node
	{
		void* ptr = nullptr;
		std::string text;
		int image;
		bool  can_have_childs = true;
		bool  opened = false;
		Node* parent = nullptr;
		int   child_index = -1;
		int   x = 0;
		int   y = 0;
		std::vector<Node*> childs;
		bool abc_sort_childs = false;

		void DeleteNodeChilds(WinDX11TreeView* tree_view);
		void AddChild(WinDX11TreeView* tree_view, Node* node, int insert_index);
		void Draw(WinDX11TreeView* tree_view, int pos_x, int& pos_y);
		void CalcHeight(int& height);
		Node* CheckSelection(int& pos_y, int ms_y);
	};

	bool def_abs_sort_childs = false;
	Node* prev_selected = nullptr;
	Node* selected = nullptr;
	Node* target = nullptr;

	static WinDX11TreeView* dragged_source_tree;
	static WinDX11Widget* dragged_target_widget;
	static Node* dragged_item;
	static Node* dragged_target;
	static bool  drag_into_item;

	int start_sel = -1;
	bool ms_at_bottom = false;
	bool ms_at_top = false;
	class EUIScrollBar* scrollbar = nullptr;

	Node root_node;

	void StartDrag();
	void Drag(int ms_x, int ms_y);
	void EndDrag();

	bool ContainNode(Node* parent, Node* node);
	void MoveDraggedItem();
	void InnerSelection(Node* node);

public:

	WinDX11TreeView(EUIWidget* owner, bool abs_sort, bool allow_edit_names);
	~WinDX11TreeView() override;

	EUITreeView* Owner();

	void  AddImage(const char* name) override;
	void  DeleteItem(void* item) override;
	void  ClearTree() override;
	void* AddItem(const char* text, int image, void* ptr, void* parent, int child_index, bool can_have_childs, const char* tooltip) override;
	void  SetABSortChilds(void* item, bool sort) override;
	void  SetItemText(void* item, const char* text) override;
	void  SetItemImage(void* item, int image) override;
	void* GetSelectedItem() override;
	void  SelectItem(void* item) override;
	void  GetItemText(void* item, std::string& text) override;
	void* GetItemPtr(void* item) override;
	void* GetItemParent(void* item) override;
	int   GetItemChildCount(void* item) override;
	void* GetItemChild(void* item, int index) override;
	bool  IsTreeView() override;
	void  Draw() override;

	void CalcThumb();
	void Resize() override;

	void OnFocusLost() override;

	void OnKeyDown(int key) override;
	void OnCharDown(int key) override;

	void OnTimer() override;
	void OnMouseMove(int ms_x, int ms_y) override;
	void OnMouseWheel(int delta) override;
	void OnLeftMouseDown(int ms_x, int ms_y) override;
	void OnLeftMouseUp(int ms_x, int ms_y) override;
	void OnRightMouseUp(int ms_x, int ms_y) override;
};
#endif
