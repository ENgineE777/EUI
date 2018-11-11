
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeCategories.h"

class EUICategories;

class WinDX11Categories : public NativeCategories, public EUIWidget::Listener
{
public:

	float overallHeight;

	class EUIScrollBar* scrollbar = nullptr;

	int howered_category = -1;
	POINT prev_point;

	WinDX11Categories(EUIWidget* owner);
	~WinDX11Categories() override;

	EUICategories* Owner();

	void Draw() override;
	void CalcThumb() override;
	void UpdateChildPos() override;
	void Resize() override;

	void OnSrollerPosChange(class EUIScrollBar* sender, int pos);

	void OnMouseMove(int ms_x, int ms_y) override;
	void OnMouseLeave() override;
	void OnLeftMouseDown(int ms_x, int ms_y) override;
	void OnLeftMouseUp(int ms_x, int ms_y) override;
};
#endif