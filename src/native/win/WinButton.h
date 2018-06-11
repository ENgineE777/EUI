
#pragma once

#include "WinWidget.h"
#include "native/NativeButton.h"

class EUIButton;

class WinButton : public NativeButton
{
	HBITMAP images[4];
	bool is_howered;

public:

	WinButton(EUIWidget* owner);
	virtual ~WinButton();

	EUIButton* Owner();

	void Enable(bool set) override;

	void SetImage(int img, const char* image_name) override;
	bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam) override;
	void SetText(const char* txt) override;

	void NotifyMouseOver() override;
	void OnMouseLeave() override;

	void Draw() override;
};
