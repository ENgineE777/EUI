
#pragma once

#include "native/NativeLabel.h"

#ifdef PLATFORM_WIN_DX11
class EUIComboBox;

class WinDX11ComboBox : public NativeComboBox
{
	bool is_opened = false;
	int cur_string = -1;
	int pre_sel_item = -1;
	std::vector<std::string> items;

public:

	WinDX11ComboBox(EUIWidget* owner);
	~WinDX11ComboBox() override;

	EUIComboBox* Owner();

	void ClearList() override;
	void AddItem(const char* str) override;
	void SetCurString(int index) override;
	void SetCurString(const char* str)  override;
	const char* GetCurString()  override;
	int  GetCurStringIndex()  override;
	void Draw() override;

	void SetText(const char* txt) override;

	bool IsHitted(int ms_x, int ms_y) override;
	void OnFocusLost() override;

	void OnMouseMove(int ms_x, int ms_y) override;
	void OnLeftMouseDown(int ms_x, int ms_y) override;
	void OnLeftMouseUp(int ms_x, int ms_y) override;
};
#endif