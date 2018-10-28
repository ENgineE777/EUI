
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativeCategories.h"

class EUICategories;

class WinDX11Categories : public NativeCategories, public EUIWidget::Listener
{
public:

	float overallHeight;

	class EUIScrollBar* scrollbar = nullptr;

	POINT prev_point;

	WinDX11Categories(EUIWidget* owner);
	~WinDX11Categories() override;

	EUICategories* Owner();

	void Draw() override;
	void CalcThumb();
	void UpdateChildPos() override;
	void Resize() override;

	void OnSrollerPosChange(class EUIScrollBar* sender, int pos);
};
#endif