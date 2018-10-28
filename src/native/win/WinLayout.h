
#pragma once

#include "native/NativeLayout.h"

#ifdef PLATFORM_WIN

class EUIButton;

class WinLayout : public NativeLayout
{
public:

	WinLayout(EUIWidget* owner);
	virtual ~WinLayout();

	EUILayout* Owner();

	virtual void Resize();
};
#endif