
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

	virtual void Enable(bool set);

	virtual void SetImage(int img, const char* image_name);
	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	virtual void SetText(const char* txt);

	virtual void NotifyMouseOver(WinWidget* widget);
	virtual void OnMouseLeave();

	virtual void Draw();
};
