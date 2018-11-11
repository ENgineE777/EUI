
struct VS_INPUT
{
	float2 position : POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	int2   tex : TEXCOORD1;
	float4 color : COLOR0;
};

struct InstParams
{
	float4 pos_size;
	float4 uv;
	float2 screen;
	int2   tex;
	float4 color;
	float4 dumm0;
	float4 dumm1;
	float4 dumm2;
	float4 dumm3;
};

StructuredBuffer<InstParams> inst_params : register(t0);

PS_INPUT VS( VS_INPUT input , in uint ver_id : SV_InstanceID)
{
	float4 posTemp = float4(inst_params[ver_id].pos_size.x + inst_params[ver_id].pos_size.z * input.position.x,
	                        inst_params[ver_id].pos_size.y + inst_params[ver_id].pos_size.w * input.position.y, 0, 1.0f);

	posTemp.x = -1.0f + posTemp.x / inst_params[ver_id].screen.x * 2.0f;
	posTemp.y = 1.0f - posTemp.y / inst_params[ver_id].screen.y * 2.0f;

	PS_INPUT output = (PS_INPUT)0;

	output.Pos = float4(posTemp.x, posTemp.y, 0.5f, 1.0f);
	output.texCoord = float2(inst_params[ver_id].uv.x + inst_params[ver_id].uv.z * input.position.x, (inst_params[ver_id].uv.y + inst_params[ver_id].uv.w * input.position.y));
	output.tex = inst_params[ver_id].tex;
	output.color = inst_params[ver_id].color;

	return output;
}

SamplerState samplear : register(s0);

Texture2D ui_tex : register(t0);
Texture2D font_tex : register(t1);
Texture2D custom_tex : register(t2);

float4 PS( PS_INPUT input) : SV_Target
{
	float4 res = float4(1.0f, 1.0f, 1.0f, 1.0f);

	if (input.tex.x == 0)
	{
		res = ui_tex.Sample(samplear, input.texCoord);
	}
	else
	if (input.tex.x == 1)
	{
		float font = font_tex.Sample(samplear, input.texCoord).r;
		res = float4(1.0f, 1.0f, 1.0f, font);
	}
	else
	if (input.tex.x == 2)
	{
		res = float4(custom_tex.Sample(samplear, input.texCoord).rgb, 1.0f);
	}

	return res * input.color;
}