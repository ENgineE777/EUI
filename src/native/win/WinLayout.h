
#pragma once

#include "WinWidget.h"
#include "native/NativeLayout.h"

class EUIButton;

class WinLayout : public NativeLayout
{
public:

	WinLayout(EUIWidget* owner);
	virtual ~WinLayout();

	EUILayout* Owner();

	virtual void Resize();
};
