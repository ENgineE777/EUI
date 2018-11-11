
#include "EUIPanel.h"
#include "WinDX11Panel.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Panel::WinDX11Panel(EUIWidget* owner) : NativePanel(owner)
{
	/*handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
	                      (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	MakeSubClassing();*/
}

WinDX11Panel::~WinDX11Panel()
{
}

EUIPanel* WinDX11Panel::Owner()
{
	return (EUIPanel*)owner;
}

void WinDX11Panel::Draw()
{
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);

	if (texture)
	{
		theme->Draw(texture, nullptr, global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	}
	else
	{
		theme->Draw("Panel", global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	}

	NativePanel::Draw();
}

void WinDX11Panel::SetTexture(void* set_texture)
{
	texture = set_texture;
}
#endif