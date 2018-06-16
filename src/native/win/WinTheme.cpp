
#include "WinTheme.h"
#include "CommCtrl.h"
#include "TCHAR.H"

WinTheme* theme = NULL;

WinTheme::WinTheme() : EUITheme()
{
	themePath[0] = 0;
	LoadLibrary("msimg32.dll");
}

void WinTheme::LoadColors(JSONParser* reader)
{
	while (reader->EnterBlock("colors"))
	{
		char name[128];

		reader->Read("name", name, 128);

		Color& clr = colors[name];

		clr.color = ReadColor(reader, "color");

		reader->LeaveBlock();
	}
}

void WinTheme::LoadFonts(JSONParser* reader)
{
	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

	while (reader->EnterBlock("fonts"))
	{
		char name[128];

		reader->Read("name", name, 128);

		Font& fnt = fonts[name];

		fnt.logFont = lf;

		reader->Read("font", fnt.logFont.lfFaceName, 32);

		int height;
		reader->Read("height", height);
		fnt.logFont.lfHeight = -height;

		bool val = false;

		if (reader->Read("bold", val) && val)
		{
			fnt.logFont.lfWeight += FW_BOLD;
		}

		if (reader->Read("underline", val))
		{
			fnt.logFont.lfUnderline = val;
		}

		reader->LeaveBlock();
	}
}

void WinTheme::LoadCursors(JSONParser* reader)
{
	cursors[""] = LoadCursor(NULL, IDC_ARROW);

	while (reader->EnterBlock("cursors"))
	{
		char name[128];
		reader->Read("name", name, 128);

		char filename[128];
		reader->Read("filename", filename, 128);

		char path[512];
		strcpy(path, themePath);
		strcat(path, filename);

		cursors[name] = (HCURSOR)LoadImage(0, path, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);;

		reader->LeaveBlock();
	}
}

COLORREF WinTheme::ReadColor(JSONParser* reader, const char* name)
{
	COLORREF color = RGB(0, 0, 0);

	if (reader->EnterBlock("color"))
	{
		int r, g, b;
		reader->Read("R", r);
		reader->Read("G", g);
		reader->Read("B", b);

		color = RGB(r, g, b);
		reader->LeaveBlock();
	}

	return color;
}

void WinTheme::ReadTheme(const char* name)
{
	EUITheme::ReadTheme(name);

	int elements[2] = { COLOR_HIGHLIGHT, COLOR_HIGHLIGHTTEXT };
	DWORD newColors[2];

	for (int i = 0; i < 2; i++)
	{
		prevColors[i] = GetSysColor(elements[i]);
	}

	newColors[0] = GetColor(fontBackSeleceted);
	newColors[1] = GetColor(fontSelecetd);

	SetSysColors(2, elements, newColors);

}

void WinTheme::Ulnload()
{
	int elements[3] = { COLOR_HIGHLIGHT, COLOR_HIGHLIGHTTEXT };

	SetSysColors(2, elements, prevColors);
}

HPEN WinTheme::GetPen(const char* color)
{
	if (colors.find(color) == colors.end())
	{
		return NULL;
	}

	Color& clr = colors[color];

	if (clr.pen == NULL)
	{
		clr.pen = ::CreatePen(PS_SOLID, 1, clr.color);
	}

	return clr.pen;
}

HFONT WinTheme::GetFont(const char* font)
{
	if (fonts.find(font) == fonts.end())
	{
		return NULL;
	}

	Font& fnt = fonts[font];

	if (fnt.font == NULL)
	{
		fnt.font = ::CreateFontIndirect(&fnt.logFont);
	}

	return fnt.font;
}

HBRUSH WinTheme::GetBrush(const char* color)
{
	if (colors.find(color) == colors.end())
	{
		return NULL;
	}

	Color& clr = colors[color];

	if (clr.brush == NULL)
	{
		clr.brush = ::CreateSolidBrush(clr.color);
	}

	return clr.brush;
}

HBITMAP WinTheme::GetImage(const char* image)
{
	if (images.find(image) != images.end())
	{
		char imagePath[512];
		strcpy(imagePath, themePath);
		strcat(imagePath, image);

		images[image] = (HBITMAP)LoadImage(0, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	return images[image];
}

HCURSOR WinTheme::GetCursor(const char* name)
{
	return cursors[name];
}

TEXTMETRIC& WinTheme::GetFontInfo(HDC hdc, const char* font)
{
	if (fonts.find(font) == fonts.end())
	{
		return fonts.begin()->second.textMetrics;
	}

	Font& fnt = fonts[font];

	if (fnt.textMetrics.tmHeight == 0)
	{
		HFONT hOldFont = (HFONT) ::SelectObject(hdc, GetFont(font));
		::GetTextMetrics(hdc, &fnt.textMetrics);
		::SelectObject(hdc, hOldFont);
	}

	return fnt.textMetrics;
}

COLORREF WinTheme::GetColor(const char* color)
{
	if (colors.find(color) == colors.end())
	{
		return RGB(0, 0, 0);
	}

	Color& clr = colors[color];

	return clr.color;
}

void WinTheme::DrawRect(HDC hDC, RECT rcItem, const char* Color)
{
	DrawRect(hDC, rcItem, GetColor(Color));
}

void WinTheme::DrawRect(HDC hDC, RECT rcItem, COLORREF clrFill)
{
	::SetBkColor(hDC, clrFill);
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcItem, NULL, 0, NULL);
}

void WinTheme::DrawFrame(HDC hDC, RECT rc, const char* light, const char* dark, const char* back)
{
	if (back)
	{
		DrawRect(hDC, rc, back);
	}

	POINT ptTemp;
	::SelectObject(hDC, GetPen(light));
	::MoveToEx(hDC, rc.left, rc.bottom - 1, &ptTemp);
	::LineTo(hDC, rc.left, rc.top);
	::LineTo(hDC, rc.right - 1, rc.top);
	::SelectObject(hDC, GetPen(dark));
	::LineTo(hDC, rc.right - 1, rc.bottom - 1);
	::LineTo(hDC, rc.left, rc.bottom - 1);
}

void WinTheme::DrawImage(HDC hDC, RECT rc, const char* image)
{
	HDC memDC = CreateCompatibleDC(hDC);

	SelectObject(memDC, GetImage(image));

	BitBlt(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, memDC, 0, 0, SRCCOPY);

	DeleteDC(memDC);
}

void WinTheme::DrawGradient(HDC hDC, RECT rc, COLORREF clrFirst, COLORREF clrSecond, bool bVertical, int nSteps)
{
	typedef BOOL(WINAPI *PGradientFill)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);
	static PGradientFill lpGradientFill = (PGradientFill) ::GetProcAddress(::GetModuleHandle("msimg32.dll"), "GradientFill");
	if (lpGradientFill != NULL)
	{
		TRIVERTEX triv[2] =
		{
			{ rc.left, rc.top, (COLOR16)(GetRValue(clrFirst) << 8), (COLOR16)(GetGValue(clrFirst) << 8), (COLOR16)(GetBValue(clrFirst) << 8), (COLOR16)0xFF00 },
			{ rc.right, rc.bottom, (COLOR16)(GetRValue(clrSecond) << 8), (COLOR16)(GetGValue(clrSecond) << 8), (COLOR16)(GetBValue(clrSecond) << 8), (COLOR16)0xFF00 }
		};
		GRADIENT_RECT grc = { 0, 1 };
		lpGradientFill(hDC, triv, 2, &grc, 1, bVertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
	}
	else
	{
		int nShift = 1;
		if (nSteps >= 64) nShift = 6;
		else if (nSteps >= 32) nShift = 5;
		else if (nSteps >= 16) nShift = 4;
		else if (nSteps >= 8) nShift = 3;
		else if (nSteps >= 4) nShift = 2;
		int nLines = 1 << nShift;
		for (int i = 0; i < nLines; i++)
		{
			// Do a little alpha blending
			BYTE bR = (BYTE)((GetRValue(clrSecond) * (nLines - i) + GetRValue(clrFirst) * i) >> nShift);
			BYTE bG = (BYTE)((GetGValue(clrSecond) * (nLines - i) + GetGValue(clrFirst) * i) >> nShift);
			BYTE bB = (BYTE)((GetBValue(clrSecond) * (nLines - i) + GetBValue(clrFirst) * i) >> nShift);
			// ... then paint with the resulting color
			HBRUSH hBrush = ::CreateSolidBrush(RGB(bR, bG, bB));
			RECT r2 = rc;
			if (bVertical) {
				r2.bottom = rc.bottom - ((i * (rc.bottom - rc.top)) >> nShift);
				r2.top = rc.bottom - (((i + 1) * (rc.bottom - rc.top)) >> nShift);
				if ((r2.bottom - r2.top) > 0) ::FillRect(hDC, &r2, hBrush);
			}
			else {
				r2.left = rc.right - (((i + 1) * (rc.right - rc.left)) >> nShift);
				r2.right = rc.right - ((i * (rc.right - rc.left)) >> nShift);
				if ((r2.right - r2.left) > 0) ::FillRect(hDC, &r2, hBrush);
			}
			::DeleteObject(hBrush);
		}
	}
}

void WinTheme::DrawText(HDC hDC, RECT& rc, LPCTSTR pstrText, const char* iTextColor, const char* iBackColor, UINT uStyle)
{
	// The string formatter supports a kind of "mini-html" that consists of various short tags:
	//
	//   Link:             <a>text</a>
	//   Bold:             <b>text</b>
	//   Color:            <c #xxxxxx>  where x = RGB in hex
	//                     <c x>        where x = color id

	if (::IsRectEmpty(&rc)) return;

	bool bDraw = (uStyle & DT_CALCRECT) == 0;

	RECT rcClip = { 0 };
	::GetClipBox(hDC, &rcClip);
	HRGN hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	HRGN hRgn = ::CreateRectRgnIndirect(&rc);
	if (bDraw) ::ExtSelectClipRgn(hDC, hRgn, RGN_AND);

	HFONT hOldFont = (HFONT) ::SelectObject(hDC, GetFont("FONT_NORMAL"));
	::SetBkMode(hDC, TRANSPARENT);
	::SetTextColor(hDC, GetColor(iTextColor));

	if ((uStyle & DT_SINGLELINE) != 0 && (uStyle & DT_VCENTER) != 0 && (uStyle & DT_CALCRECT) == 0) {
		RECT rcText = { 0, 0, 9999, 100 };
		int nLinks = 0;
		DrawText(hDC, rcText, pstrText, iTextColor, iBackColor, uStyle | DT_CALCRECT);
		rc.top = rc.top + ((rc.bottom - rc.top) / 2) - ((rcText.bottom - rcText.top) / 2);
		rc.bottom = rc.top + (rcText.bottom - rcText.top);
	}
	if ((uStyle & DT_SINGLELINE) != 0 && (uStyle & DT_CENTER) != 0 && (uStyle & DT_CALCRECT) == 0) {
		RECT rcText = { 0, 0, 9999, 100 };
		int nLinks = 0;
		DrawText(hDC, rcText, pstrText, iTextColor, iBackColor, uStyle | DT_CALCRECT);
		::OffsetRect(&rc, (rc.right - rc.left) / 2 - (rcText.right - rcText.left) / 2, 0);
	}
	if ((uStyle & DT_SINGLELINE) != 0 && (uStyle & DT_RIGHT) != 0 && (uStyle & DT_CALCRECT) == 0) {
		RECT rcText = { 0, 0, 9999, 100 };
		int nLinks = 0;
		DrawText(hDC, rcText, pstrText, iTextColor, iBackColor, uStyle | DT_CALCRECT);
		rc.left = rc.right - (rcText.right - rcText.left);
	}

	if (iBackColor)
	{
		DrawRect(hDC, rc, iBackColor);
	}

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	
	TEXTMETRIC tm = GetFontInfo(hDC, "FONT_NORMAL");
	POINT pt = { rc.left, rc.top };
	int iLineIndent = 0;
	int iLinkIndex = 0;
	int cyLine = tm.tmHeight + tm.tmExternalLeading;
	int cyMinHeight = 0;
	POINT ptLinkStart = { 0 };
	bool bInLink = false;

	while (*pstrText != '\0')
	{
		if (pt.x >= rc.right || *pstrText == '\n')
		{
			if ((uStyle & DT_SINGLELINE) != 0) break;
			if (*pstrText == '\n') pstrText++;
			pt.x = rc.left + iLineIndent;
			pt.y += cyLine - tm.tmDescent;
			ptLinkStart = pt;
			cyLine = tm.tmHeight + tm.tmExternalLeading;
			if (pt.x >= rc.right) break;
			while (*pstrText == ' ') pstrText++;
		}
		else if (*pstrText == '<'
			&& (pstrText[1] >= 'a' && pstrText[1] <= 'z')
			&& (pstrText[2] == ' ' || pstrText[2] == '>'))
		{
			pstrText++;
			switch (*pstrText++)
			{
			case 'a':
			{
				::SelectObject(hDC, GetFont("FONT_LINK"));
				tm = GetFontInfo(hDC, "FONT_LINK");
				cyLine = max(cyLine, tm.tmHeight + tm.tmExternalLeading);
				ptLinkStart = pt;
				bInLink = true;
			}
			break;
			case 'b':
			{
				::SelectObject(hDC, GetFont("FONT_BOLD"));
				tm = GetFontInfo(hDC, "FONT_BOLD");
				cyLine = max(cyLine, tm.tmHeight + tm.tmExternalLeading);
			}
			break;
			case 'c':
			{
				if (*pstrText == ' ') pstrText++;
				if (*pstrText == '#')
				{
					pstrText++;
					COLORREF clrColor = _tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 16);
					clrColor = RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor));
					::SetTextColor(hDC, clrColor);
				}
				else
				{
					//UITYPE_COLOR Color = (UITYPE_COLOR)_tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
					//::SetTextColor(hDC, GetColor(Color));
				}
			}
			break;
			}
			while (*pstrText != '\0' && *pstrText != '>') pstrText++;
			pstrText++;
		}
		else
		if (*pstrText == '<' && pstrText[1] == '/')
		{
			pstrText += 2;
			switch (*pstrText++)
			{
				case 'a':
					::SelectObject(hDC, GetFont("FONT_NORMAL"));
					tm = GetFontInfo(hDC, "FONT_NORMAL");
					bInLink = false;
				break;
				case 'f':
				case 'b':
					::SelectObject(hDC, GetFont("FONT_NORMAL"));
					tm = GetFontInfo(hDC, "FONT_NORMAL");
				break;
				case 'c':
					::SetTextColor(hDC, GetColor(iTextColor));
				break;
			}
			while (*pstrText != '\0' && *pstrText != '>') pstrText++;
			pstrText++;
		}
		else
		if (*pstrText == '&')
		{
			if ((uStyle & DT_NOPREFIX) == 0)
			{
				::TextOut(hDC, pt.x, pt.y, "_", 1);
			}
			else
			{
				SIZE szChar = { 0 };
				::GetTextExtentPoint32(hDC, "&", 1, &szChar);
				if (bDraw) ::TextOut(hDC, pt.x, pt.y, "&", 1);
				pt.x += szChar.cx;
			}
			pstrText++;
		}
		else
		if (*pstrText == ' ')
		{
			SIZE szSpace = { 0 };
			::GetTextExtentPoint32(hDC, " ", 1, &szSpace);
			if (bDraw) ::TextOut(hDC, pt.x, pt.y, " ", 1);
			pt.x += szSpace.cx;
			pstrText++;
		}
		else
		{
			POINT ptPos = pt;
			int cchChars = 0;
			int cchLastGoodWord = 0;
			LPCTSTR p = pstrText;
			SIZE szText = { 0 };
			if (*p == '<') p++, cchChars++;
			while (*p != '\0' && *p != '<' && *p != '\n' && *p != '&')
			{
				cchChars++;
				szText.cx = cchChars * tm.tmMaxCharWidth;
				if (pt.x + szText.cx >= rc.right)
				{
					::GetTextExtentPoint32(hDC, pstrText, cchChars, &szText);
				}
				if (pt.x + szText.cx >= rc.right)
				{
					if ((uStyle & DT_WORDBREAK) != 0 && cchLastGoodWord > 0)
					{
						cchChars = cchLastGoodWord;
						pt.x = rc.right;
					}
					if ((uStyle & DT_END_ELLIPSIS) != 0 && cchChars > 2)
					{
						cchChars -= 2;
						pt.x = rc.right;
					}
					break;
				}
				if (*p == ' ') cchLastGoodWord = cchChars;
				p = ::CharNext(p);
			}
			if (cchChars > 0)
			{
				::GetTextExtentPoint32(hDC, pstrText, cchChars, &szText);
				if (bDraw)
				{
					::TextOut(hDC, ptPos.x, ptPos.y, pstrText, cchChars);
					if (pt.x == rc.right && (uStyle & DT_END_ELLIPSIS) != 0) ::TextOut(hDC, rc.right - 10, ptPos.y, "...", 3);
				}
				pt.x += szText.cx;
				pstrText += cchChars;
			}
		}
	}

	if ((uStyle & DT_CALCRECT) != 0)
	{
		rc.bottom = max(cyMinHeight, pt.y + cyLine);
		if (rc.right >= 9999)
		{
			if (_tcslen(pstrText) > 0) pt.x += 3;
			rc.right = pt.x;
		}
	}

	if (bDraw) ::SelectClipRgn(hDC, hOldRgn);
	::DeleteObject(hOldRgn);
	::DeleteObject(hRgn);

	::SelectObject(hDC, hOldFont);
}

void WinTheme::DrawButton(HDC hDC, RECT rc, LPCTSTR pstrText, UINT uState, UINT uStyle)
{
	ButtonColors* btn;
	
	if (uState & UISTATE_HOWERED)
	{
		btn = &buttonColors[1];
	}
	else
	if (uState & UISTATE_PUSHED)
	{
		btn = &buttonColors[2];
	}
	else
	if (uState & UISTATE_DISABLED)
	{
		btn = &buttonColors[3];
	}
	else
	{
		btn = &buttonColors[0];
	}

	DrawFrame(hDC, rc, btn->borderFromColor, btn->borderToColor, NULL);
	::InflateRect(&rc, -1, -1);
	DrawGradient(hDC, rc, GetColor(btn->backFromColor), GetColor(btn->backToColor), true, 32);

	RECT rcText = rc;
	::InflateRect(&rcText, -1, -1);

	DrawText(hDC, rcText, pstrText, btn->textColor, NULL, DT_SINGLELINE | uStyle);

	if (uState & UISTATE_FOCUSED)
	{
		DrawFrame(hDC, rc, buttonFocusColor, buttonFocusColor, NULL);
		::InflateRect(&rc, -1, -1);
	}
}

void WinTheme::DrawLabel(HDC hDC, RECT rc, LPCTSTR pstrText, UINT uState, UINT uStyle)
{
	ButtonColors* btn;

	if (uState & UISTATE_DISABLED)
	{
		btn = &buttonColors[3];
	}
	else
	{
		btn = &buttonColors[0];
	}

	RECT rcText = rc;
	::InflateRect(&rcText, -1, -1);

	DrawText(hDC, rcText, pstrText, btn->textColor, NULL, DT_SINGLELINE | uStyle);
}

void WinTheme::DrawCheckBox(HDC hDC, RECT rc, LPCTSTR pstrText, UINT uState, UINT uStyle)
{
	CheckBoxColors* chk;

	if (uState & UISTATE_DISABLED)
	{
		chk = &checkBoxColors[1];
	}
	else
	{
		chk = &checkBoxColors[0];
	}

	RECT box_rc = rc;

	box_rc.left += 2;
	float box_y = (rc.top + rc.bottom) * 0.5f - checkBoxSize * 0.5f;
	box_rc.top = (int)box_y;
	box_rc.bottom = (int)box_y + checkBoxSize;

	box_rc.right = box_rc.left + checkBoxSize;

	DrawFrame(hDC, box_rc, chk->boxBorderColor, chk->boxBorderColor, NULL);
	::InflateRect(&box_rc, -1, -1);

	if (uState & UISTATE_PUSHED)
	{
		DrawFrame(hDC, box_rc, chk->boxBackColor, chk->boxBackColor, NULL);
		::InflateRect(&box_rc, -1, -1);
		DrawGradient(hDC, box_rc, GetColor(chk->boxCheckColor), GetColor(chk->boxCheckColor), true, 32);
	}
	else
	{
		DrawGradient(hDC, box_rc, GetColor(chk->boxBackColor), GetColor(chk->boxBackColor), true, 32);
	}

	RECT rcText = rc;

	rcText.left += rc.left + checkBoxSize + 4;
	::InflateRect(&rcText, -1, -1);

	DrawText(hDC, rcText, pstrText, chk->textColor, NULL, DT_VCENTER | DT_SINGLELINE | uStyle);
}

void WinTheme::DrawCategory(HDC hDC, RECT rc, LPCTSTR pstrText, UINT uState, UINT uStyle)
{
	CategoriesColors* cat;

	if (uState & UISTATE_DISABLED)
	{
		cat = &categoriesColors[1];
	}
	else
	{
		cat = &categoriesColors[0];
	}

	DrawFrame(hDC, rc, cat->borderColor, cat->borderColor, NULL);
	::InflateRect(&rc, -1, -1);
	DrawGradient(hDC, rc, GetColor(cat->backColor), GetColor(cat->backColor), true, 32);

	RECT rcText = rc;
	::InflateRect(&rcText, -1, -1);

	if (strlen(pstrText) > 1)
	{
		int thin = rcText.bottom - rcText.top;
		RECT rcImage = rcText;

		rcImage.right = rcImage.left + thin;

		if (uState & UISTATE_PUSHED)
		{
			DrawImage(hDC, rcImage, cat->openedImage);
		}
		else
		{
			DrawImage(hDC, rcImage, cat->closedImage);
		}

		rcText.left += thin;
	}

	DrawText(hDC, rcText, pstrText, cat->textColor, NULL, DT_SINGLELINE | uStyle);
}

void WinTheme::DrawScrollBar(HDC hDC, RECT rc, int pos, int size, UINT uState)
{
	RECT rcItem = { rc.right - scrollbarThin, 0, rc.right, scrollbarThin };

	ScrollbarColors* scrl;

	if (uState & UISTATE_DISABLED)
	{
		scrl = &scrollbarColors[1];
	}
	else
	{
		scrl = &scrollbarColors[0];
	}

	DrawFrame(hDC, rcItem, scrl->topBorderColor, scrl->topBorderColor, scrl->topBorderColor);

	RECT rcImage = rcItem;
	::InflateRect(&rcImage, -2, -2);
	DrawImage(hDC, rcImage, scrl->topImage);

	rcItem.top = scrollbarThin;
	rcItem.bottom = rc.bottom - scrollbarThin;

	DrawFrame(hDC, rcItem, scrl->middleBorderColor, scrl->middleBackColor, scrl->topBorderColor);

	rcItem.top = rc.bottom - scrollbarThin;
	rcItem.bottom = rc.bottom;

	DrawFrame(hDC, rcItem, scrl->bottomBorderColor, scrl->bottomBackColor, scrl->topBorderColor);

	rcImage = rcItem;
	::InflateRect(&rcImage, -2, -2);
	DrawImage(hDC, rcImage, scrl->bottomImage);

	rcItem.top = scrollbarThin + scrollbarPaddingY + pos;
	rcItem.bottom = rcItem.top + size;
	rcItem.left += scrollbarPaddingX;
	rcItem.right -= scrollbarPaddingX;

	DrawFrame(hDC, rcItem, scrl->thumbBorderColor, scrl->thumbBorderColor, scrl->thumbBackColor);
}