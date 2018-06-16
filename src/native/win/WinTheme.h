
#pragma once

#include "windows.h"
#include <map>
#include "EUITheme.h"

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

public:

	std::map<std::string, Color> colors;
	std::map<std::string, Font> fonts;
	std::map<std::string, HBITMAP> images;
	std::map<std::string, HCURSOR> cursors;

	DWORD prevColors[2];

	WinTheme();

	virtual void ReadTheme(const char* name);
	virtual void Ulnload();

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
	COLORREF ReadColor(JSONParser* reader, const char* name);
	void LoadColors(JSONParser* reader) override;
	void LoadFonts(JSONParser* reader) override;
	void LoadCursors(JSONParser* reader) override;
};

extern WinTheme* theme;