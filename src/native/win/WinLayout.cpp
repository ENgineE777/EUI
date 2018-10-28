
#include "EUILayout.h"
#include "WinLayout.h"

#ifdef PLATFORM_WIN

WinLayout::WinLayout(EUIWidget* owner) : NativeLayout(owner)
{
	handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_NOTIFY,
	                      (int)Owner()->parent->x, (int)Owner()->parent->y, (int)Owner()->parent->width, (int)Owner()->parent->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

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
#endif

