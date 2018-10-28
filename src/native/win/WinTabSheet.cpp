
#include "EUITabSheet.h"
#include "WinTabSheet.h"
#include "WinTabPanel.h"

#ifdef PLATFORM_WIN

WinTabSheet::WinTabSheet(EUIWidget* owner) : NativeTabSheet(owner)
{
	handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | SS_OWNERDRAW | SS_NOTIFY,
	                      5, 25, (int)Owner()->parent->width - 12, (int)Owner()->parent->height - 30,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();

	parent = (WinTabPanel*)Owner()->parent->nativeWidget;
	parent->AddTab(Owner()->GetText(), handle);
}

WinTabSheet::~WinTabSheet()
{

}

EUITabSheet* WinTabSheet::Owner()
{
	return (EUITabSheet*)owner;
}


void WinTabSheet::SetText(const char* text)
{
	parent->SetTabName(owner->GetIndexAsChild(), text);
}

void WinTabSheet::MakeAsCurrent()
{
	parent->SetCurrentTab(owner->GetIndexAsChild());
}
#endif