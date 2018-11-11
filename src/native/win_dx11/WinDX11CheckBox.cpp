#include "EUICheckBox.h"
#include "WinDX11CheckBox.h"

#ifdef PLATFORM_WIN_DX11

WinDX11CheckBox::WinDX11CheckBox(EUIWidget* owner) : NativeCheckBox(owner)
{
}

WinDX11CheckBox::~WinDX11CheckBox()
{
}

EUICheckBox* WinDX11CheckBox::Owner()
{
	return (EUICheckBox*)owner;
}

void WinDX11CheckBox::SetText(const char* txt)
{
	Redraw();
}

void WinDX11CheckBox::SetChecked(bool set)
{
}

void WinDX11CheckBox::Draw()
{
	const char* elem = "CheckBoxMarkNormal";

	if (!owner->IsEnabled())
	{
		elem = "CheckBoxMarkDisabled";
	}
	else
	if (is_howered)
	{
		if (mouse_pressed && is_howered)
		{
			elem = "CheckBoxMarkPressed";
		}
		else
		if (is_howered || mouse_pressed)
		{
			elem = "CheckBoxMarkHowered";
		}
	}

	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->Draw(elem, global_x + owner->x, global_y + owner->y, 19, 19);
	theme->Draw((Owner()->checked) ? "CheckBoxMarkOn" : "CheckBoxMarkOff", global_x + owner->x, global_y + owner->y, 19, 19);
	theme->SetClampBorder(global_x + owner->x, global_y + owner->y, owner->width, owner->height);
	theme->font.Print(global_x + owner->x + 22, global_y + owner->y, nullptr, owner->GetText());

	NativeCheckBox::Draw();
}

void WinDX11CheckBox::OnMouseMove(int ms_x, int ms_y)
{
	if (0 < ms_x && ms_x < 19 &&
		0 < ms_x && ms_x < 19)
	{
		is_howered = true;
	}
	else
	{
		is_howered = false;
	}

	NativeCheckBox::OnMouseMove(ms_x, ms_y);
}

void WinDX11CheckBox::OnLeftMouseUp(int ms_x, int ms_y)
{
	if (is_howered)
	{
		Owner()->checked = !Owner()->checked;
	}

	NativeCheckBox::OnLeftMouseUp(ms_x, ms_y);
}

#endif