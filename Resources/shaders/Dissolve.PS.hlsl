#include"PostEffect.hlsli"

struct CBuffData {
	float32_t threshold;
};

ConstantBuffer<CBuffData> gData : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t> gMaskTex : register(t1);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	
	float32_t mask = gMaskTex.Sample(gSampler, input.texcoord);
	
	if(mask < gData.threshold){
		discard;
	}

	PixelShaderOutput output;

	float32_t edge = 1.0f - smoothstep(gData.threshold, gData.threshold + 0.03, mask);
	output.color = gTexture.Sample(gSampler, input.texcoord);

	output.color.rgb += edge * float32_t3(1.0f, 0.4f,0.3f);
	output.color.a = 1.0f;

	return output;
}