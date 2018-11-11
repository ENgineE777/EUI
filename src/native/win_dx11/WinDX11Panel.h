
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "native/NativePanel.h"

class EUIPanel;

class WinDX11Panel : public NativePanel
{
	void* texture = nullptr;

public:

	WinDX11Panel(EUIWidget* owner);
	~WinDX11Panel() override;

	EUIPanel* Owner();

	void Draw() override;

	void SetTexture(void* texture) override;
};
#endif