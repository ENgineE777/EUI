
#pragma once

#include "windows.h"
#include <map>
#include "EUITheme.h"

#ifdef PLATFORM_WIN

class WinTheme : public EUITheme
{
	struct Color
	{
		HPEN     pen;
		HBRUSH   brush;
		COLORREF color;

		Color()
		{
			pen = 0;
			brush = 0;
			color = 0;
		}
	};

	struct Font
	{
		HFONT font;
		LOGFONT logFont;
		TEXTMETRIC textMetrics;

		Font()
		{
			font = 0;
			textMetrics.tmHeight = 0;
		}
	};

	struct ButtonColors
	{
		char backFromColor[32];
		char backToColor[32];
		char borderFromColor[32];
		char borderToColor[32];
		char textColor[32];
		char font[32];
	};

	struct CheckBoxColors
	{
		char boxBorderColor[32];
		char boxBackColor[32];
		char boxCheckColor[32];
		char textColor[32];
		char font[32];
	};

	struct CategoriesColors
	{
		char borderColor[32];
		char backColor[32];
		char openedImage[32];
		char closedImage[32];
		char textColor[32];
		char font[32];
	};

	struct ScrollbarColors
	{
		char topBorderColor[32];
		char topBackColor[32];
		char topImage[32];
		char middleBorderColor[32];
		char middleBackColor[32];
		char bottomBorderColor[32];
		char bottomBackColor[32];
		char bottomImage[32];
		char thumbBorderColor[32];
		char thumbBackColor[32];
	};

public:

	char             fontSelecetd[32];
	char             fontBackSeleceted[32];
	char             buttonFocusColor[32];
	ButtonColors     buttonColors[4];
	int              checkBoxSize;
	CheckBoxColors   checkBoxColors[2];
	CategoriesColors categoriesColors[2];
	int              scrollbarThin;
	int              scrollbarPaddingX;
	int              scrollbarPaddingY;
	ScrollbarColors  scrollbarColors[2];

	enum State
	{
		UISTATE_NORMAL = 1,
		UISTATE_HOWERED = 2,
		UISTATE_PUSHED = 4,
		UISTATE_DISABLED = 8,
		UISTATE_FOCUSED = 16
	};

	std::map<std::string, Color> colors;
	std::map<std::string, Font> fonts;
	std::map<std::string, HBITMAP> images;
	std::map<std::string, HCURSOR> cursors;

	DWORD prevColors[2];

	WinTheme();

	void ReadTheme(JSONParser& parser) override;
	void Ulnload() override;

	HPEN        GetPen(const char* color);
	HFONT       GetFont(const char* font);
	HBRUSH      GetBrush(const char* color);
	COLORREF    GetColor(const char* color);
	HBITMAP     GetImage(const char* image);
	HCURSOR     GetCursor(const char* name);
	TEXTMETRIC& GetFontInfo(HDC hdc, const char* font);

	void DrawRect(HDC hDC, RECT rc, const char* color);
	void DrawRect(HDC hDC, RECT rc, COLORREF clrFill);

	void DrawGradient(HDC hDC, RECT rc, COLORREF from, COLORREF to, bool vertical, int steps);
	void DrawText(HDC hDC, RECT& rc, LPCTSTR pstrText, const char* textColor, const char* backColor, UINT uStyle);
	void DrawFrame(HDC hDC, RECT rc, const char* light, const char* dark, const char* back);
	void DrawImage(HDC hDC, RECT rc, const char* image);

	void DrawButton(HDC hDC, RECT rc, LPCTSTR pstrText, UINT uState, UINT uStyle);
	void DrawLabel(HDC hDC, RECT rc, LPCTSTR pstrText, UINT uState, UINT uStyle);
	void DrawCheckBox(HDC hDC, RECT rc, LPCTSTR pstrText, UINT uState, UINT uStyle);
	void DrawCategory(HDC hDC, RECT rc, LPCTSTR pstrText, UINT uState, UINT uStyle);
	void DrawScrollBar(HDC hDC, RECT rc, int pos, int size, UINT uState);

protected:
	COLORREF ReadColor(JSONParser& reader, const char* name);
	void LoadColors(JSONParser& reader);
	void LoadFonts(JSONParser& reader);
	void LoadCursors(JSONParser& reader);
};

extern WinTheme* theme;
#endif