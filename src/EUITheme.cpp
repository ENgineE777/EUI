
#include "EUITheme.h"
#include "TCHAR.H"

EUITheme::EUITheme()
{
	themePath[0] = 0;
}

void EUITheme::ReadTheme(const char* name)
{
	strcpy(themePath, name);

	int index = strlen(themePath) - 1;

	while (themePath[index] != '/' && themePath[index] != '\\' && index >= 0)
	{
		index--;
	}

	themePath[index + 1] = 0;

	JSONReader* reader = new JSONReader();

	if (!reader->Parse(name))
	{
		return;
	}

	LoadColors(reader);
	LoadFonts(reader);

	reader->Read("FONT_SELECTED", fontSelecetd, 32);
	reader->Read("FONT_BACK_SELECTED", fontBackSeleceted, 32);

	if (reader->EnterBlock("BUTTON"))
	{
		reader->Read("BORDER_FOCUS", buttonFocusColor, 32);

		const char* names[] = { "BUTTON_NORMAL", "BUTTON_HOVERED", "BUTTON_PUSHED", "BUTTON_DISABLED" };

		for (int i = 0; i < 4; i++)
		{
			if (reader->EnterBlock(names[i]))
			{
				ButtonColors& btn = buttonColors[i];

				reader->Read("BACK_FROM_COLOR", btn.backFromColor, 32);
				reader->Read("BACK_TO_COLOR", btn.backToColor, 32);
				reader->Read("BORDER_FROM_COLOR", btn.borderFromColor, 32);
				reader->Read("BORDER_TO_COLOR", btn.borderToColor, 32);
				reader->Read("TEXT_COLOR", btn.textColor, 32);
				reader->Read("TEXT_FONT", btn.font, 32);

				reader->LeaveBlock();
			}
		}

		reader->LeaveBlock();
	}

	if (reader->EnterBlock("CHECKBOX"))
	{
		reader->Read("BOX_SIZE", checkBoxSize);

		const char* names[] = {"CHECKBOX_NORMAL", "CHECKBOX_DISABLED"};
		
		for (int i = 0; i < 2; i++)
		{
			if (reader->EnterBlock(names[i]))
			{
				CheckBoxColors& chk = checkBoxColors[i];

				reader->Read("BOX_BORDER_COLOR", chk.boxBorderColor, 32);
				reader->Read("BOX_BACK_COLOR", chk.boxBackColor, 32);
				reader->Read("BOX_CHECK_COLOR", chk.boxCheckColor, 32);
				reader->Read("TEXT_COLOR", chk.textColor, 32);
				reader->Read("TEXT_FONT", chk.font, 32);

				reader->LeaveBlock();
			}
		}

		reader->LeaveBlock();
	}

	if (reader->EnterBlock("CATEGORIES"))
	{
		reader->Read("CATEGORY_HEIGHT", categoryHeight);

		const char* names[] = { "CATEGORY_NORMAL", "CATEGORY_DISABLED" };

		for (int i = 0; i < 2; i++)
		{
			if (reader->EnterBlock(names[i]))
			{
				CategoriesColors& chk = categoriesColors[i];

				reader->Read("BORDER_COLOR", chk.borderColor, 32);
				reader->Read("BACK_COLOR", chk.backColor, 32);
				reader->Read("OPENED_IMAGE", chk.openedImage, 32);
				reader->Read("CLOSED_IMAGE", chk.closedImage, 32);
				reader->Read("TEXT_COLOR", chk.textColor, 32);
				reader->Read("TEXT_FONT", chk.font, 32);

				reader->LeaveBlock();
			}
		}

		reader->LeaveBlock();
	}

	if (reader->EnterBlock("SCROLLBAR"))
	{
		reader->Read("THIN", scrollbarThin);
		reader->Read("THUMB_PADDING_X", scrollbarPaddingX);
		reader->Read("THUMB_PADDING_Y", scrollbarPaddingY);

		const char* names[] = { "SCROLLBAR_NORMAL", "SCROLLBAR_DISABLED" };

		for (int i = 0; i < 2; i++)
		{
			if (reader->EnterBlock(names[i]))
			{
				ScrollbarColors& scrl = scrollbarColors[i];

				reader->Read("TOP_BORDER_COLOR", scrl.topBorderColor, 32);
				reader->Read("TOP_BACK_COLOR", scrl.topBackColor, 32);
				reader->Read("TOP_IMAGE", scrl.topImage, 32);
				reader->Read("MIDDLE_BORDER_COLOR", scrl.middleBorderColor, 32);
				reader->Read("MIDDLE_BACK_COLOR", scrl.middleBackColor, 32);
				reader->Read("BOTTOM_BORDER_COLOR", scrl.bottomBorderColor, 32);
				reader->Read("BOTTOM_BACK_COLOR", scrl.bottomBackColor, 32);
				reader->Read("BOTTOM_IMAGE", scrl.bottomImage, 32);
				reader->Read("THUMB_BORDER_COLOR", scrl.thumbBorderColor, 32);
				reader->Read("THUMB_BACK_COLOR", scrl.thumbBackColor, 32);

				reader->LeaveBlock();
			}
		}

		reader->LeaveBlock();
	}
}