
#pragma once

#include "EUIWidget.h"
#include "Native/NativePanel.h"

class EUIPanel : public EUIWidget
{
	friend class WinPanel;

	NativePanel* Native();

public:

	EUIPanel(int id, EUIWidget* parent, float x, float y, float w, float h);
	virtual ~EUIPanel();
};
