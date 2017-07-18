
#pragma once

#include "EUIWidget.h"
#include "Native/NativeLabel.h"

class EUILabel : public EUIWidget
{
	friend class WinLabel;

	NativeLabel* Native();

public:

	bool color_box;
	int  color[3];

	EUILabel(int id, EUIWidget* parent, const char* txt, bool color_box, float x, float y, float w, float h);
	virtual ~EUILabel();

	virtual void SetText(const char* txt);
};
