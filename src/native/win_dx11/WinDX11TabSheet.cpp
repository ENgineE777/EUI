
#include "EUITabSheet.h"
#include "WinDX11TabSheet.h"
#include "WinDX11TabPanel.h"

#ifdef PLATFORM_WIN_DX11

WinDX11TabSheet::WinDX11TabSheet(EUIWidget* owner) : NativeTabSheet(owner)
{
	/*handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | SS_OWNERDRAW | SS_NOTIFY,
	                      5, 25, (int)Owner()->parent->width - 12, (int)Owner()->parent->height - 30,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	parent = (WinTabPanel*)Owner()->parent->nativeWidget;
	parent->AddTab(Owner()->GetText(), handle);*/
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
