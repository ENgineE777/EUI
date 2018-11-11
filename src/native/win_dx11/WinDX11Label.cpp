
#include "EUILabel.h"
#include "WinDX11Label.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Label::WinDX11Label(EUIWidget* owner) : NativeLabel(owner)
{
}

WinDX11Label::~WinDX11Label()
{
}

EUILabel* WinDX11Label::Owner()
{
	return (EUILabel*)owner;
}

void WinDX11Label::Draw()
{
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->font.Print(global_x + owner->x + 2, global_y + owner->y + 3, nullptr, owner->GetText());

	if (Owner()->bck_use)
	{
		float color[4];
		color[0] = Owner()->bck_color[0] / 255.0f;
		color[1] = Owner()->bck_color[1] / 255.0f;
		color[2] = Owner()->bck_color[2] / 255.0f;
		color[3] = 1.0f;

		theme->Draw(nullptr, color, global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	}

	NativeLabel::Draw();
}

#endif