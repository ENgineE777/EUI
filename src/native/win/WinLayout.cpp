
#include "EUILayout.h"
#include "WinLayout.h"

WinLayout::WinLayout(EUIWidget* owner) : NativeLayout(owner)
{
	handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_NOTIFY,
							Owner()->parent->x, Owner()->parent->y, Owner()->parent->width, Owner()->parent->height,
							((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), NULL, NULL, NULL);

	MakeSubClassing();
}

WinLayout::~WinLayout()
{

}

EUILayout* WinLayout::Owner()
{
	return (EUILayout*)owner;
}

void WinLayout::Resize()
{
	Owner()->Resize();
	NativeLayout::Resize();
}


