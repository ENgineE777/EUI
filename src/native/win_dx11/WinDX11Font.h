#pragma once

#ifdef PLATFORM_WIN_DX11

#include "d3d11.h"
#include <map>
#include <vector>

class WinDX11Font
{
public:

	struct Glyph
	{
		int   width;
		int   height;
		float x_offset;
		float y_offset;
		float x_advance;
		float u;
		float v;
		float du;
		float dv;
		int   skip;
	};

	struct LineBreak
	{
		float offset = 0.0f;
		float width  = 0.0f;
		int index = -1;
	};

protected:

	int      cur_tex_x = 0;
	int      cur_tex_y = 0;
	int      cur_tex_row_height = 0;

	uint8_t* tex_buffer = nullptr;

	int height = 10;
	float used_height = 10;

	int tex_w = -1;
	int tex_h = -1;

	struct stbtt_pack_context* context = nullptr;
	bool need_update_tex = false;
	uint8_t* font_buffer = nullptr;

	std::map<int, Glyph> glyphs;

	ID3D11Texture2D* texture = nullptr;

public:

	ID3D11ShaderResourceView * srview = nullptr;

	bool Load(uint8_t* font_buffer, int hgt);
	Glyph* GenerateChar(int ch);

	int CalcWidth(const char* text);
	int CalcOffset(const char* text, int width);
	void Print(int x, int y, float* color, const char* text);

	int GetHeight();
	int GetCharHeight();

	void UpdateTexture();

	Glyph* GetGlyph(int code);
};

#endif
