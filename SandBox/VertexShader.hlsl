

cbuffer ConstantBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    float4 lightDirection;
    float4 lightColor;
    float4 ambientLightColor;
    float ambientLightStrength;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 texcoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    float4 worldPos = mul(float4(input.position, 1.0f), worldMatrix);
    output.worldPos = worldPos.xyz;
    
    output.position = mul(worldPos, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.normal = mul(float4(input.normal, 0.0f), worldMatrix).xyz;

    output.texcoord = input.texcoord;
    
    return output;
}