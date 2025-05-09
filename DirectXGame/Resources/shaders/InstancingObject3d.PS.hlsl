#include"Object3d.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
	float32_t shininess;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct Camera {
	float32_t4x4 view;
	float32_t4x4 projection;
	float32_t3 cameraPos;
};
ConstantBuffer<Camera> gCamera : register(b1);

struct DirectionalLight {
	float32_t4 color;
	float32_t3 direction;
	float intensity;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);


PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	//float4 transformedUV = mul(float32_t4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

	if(gMaterial.enableLighting != 0) {
		float32_t3 toEye = normalize(gCamera.cameraPos - input.worldPosition);

		float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);

		float NdotH = dot(normalize(input.normal), halfVector);
		float specularPow = pow(saturate(NdotH), gMaterial.shininess);

		///Half Lambert
	    float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
	    float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

		float32_t3 diffuseDL = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		float32_t3 specularDL = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f,1.0f,1.0f);

		
		//合計

		//ライト系の色加算
		float32_t3 litColor = diffuseDL + specularDL;

		output.color.rgb = litColor;

        output.color.a = gMaterial.color.a * textureColor.a;
	} else {
	    output.color = gMaterial.color * textureColor;
	}

	if(output.color.a == 0.0){
		discard;
	}

	return output;
}