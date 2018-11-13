
#pragma once

#ifdef PLATFORM_WIN_DX11

#include "d3d11.h"
#include <map>
#include "EUITheme.h"
#include "WinDX11Font.h"

class WinDX11Theme : public EUITheme
{
public:

	ID3D11Device*           pd3dDevice = nullptr;
	IDXGIFactory1*          factory = nullptr;
	ID3D11DeviceContext*    immediateContext = nullptr;

	struct WindowData
	{
		IDXGISwapChain*         swapChain = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;
		ID3D11Texture2D*        depthStencil = nullptr;
		ID3D11DepthStencilView* depthStencilView = nullptr;
	};

	ID3D11Buffer* buffer = nullptr;
	ID3D11InputLayout* layout = nullptr;
	ID3D11VertexShader* vshader;
	ID3D11PixelShader*  pshader;
	ID3D11Buffer* struct_buffer = nullptr;
	ID3D11ShaderResourceView* sbuffer_srview;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srview;
	ID3D11SamplerState* sampler;

	ID3D11RasterizerState* raster_state = nullptr;
	ID3D11BlendState* blend_state = nullptr;

	uint32_t scr_width = 0;
	uint32_t scr_height = 0;

	struct Elem
	{
		float u, v, du, dv;
		float offset_u = -1;
		float offset_v = -1;
	};

	std::map<std::string, Elem> elems;

	enum
	{
		MaxInstCount = 1000
	};

	struct Params
	{
		float x, y, width, height;
		float u, v, du, dv;
		float scr_width, scr_height;
		int   texture, dummy1;
		float r, g, b, a;
		float dummy[16];
	};

	int inst_count = 0;
	Params params;
	Params* data_buffer = nullptr;

	WinDX11Font font;
	int clamp_x = 0;
	int clamp_y = 0;
	int clamp_x2 = 0;
	int clamp_y2 = 0;

	std::map<std::string, HCURSOR> cursors;

public:

	WinDX11Theme();

	void* GetRenderDevice() override;
	void ReadTheme(JSONParser& parser) override;
	void Ulnload() override;

	void SetOutputWnd(WindowData& data, HWND hwnd, int wgt, int hgt);

	HCURSOR GetCursor(const char* name);

	void SetClampBorder(int x, int y, int w, int h);
	void SetScreenSize(WindowData& data, int scr_width, int scr_height);
	void Draw(const char* elem, int x, int y, int width, int height);
	void Draw(void* texture, float* color, int x, int y, int width, int height, float u = 0.0f, float v = 0.0f, float du = 1.0f, float dv = 1.0f);

	inline void Start()
	{
		D3D11_MAPPED_SUBRESOURCE res;
		immediateContext->Map(struct_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		data_buffer = (Params*)res.pData;
		inst_count = 0;
	}

	inline bool WinDX11Theme::ClampRect()
	{
		if (clamp_x > params.x + params.width)
		{
			return false;
		}

		if (params.x  > clamp_x2)
		{
			return false;
		}

		if (clamp_y > params.y + params.height)
		{
			return false;
		}

		if (params.y  > clamp_y2 + params.height)
		{
			return false;
		}

		if (clamp_x > params.x)
		{
			float k = (clamp_x - params.x) / params.width;
			params.width *= (1.0f - k);
			params.x = (float)clamp_x;

			params.u += params.du * k;
			params.du *= (1.0f - k);
		}

		if (params.x + params.width > clamp_x2)
		{
			float k = (clamp_x2 - params.x) / params.width;
			params.width *= k;

			params.du *= k;
		}

		if (clamp_y > params.y)
		{
			float k = (clamp_y - params.y) / params.height;
			params.height *= (1.0f - k);
			params.y = (float)clamp_y;

			params.v += params.dv * k;
			params.dv *= (1.0f - k);
		}

		if (params.y + params.height > clamp_y2)
		{
			float k = (clamp_y2 - params.y) / params.height;
			params.height *= k;

			params.dv *= k;
		}

		return true;
	}

	inline void GetNext()
	{
		memcpy(data_buffer, &params, sizeof(params));

		inst_count++;

		if (inst_count >= MaxInstCount)
		{
			immediateContext->Unmap(struct_buffer, 0);

			immediateContext->DrawInstanced(4, inst_count, 0, 0);

			Start();
		}
		else
		{
			data_buffer++;
		}
	}
	void Present(WindowData& data);
};

extern WinDX11Theme* theme;
#endif