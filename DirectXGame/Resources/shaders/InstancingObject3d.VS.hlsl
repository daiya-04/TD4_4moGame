#include "InstancingObject3d.hlsli"

struct ObjectGPU {
	float32_t4x4 matWorld;
	float32_t4x4 matWorldInverse;
	float32_t4 color;
};

StructuredBuffer<ObjectGPU> gObject : register(t1);

struct Camera {
	float32_t4x4 view;
	float32_t4x4 projection;
	float32_t3 cameraPos;
};
ConstantBuffer<Camera> gCamera : register(b1);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID) {
	VertexShaderOutput output;
	output.position = mul(input.position, mul(gObject[instanceId].matWorld, mul(gCamera.view, gCamera.projection)));
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float32_t3x3)gObject[instanceId].matWorldInverse));
	output.worldPosition = mul(input.position, gObject[instanceId].matWorld).xyz;
	output.color = gObject[instanceId].color;
	return output;
}