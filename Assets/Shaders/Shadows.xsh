#ifndef _XIN_SHADER_SHADOWS_H_
#define _XIN_SHADER_SHADOWS_H_ 1

#include "Common.xsh"

#ifndef XIN_CSM_CASCADE_MAX
#define XIN_CSM_CASCADE_MAX 4
#endif

#ifndef XIN_CSM_CASCADE_COUNT
#define XIN_CSM_CASCADE_COUNT 3
#endif

//--------------------------------------------------------------------------------------
// This function calculates the screen space depth for shadow space texels
//--------------------------------------------------------------------------------------
void CalculateRightAndUpTexelDepthDeltas(float3 ShadowCoordDDX, float3 ShadowCoordDDY, float ShadowTexelSize, 
    out float2 TexelDepthDelta)
{
    // We use the derivatives in X and Y to create a transformation matrix.  Because these derivives give us the 
    // transformation from screen space to shadow space, we need the inverse matrix to take us from shadow space 
    // to screen space.  This new matrix will allow us to map shadow map texels to screen space.  This will allow 
    // us to find the screen space depth of a corresponding depth pixel.
    // This is not a perfect solution as it assumes the underlying geometry of the scene is a plane.  A more 
    // accureate way of finding the actual depth would be to do a deferred rendering approach and actually 
    //sample the depth.
    
    // Using an offset, or using variance shadow maps is a better approach to reducing these artifacts in most cases.
    
    float2x2 ScreenToShadowMatrix = float2x2(ShadowCoordDDX.xy, ShadowCoordDDY.xy);
    float Determinant = determinant(ScreenToShadowMatrix);
    if (Determinant < 0.00001f)
        return;

    float InvDeterminant = 1.0f / Determinant;
    
    float2x2 ShadowToScreenMatrix = float2x2(
        ScreenToShadowMatrix._22 * InvDeterminant, ScreenToShadowMatrix._12 * -InvDeterminant,
        ScreenToShadowMatrix._21 * -InvDeterminant, ScreenToShadowMatrix._11 * InvDeterminant);

    // Transform the right pixel by the shadow space to screen space matrix.
    float2 RightTexelDepthRatio = mul(float2(ShadowTexelSize, 0.0f), ShadowToScreenMatrix);
    float2 UpTexelDepthRatio = mul(float2(0.0f, ShadowTexelSize), ShadowToScreenMatrix);

    // We can now caculate how much depth changes when you move up or right in the shadow map.
    // We use the ratio of change in x and y times the dervivite in X and Y of the screen space 
    // depth to calculate this change.
    TexelDepthDelta.x = RightTexelDepthRatio.x * ShadowCoordDDX.z + RightTexelDepthRatio.y * ShadowCoordDDY.z;
    TexelDepthDelta.y = UpTexelDepthRatio.x * ShadowCoordDDX.z + UpTexelDepthRatio.y * ShadowCoordDDY.z;
}

#endif // _XIN_SHADER_SHADOWS_H_
