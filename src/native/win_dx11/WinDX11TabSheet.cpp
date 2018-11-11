
#include "EUITabSheet.h"
#include "WinDX11TabSheet.h"
#include "WinDX11TabPanel.h"

#ifdef PLATFORM_WIN_DX11

WinDX11TabSheet::WinDX11TabSheet(EUIWidget* owner) : NativeTabSheet(owner)
{
	parent = (WinDX11TabPanel*)Owner()->parent->nativeWidget;
	parent->AddTab(Owner()->GetText());
}

WinDX11TabSheet::~WinDX11TabSheet()
{

}

EUITabSheet* WinDX11TabSheet::Owner()
{
	return (EUITabSheet*)owner;
}


void WinDX11TabSheet::SetText(const char* text)
{
	parent->SetTabName(owner->GetIndexAsChild(), text);
}

void WinDX11TabSheet::MakeAsCurrent()
{
	parent->SetCurrentTab(owner->GetIndexAsChild());
}
#endif
