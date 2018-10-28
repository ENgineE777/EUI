
#include "EUITabSheet.h"

#ifdef PLATFORM_WIN
#include "native/win/WinTabSheet.h"
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11TabSheet.h"
#endif

EUITabSheet::EUITabSheet(EUITabPanel* owner, const char* txt) : EUIWidget(owner, txt)
{
#ifdef PLATFORM_WIN
	nativeWidget = new WinTabSheet(this);
#endif
#ifdef PLATFORM_WIN_DX11
	nativeWidget = new WinDX11TabSheet(this);
#endif
}

EUITabSheet::~EUITabSheet()
{
}

NativeTabSheet* EUITabSheet::Native()
{
	return nullptr;
}

void EUITabSheet::SetText(const char* text)
{
	EUIWidget::SetText(text);
	Native()->SetText(text);
}

void EUITabSheet::MakeAsCurrent()
{
	Native()->MakeAsCurrent();
}