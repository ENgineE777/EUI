
#pragma once

#include "native/NativeCategories.h"

#ifdef PLATFORM_WIN

class EUICategories;

class WinCategories : public NativeCategories, public EUIWidget::Listener
{
public:

	float overallHeight;

	class EUIScrollBar* scrollbar = nullptr;

	POINT prev_point;

	WinCategories(EUIWidget* owner);
	virtual ~WinCategories();

	EUICategories* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);

	virtual void Draw();
	virtual void CalcThumb();
	virtual void UpdateChildPos();
	virtual void Resize();

	void OnSrollerPosChange(class EUIScrollBar* sender, int pos);
};
#endif