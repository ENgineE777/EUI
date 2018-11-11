
#include "EUIButton.h"
#include "WinDX11Button.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Button::WinDX11Button(EUIWidget* owner) : NativeButton(owner)
{
}

WinDX11Button::~WinDX11Button()
{
}

EUIButton* WinDX11Button::Owner()
{
	return (EUIButton*)owner;
}

void WinDX11Button::SetImage(int img, const char* image_name)
{
	/*if (images[img])
	{
		DeleteObject(images[img]);
	}

	images[img] = (HBITMAP)LoadImage (0, image_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	InvalidateRect(handle, NULL, false);*/
}

void WinDX11Button::Draw()
{
	const char* elem = "ButtonNormal";

	if (!owner->IsEnabled())
	{
		elem = "ButtonDisabled";
	}
	else
	{
		if (mouse_pressed && is_howered)
		{
			elem = "ButtonPressed";
		}
		else
		if (is_howered || mouse_pressed)
		{
			elem = "ButtonHowered";
		}
		else
		if (Owner()->is_pushed)
		{
			elem = "ButtonPressed";
		}
	}

	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->Draw(elem, global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->font.Print(global_x + owner->x + theme->font.CalcOffset(owner->GetText(), owner->width), global_y + owner->y + 4, nullptr, owner->GetText());
	NativeButton::Draw();
}

void WinDX11Button::OnLeftMouseUp(int ms_x, int ms_y)
{
	if (Owner()->is_pushable && is_howered)
	{
		Owner()->is_pushed = !Owner()->is_pushed;
	}

	NativeButton::OnLeftMouseUp(ms_x, ms_y);
}

#endif
