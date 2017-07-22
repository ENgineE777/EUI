
#include "EUIPanel.h"
#include "WinPanel.h"

WinPanel::WinPanel(EUIWidget* owner) : NativePanel(owner)
{
	handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
							Owner()->x, Owner()->y, Owner()->width, Owner()->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), (HMENU)win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();
}

WinPanel::~WinPanel()
{
}

EUIPanel* WinPanel::Owner()
{
	return (EUIPanel*)owner;
}