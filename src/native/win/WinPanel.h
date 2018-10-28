
#pragma once

#include "native/NativePanel.h"

#ifdef PLATFORM_WIN

class EUIPanel;

class WinPanel : public NativePanel
{

public:

	WinPanel(EUIWidget* owner);
	virtual ~WinPanel();

	EUIPanel* Owner();

	void Draw() override;
};
#endif