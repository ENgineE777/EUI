
#include "WinDX11Button.h"

#ifdef PLATFORM_WIN_DX11

#include "CommCtrl.h"
#include "TCHAR.H"

#include "ui_vs.h"
#include "ui_ps.h"

#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM


#include "stb/stb_image.h"

WinDX11Theme* theme = nullptr;

WinDX11Theme::WinDX11Theme() : EUITheme()
{
	themePath[0] = 0;
	LoadLibrary("msimg32.dll");
}

void WinDX11Theme::ReadTheme(JSONParser& parser)
{
	parser.Read("CATEGORY_HEIGHT", categoryHeight);

	while (parser.EnterBlock("Elems"))
	{
		char name[128];
		parser.Read("name", name, 128);

		Elem& elem = elems[name];

		parser.Read("u", elem.u);
		parser.Read("v", elem.v);
		parser.Read("du", elem.du);
		parser.Read("dv", elem.dv);
		parser.Read("offset_u", elem.offset_u);
		parser.Read("offset_v", elem.offset_v);

		parser.LeaveBlock();
	}

	cursors[""] = LoadCursor(NULL, IDC_ARROW);

	while (parser.EnterBlock("cursors"))
	{
		char name[128];
		parser.Read("name", name, 128);

		char filename[128];
		parser.Read("filename", filename, 128);

		char path[512];
		strcpy(path, themePath);
		strcat(path, filename);

		cursors[name] = (HCURSOR)::LoadImage(0, path, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);;

		parser.LeaveBlock();
	}

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE   driverType;
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr;

	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&factory));

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &pd3dDevice, &featureLevel, &immediateContext);

		if (SUCCEEDED(hr))
		{
			break;
		}
	}

	float v[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(v);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pd3dDevice->CreateBuffer(&bd, NULL, &buffer);

	D3D11_MAPPED_SUBRESOURCE res;
	immediateContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

	memcpy(res.pData, v, sizeof(v));

	immediateContext->Unmap(buffer, 0);

	hr = pd3dDevice->CreateVertexShader(g_VS, sizeof(g_VS), nullptr, &vshader);
	hr = pd3dDevice->CreatePixelShader(g_PS, sizeof(g_PS), nullptr, &pshader);

	D3D11_INPUT_ELEMENT_DESC elementDesc;

	elementDesc.SemanticIndex = 0;
	elementDesc.InputSlot = 0;
	elementDesc.AlignedByteOffset = 0;
	elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDesc.InstanceDataStepRate = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	elementDesc.SemanticName = "POSITION";

	hr = pd3dDevice->CreateInputLayout(&elementDesc, 1, g_VS, sizeof(g_VS), &layout);

	immediateContext->VSSetShader(vshader, 0, 0);
	immediateContext->PSSetShader(pshader, 0, 0);

	immediateContext->IASetInputLayout(layout);

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = 128 * MaxInstCount;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = 128;

	pd3dDevice->CreateBuffer(&bd, NULL, &struct_buffer);

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.ElementWidth = MaxInstCount;
		pd3dDevice->CreateShaderResourceView(struct_buffer, &srvDesc, &sbuffer_srview);
	}

	std::string tex_name;
	parser.Read("SKIN_TEXTURE", tex_name);

	uint32_t tex_size = 0;
	uint8_t* tex_buffer = LoadFile(tex_name.c_str(), tex_size);

	int bytes;
	uint8_t* data = stbi_load_from_memory(tex_buffer, tex_size, &skin_width, &skin_height, &bytes, STBI_rgb_alpha);

	D3D11_TEXTURE2D_DESC desc;

	desc.Width = skin_width;
	desc.Height = skin_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.Usage = D3D11_USAGE_DEFAULT;

	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	pd3dDevice->CreateTexture2D(&desc, NULL, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	pd3dDevice->CreateShaderResourceView(texture, &srvDesc, &srview);

	UINT index = D3D11CalcSubresource(0, 0, 1);
	immediateContext->UpdateSubresource(texture, index, nullptr, data, skin_width * 4, 0);

	free(data);
	free(tex_buffer);

	for (auto& elem : elems)
	{
		elem.second.u /= (float)skin_width;
		elem.second.v /= (float)skin_height;
		elem.second.du /= (float)skin_width;
		elem.second.dv /= (float)skin_height;
		elem.second.offset_du = elem.second.offset_u / (float)skin_width;
		elem.second.offset_dv = elem.second.offset_v / (float)skin_height;
	}

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	pd3dDevice->CreateSamplerState(&sampDesc, &sampler);

	D3D11_RASTERIZER_DESC raster_desc;// = new D3D11_RASTERIZER_DESC();
	ZeroMemory(&raster_desc, sizeof(D3D11_RASTERIZER_DESC));

	raster_desc.AntialiasedLineEnable = false;
	raster_desc.CullMode = D3D11_CULL_BACK;
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0.0f;
	raster_desc.DepthClipEnable = false;

	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.FrontCounterClockwise = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	raster_desc.SlopeScaledDepthBias = 0.0f;

	pd3dDevice->CreateRasterizerState(&raster_desc, &raster_state);

	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
	
	blend_desc.RenderTarget[0].BlendEnable = true;

	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pd3dDevice->CreateBlendState(&blend_desc, &blend_state);

	{
		std::string font_name;
		parser.Read("FONT_FILENAME", font_name);

		int font_height = 0;
		parser.Read("FONT_HEIGHT", font_height);

		uint32_t font_size = 0;
		uint8_t* font_buffer = LoadFile(font_name.c_str(), tex_size);
		font.Load(font_buffer, font_height);
	}
}

void* WinDX11Theme::GetRenderDevice()
{
	return pd3dDevice;
}

void WinDX11Theme::LoadImage(const char* name)
{
	if (elems.count(name) > 0)
	{
		return;
	}

	FILE* file = fopen(name, "rb");

	uint8_t* buffer = nullptr;
	int size = 0;

	if (file)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);

		buffer = (uint8_t*)malloc(size + 1);
		fread(buffer, size, 1, file);

		buffer[size] = 0;

		fclose(file);
	}

	int bytes;
	int width;
	int height;
	uint8_t* data = stbi_load_from_memory(buffer, size, &width, &height, &bytes, STBI_rgb_alpha);

	D3D11_BOX dest_box;
	dest_box.left = 20 * image_index;
	dest_box.top = 241;
	dest_box.front = 0;
	dest_box.right = 20 + dest_box.left;
	dest_box.bottom = 255;
	dest_box.back = 1;

	UINT index = D3D11CalcSubresource(0, 0, 1);
	immediateContext->UpdateSubresource(texture, index, &dest_box, data, width * 4, 0);

	Elem& elem = elems[name];

	elem.u = 20.0f * (float)image_index / (float)skin_width;
	elem.v = 241.0f / (float)skin_height;
	elem.du = 20.0f / (float)skin_width;;
	elem.dv = 15.0f / (float)skin_height;

	free(data);
	free(buffer);

	image_index++;
}

void WinDX11Theme::SetOutputWnd(WindowData& data, HWND hwnd, int wgt, int hgt)
{
	if (data.depthStencilView)
	{
		data.depthStencilView->Release();
	}

	if (data.depthStencil)
	{
		data.depthStencil->Release();
	}

	if (data.renderTargetView)
	{
		data.renderTargetView->Release();
	}

	if (data.swapChain)
	{
		data.swapChain->Release();
	}

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = wgt;
	sd.BufferDesc.Height = hgt;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Windowed = TRUE;

	factory->CreateSwapChain(pd3dDevice, &sd, &data.swapChain);

	ID3D11Texture2D* pBackBuffer = NULL;
	HRESULT hr = data.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &data.renderTargetView);
	pBackBuffer->Release();

	if (FAILED(hr))
	{
		return;
	}

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = wgt;
	descDepth.Height = hgt;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pd3dDevice->CreateTexture2D(&descDepth, NULL, &data.depthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = pd3dDevice->CreateDepthStencilView(data.depthStencil, &descDSV, &data.depthStencilView);
}

void WinDX11Theme::Ulnload()
{
}

HCURSOR WinDX11Theme::GetCursor(const char* name)
{
	return cursors[name];
}

void WinDX11Theme::SetClampBorder(int x, int y, int w, int h)
{
	clamp_x = x;
	clamp_y = y;
	clamp_x2 = x + w;
	clamp_y2 = y + h;
}

void WinDX11Theme::SetScreenSize(WindowData& data, int set_scr_width, int set_scr_height)
{
	scr_width = set_scr_width;
	scr_height = set_scr_height;

	immediateContext->OMSetDepthStencilState(nullptr, 255);
	immediateContext->RSSetState(raster_state);
	immediateContext->OMSetBlendState(blend_state, 0, 0xffffffff);
	immediateContext->OMSetRenderTargets(1, &data.renderTargetView, nullptr);

	{
		D3D11_VIEWPORT vp;
		vp.Width = (float)scr_width;
		vp.Height = (float)scr_height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;

		immediateContext->RSSetViewports(1, &vp);
	}

	immediateContext->VSSetShader(vshader, 0, 0);
	immediateContext->PSSetShader(pshader, 0, 0);

	immediateContext->IASetInputLayout(layout);

	immediateContext->PSSetSamplers(0, 1, &sampler);
	immediateContext->PSSetSamplers(1, 1, &sampler);
	immediateContext->PSSetSamplers(2, 1, &sampler);

	immediateContext->PSSetShaderResources(0, 1, &srview);
	immediateContext->PSSetShaderResources(1, 1, &font.srview);

	immediateContext->VSSetShaderResources(0, 1, &sbuffer_srview);

	Start();

	unsigned int stride = 4 * 2;
	unsigned int offset = 0;
	immediateContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void WinDX11Theme::Draw(const char* elem_name, int x, int y, int width, int height)
{
	if (elems.count(elem_name) == 0)
	{
		return;
	}

	Elem& elem = elems[elem_name];

	if (elem.offset_u == -1 && elem.offset_v == -1)
	{
		params.x = (float)x;
		params.y = (float)y;

		params.width = (float)width;
		params.height = (float)height;

		params.u = elem.u;
		params.v = elem.v;

		params.du = elem.du;
		params.dv = elem.dv;

		params.scr_width = (float)scr_width;
		params.scr_height = (float)scr_height;

		params.texture = 0;

		params.r = params.g = params.b = params.a = 1.0f;

		if (ClampRect())
		{
			GetNext();
		}
	}
	else
	if (elem.offset_u != -1 && elem.offset_v == -1)
	{
		float xs[] = { 0, elem.offset_u, (float)width - elem.offset_u, (float)width };
		float ys[] = { 0, (float)height };

		float us[] = { 0, elem.offset_du, elem.du - elem.offset_du, elem.du };
		float vs[] = { 0, elem.dv };

		for (int j = 0; j < 3; j++)
		{
			params.x = (float)x + xs[j];
			params.y = (float)y + ys[0];

			params.width = xs[j + 1] - xs[j];
			params.height = ys[1] - ys[0];

			params.u = (elem.u + us[j]);
			params.v = (elem.v + vs[0]);

			params.du = (us[j + 1] - us[j]);
			params.dv = (vs[1] - vs[0]);

			params.scr_width = (float)scr_width;
			params.scr_height = (float)scr_height;

			params.texture = 0;

			params.r = params.g = params.b = params.a = 1.0f;

			if (ClampRect())
			{
				GetNext();
			}
		}
	}
	else
	if (elem.offset_u != -1 && elem.offset_v != -1)
	{
		float xs[] = { 0, elem.offset_u, (float)width - elem.offset_u, (float)width};
		float ys[] = { 0, elem.offset_v, (float)height - elem.offset_v, (float)height };

		float us[] = { 0, elem.offset_du, elem.du - elem.offset_du, elem.du };
		float vs[] = { 0, elem.offset_dv, elem.dv - elem.offset_dv, elem.dv };

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				params.x = (float)x + xs[j];
				params.y = (float)y + ys[i];

				params.width = xs[j + 1] - xs[j];
				params.height = ys[i + 1] - ys[i];

				params.u = (elem.u + us[j]);
				params.v = (elem.v + vs[i]);

				params.du = (us[j + 1] - us[j]);
				params.dv = (vs[i + 1] - vs[i]);

				params.scr_width = (float)scr_width;
				params.scr_height = (float)scr_height;

				params.texture = 0;

				params.r = params.g = params.b = params.a = 1.0f;

				if (ClampRect())
				{
					GetNext();
				}
			}
		}
	}
}

void WinDX11Theme::Draw(void* texture, float* color, int x, int y, int width, int height, float u, float v, float du, float dv)
{
	params.x = (float)x;
	params.y = (float)y;
	params.width = (float)width;
	params.height = (float)height;

	params.u = u;
	params.v = v;
	params.du = du;
	params.dv = dv;

	params.scr_width = (float)scr_width;
	params.scr_height = (float)scr_height;

	if (color)
	{
		params.r = color[0];
		params.g = color[1];
		params.b = color[2];
		params.a = color[3];
	}
	else
	{
		params.r = params.g = params.b = params.a = 1.0f;
	}

	ID3D11ShaderResourceView* tex_srview = (ID3D11ShaderResourceView*)texture;

	if (!tex_srview)
	{
		params.texture = 3;
	}
	else
	if (font.srview != tex_srview)
	{
		immediateContext->PSSetShaderResources(2, 1, &tex_srview);
		params.texture = 2;
	}
	else
	{
		params.texture = 1;
	}

	if (ClampRect())
	{
		GetNext();
	}
}

void WinDX11Theme::Present(WindowData& data)
{
	immediateContext->Unmap(struct_buffer, 0);

	if (inst_count > 0)
	{
		immediateContext->DrawInstanced(4, inst_count, 0, 0);
	}

	data.swapChain->Present(0, 0);

	ID3D11ShaderResourceView* ps[] = { nullptr, nullptr, nullptr };
	immediateContext->PSSetShaderResources(0, 3, ps);
}

#endif