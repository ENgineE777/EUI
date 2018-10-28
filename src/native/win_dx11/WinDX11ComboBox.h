
#pragma once

#include "native/NativeLabel.h"

#ifdef PLATFORM_WIN_DX11
class EUIComboBox;

class WinDX11ComboBox : public NativeComboBox
{
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
};
#endif