
#pragma once

#include "WinWidget.h"
#include "native/NativePanel.h"

class EUIPanel;

class WinPanel : public NativePanel
{

public:

	WinPanel(EUIWidget* owner);
	virtual ~WinPanel();

	EUIPanel* Owner();
};
