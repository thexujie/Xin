#ifndef _XIN_SHADER_MATERIAL_FALLBACK_H_
#define _XIN_SHADER_MATERIAL_FALLBACK_H_ 1

#ifndef XIN_MATERIAL

float3 GetMaterialDiffuse(float2 Texcoord)
{
    return float3(0.0f, 0.0f, 0.0f);
}

float3 GetMaterialNormal(float3 InputNormal, float2 Texcoord : TEXCOORD)
{
    return InputNormal;
}

float GetMaterialSpecular(float InputSpecular, float2 Texcoord : TEXCOORD)
{
    return InputSpecular;
}

float GetMaterialAO(float4 ScreenPosition)
{
    return 1.0f;
}

void MaterialClipping(float2 Texcoord)
{
	
}
#endif // XIN_MATERIAL

#endif // _XIN_SHADER_MATERIAL_FALLBACK_H_
