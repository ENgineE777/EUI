
#include "WinDX11Theme.h"
#include "UTFConv.h"

#ifdef PLATFORM_WIN_DX11

#include "stb/stb_truetype.h"

WinDX11Font::Glyph* WinDX11Font::GenerateChar(int ch)
{
	Glyph& set_glyph = glyphs[ch];

	stbtt_packedchar packed;
	stbtt_PackFontRange(context, font_buffer, 0, STBTT_POINT_SIZE(used_height), ch, 1, &packed);

	float w = packed.xoff2 - packed.xoff;
	float h = packed.yoff2 - packed.yoff;

	set_glyph.width = (packed.x1 - packed.x0);
	set_glyph.height = (packed.y1 - packed.y0);

	set_glyph.x_offset = (float)packed.xoff;
	set_glyph.y_offset = (float)packed.yoff + height;

	set_glyph.x_advance = packed.xadvance;

	if (set_glyph.width > 0 && set_glyph.height > 0)
	{
		set_glyph.u = (float)(packed.x0) / (float)(tex_w);
		set_glyph.v = (float)(packed.y0) / (float)(tex_h);

		set_glyph.du = (float)(set_glyph.width) / (float)(tex_w);
		set_glyph.dv = (float)(set_glyph.height) / (float)(tex_h);

		set_glyph.skip = 0;
	}
	else
	{
		set_glyph.x_offset = 0;
		set_glyph.x_advance = height * 0.45f;
		set_glyph.skip = 1;
	}

	need_update_tex = true;

	return &set_glyph;
}

bool WinDX11Font::Load(uint8_t* set_font_buffer, int set_height)
{
	context = new stbtt_pack_context();

	font_buffer = set_font_buffer;

	height = set_height;
	used_height = height * 1.38f;

	int nums = (int)(1024.0f / height);

	tex_w = 1024;
	tex_h = 128;// GetPow2((int)((400.0f / nums) * height));

	if (tex_h > 2048)
	{
		tex_h = 2048;
	}

	cur_tex_x = 3;
	cur_tex_y = 1;
	cur_tex_row_height = 0;

	tex_buffer = new uint8_t[tex_w * tex_h];
	memset(tex_buffer, 0, tex_w * tex_h);

	stbtt_PackBegin(context, tex_buffer, tex_w, tex_h, 0, 1, nullptr);

	D3D11_TEXTURE2D_DESC desc;

	desc.Width = tex_w;
	desc.Height = tex_h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.Usage = D3D11_USAGE_DEFAULT;

	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	theme->pd3dDevice->CreateTexture2D(&desc, nullptr, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	srvDesc.Format = DXGI_FORMAT_R8_UNORM;

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	theme->pd3dDevice->CreateShaderResourceView(texture, &srvDesc, &srview);

	return true;
}

WinDX11Font::Glyph* WinDX11Font::GetGlyph(int code)
{
	if (glyphs.find(code) == glyphs.end())
	{
		return GenerateChar(code);
	}

	return &glyphs[code];
}

int WinDX11Font::GetIndex(int pos_x, const char* text)
{
	int len = (int)strlen(text);

	if (len == 0) return 0;

	float scr_x = (float)0.5f;

	int w = 0;
	int bytes = 0;

	int index = -1;

	for (int i = 0; i<len; i++)
	{
		if (!UTFConv::BuildUtf16fromUtf8(text[i], bytes, w))
		{
			continue;
		}

		if (w > 65000) continue;

		if (w == 10)
		{
			continue;
		}
		else
		if (w == '\\')
		{
			if (i <len - 1)
			{
				if (text[i + 1] == 'n')
				{
					i++;
					continue;
				}
			}
		}

		Glyph* set_glyph = GetGlyph(w);
		if (!set_glyph) continue;

		if (set_glyph->skip == 0)
		{
			int char_x = (int)(scr_x + set_glyph->x_offset);
			int char_w = (int)(set_glyph->width * 0.5f);

			if (char_x <= pos_x && pos_x <= char_x + char_w)
			{
				index = i;
			}
			else
			if (char_x  + char_w <= pos_x && pos_x <= char_x + char_w * 2 + 1)
			{
				index = i + 1;
			}

		}

		scr_x += set_glyph->x_advance;
	}

	return index == -1? len : index;
}

int WinDX11Font::CalcWidth(const char* text, int index)
{
	int len = (int)strlen(text);

	if (len == 0) return 0;

	float scr_x = 0;

	int w = 0;
	int bytes = 0;

	for (int i = 0; i<len; i++)
	{
		if (!UTFConv::BuildUtf16fromUtf8(text[i], bytes, w))
		{
			continue;
		}

		if (w > 65000) continue;

		if (i == index)
		{
			break;
		}

		if (w == 10)
		{
			continue;
		}
		else
		if (w == '\\')
		{
			if (i <len - 1)
			{
				if (text[i + 1] == 'n')
				{
					i++;
					continue;
				}
			}
		}

		Glyph* set_glyph = GetGlyph(w);
		if (!set_glyph) continue;

		if (i == (len - 1) && !set_glyph->skip)
		{
			scr_x += set_glyph->x_offset + set_glyph->width;
		}
		else
		{
			scr_x += set_glyph->x_advance;
		}
	}

	return (int)scr_x;
}

int WinDX11Font::CalcOffset(const char* text, int width)
{
	int scr_x = CalcWidth(text);

	if (width > scr_x)
	{
		return (int)((width - scr_x) * 0.5f);
	}

	return 0;
}

void WinDX11Font::Print(int x, int y, float* color, const char* text)
{
	if (!texture) return;

	//render.DebugSprite(tex, 0.0f, 1024.0f);

	int len = (int)strlen(text);

	if (len == 0) return;

	float scr_x = (float)x;

	scr_x += 0.5f;

	float scr_y = (float)y;

	scr_y += 0.5f;

	int w = 0;
	int bytes = 0;

	for (int i=0;i<len;i++)
	{
		if (!UTFConv::BuildUtf16fromUtf8(text[i], bytes, w))
		{
			continue;
		}

		if (w > 65000) continue;

		if (w == 10)
		{
			continue;
		}
		else
		if (w == '\\')
		{
			if (i <len-1)
			{
				if (text[i+1] == 'n')
				{
					i++;
					continue;
				}
			}
		}
		
		Glyph* set_glyph = GetGlyph(w);
		if (!set_glyph) continue;

		if (set_glyph->skip == 0)
		{
			int char_x = (int)(scr_x + set_glyph->x_offset);
			int char_y = (int)(scr_y + set_glyph->y_offset - 0.5f);
			int char_w = set_glyph->width;
			int char_h = set_glyph->height;

			float char_u = set_glyph->u;
			float char_v = set_glyph->v;

			float char_du = set_glyph->du;
			float char_dv = set_glyph->dv;

			if (need_update_tex)
			{
				UpdateTexture();
			}

			theme->Draw(srview, color, char_x, char_y, char_w, char_h, char_u, char_v, char_du, char_dv);
		}
		
		scr_x += set_glyph->x_advance;
	}
}

int WinDX11Font::GetHeight()
{
	return height;
}

int WinDX11Font::GetCharHeight()
{
	return height;
}

void WinDX11Font::UpdateTexture()
{
	need_update_tex = false;

	UINT index = D3D11CalcSubresource(0, 0, 1);
	theme->immediateContext->UpdateSubresource(texture, index, NULL, tex_buffer, tex_w, 0);
}

/*
void UTFConv::Release()
{
	DELETE_PTR(context)
	if (tex_buffer)
	{
		free(tex_buffer);
		stbtt_PackEnd(context);
	}

	RELEASE(tex)

	delete this;
}*/

#endif

