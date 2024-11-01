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

Texture2D diffuseTexture: register(t0);
Texture2D normalTexture : register(t1);
Texture2D metallicTexture : register(t2);
Texture2D roughnessTexture : register(t3);
Texture2D aoTexture : register(t4);

SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Sample the diffuse, metallic, roughness, and AO textures
    float3 baseColor = diffuseTexture.Sample(samplerState, input.texcoord).rgb;
    float metalness = metallicTexture.Sample(samplerState, input.texcoord).r;
    float roughness = roughnessTexture.Sample(samplerState, input.texcoord).r;
    float ao = aoTexture.Sample(samplerState, input.texcoord).r; // Ambient occlusion

    // Sample and transform the normal map as before
    float3 tangentNormal = normalTexture.Sample(samplerState, input.texcoord).rgb * 2.0 - 1.0;
    float3 normal = normalize(input.normal + tangentNormal);

    // Ambient lighting with AO applied
    float3 ambientLight = ambientLightColor.rgb * ambientLightStrength * ao;

    // Diffuse lighting
    float3 pos = float3(500, 250,500);
    float3 lightDir = normalize(pos - input.worldPos);
    float diffuseIntensity = saturate(dot(normal, lightDir));
    
    // Adjust diffuse color by metalness
    float3 diffuseColor = lerp(baseColor * lightColor.rgb * diffuseIntensity, lightColor.rgb * diffuseIntensity, metalness);

    // Specular lighting
    float3 viewDir = normalize(-input.worldPos); // Assumes camera at (0,0,0)
    float3 reflectDir = reflect(-lightDir, normal);

    // Calculate specular intensity based on roughness
    float specularFactor = pow(saturate(dot(viewDir, reflectDir)), lerp(128.0, 1.0, roughness));
    float3 specularColor = lerp(float3(0.04, 0.04, 0.04), baseColor, metalness); // Non-metal baseline of 0.04, metals use baseColor

    float3 specular = specularColor * specularFactor * lightColor.rgb;

    // Final color combining diffuse, ambient with AO, and specular
    float3 finalColor = diffuseColor + ambientLight * baseColor + specular;

    return float4(finalColor, 1.0f);
}
