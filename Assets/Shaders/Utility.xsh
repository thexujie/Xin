#ifndef _XIN_SHADER_HEADER_UTILITY_
#define _XIN_SHADER_HEADER_UTILITY_ 1

// Encodes a smooth logarithmic gradient for even distribution of precision natural to vision
float LinearToLogLuminance(float LinearRGB, float Gamma = 4.0)
{
    return log2(lerp(1, exp2(Gamma), LinearRGB)) / Gamma;
}

// This assumes the default color gamut found in sRGB and REC709.  The color primaries determine these
// coefficients.  Note that this operates on linear values, not gamma space.
float RGBToLuminance(float3 LinearRGB)
{
    return dot(LinearRGB, float3(0.212671, 0.715160, 0.072169)); // Defined by sRGB/Rec.709 gamut
}

// This is the same as above, but converts the linear luminance value to a more subjective "perceived luminance",
// which could be called the Log-Luminance.
float RGBToLogLuminance(float3 LinearRGB, float Gamma = 4.0)
{
    return LinearToLogLuminance(RGBToLuminance(LinearRGB), Gamma);
}


// 8-bit should range from 16 to 235
float3 RGBFullToLimited8bit(float3 LinearRGB)
{
    return saturate(LinearRGB) * 219.0 / 255.0 + 16.0 / 255.0;
}

float3 RGBLimitedToFull8bit(float3 LinearRGB)
{
    return saturate((LinearRGB - 16.0 / 255.0) * 255.0 / 219.0);
}

// 10-bit should range from 64 to 940
float3 RGBFullToLimited10bit(float3 LinearRGB)
{
    return saturate(LinearRGB) * 876.0 / 1023.0 + 64.0 / 1023.0;
}

float3 RGBLimitedToFull10bit(float3 LinearRGB)
{
    return saturate((LinearRGB - 64.0 / 1023.0) * 1023.0 / 876.0);
}

// The standard 32-bit HDR color format.  Each float has a 5-bit exponent and no sign bit.
uint Pack_R11G11B10_FLOAT(float3 RGB)
{
    // Clamp upper bound so that it doesn't accidentally round up to INF 
    // Exponent=15, Mantissa=1.11111
    RGB = min(RGB, asfloat(0x477C0000));
    uint r = ((f32tof16(RGB.x) + 8) >> 4) & 0x000007FF;
    uint g = ((f32tof16(RGB.y) + 8) << 7) & 0x003FF800;
    uint b = ((f32tof16(RGB.z) + 16) << 17) & 0xFFC00000;
    return r | g | b;
}

float3 Unpack_R11G11B10_FLOAT(uint RGB)
{
    float r = f16tof32((RGB << 4) & 0x7FF0);
    float g = f16tof32((RGB >> 7) & 0x7FF0);
    float b = f16tof32((RGB >> 17) & 0x7FE0);
    return float3(r, g, b);
}

// The Reinhard tone operator.  Typically, the value of k is 1.0, but you can adjust exposure by 1/k.
// I.e. TM_Reinhard(x, 0.5) == TM_Reinhard(x * 2.0, 1.0)
float3 TM_Reinhard(float3 HDRColor, float k = 1.0)
{
    return HDRColor / (HDRColor + k);
}

// The inverse of Reinhard
float3 ITM_Reinhard(float3 SDRColor, float k = 1.0)
{
    return k * SDRColor / (k - SDRColor);
}

//
// Reinhard-Squared
//

// This has some nice properties that improve on basic Reinhard.  Firstly, it has a "toe"--that nice,
// parabolic upswing that enhances contrast and color saturation in darks.  Secondly, it has a long
// shoulder giving greater detail in highlights and taking longer to desaturate.  It's invertible, scales
// to HDR displays, and is easy to control.
//
// The default constant of 0.25 was chosen for two reasons.  It maps closely to the effect of Reinhard
// with a constant of 1.0.  And with a constant of 0.25, there is an inflection point at 0.25 where the
// curve touches the line y=x and then begins the shoulder.
//
// Note:  If you are currently using ACES and you pre-scale by 0.6, then k=0.30 looks nice as an alternative
// without any other adjustments.

float3 TM_ReinhardSq(float3 HDRColor, float k = 0.25)
{
    float3 reinhard = HDRColor / (HDRColor + k);
    return reinhard * reinhard;
}

float3 ITM_ReinhardSq(float3 SDRColor, float k = 0.25)
{
    return k * (SDRColor + sqrt(SDRColor)) / (1.0 - SDRColor);
}

//
// Stanard (New)
//

// This is the new tone operator.  It resembles ACES in many ways, but it is simpler to evaluate with ALU.  One
// advantage it has over Reinhard-Squared is that the shoulder goes to white more quickly and gives more overall
// brightness and contrast to the image.

float3 TM_Stanard(float3 HDRColor)
{
    return TM_Reinhard(HDRColor * sqrt(HDRColor), sqrt(4.0 / 27.0));
}

float3 ITM_Stanard(float3 SDRColor)
{
    return pow(ITM_Reinhard(SDRColor, sqrt(4.0 / 27.0)), 2.0 / 3.0);
}

float3 TM_ACES(float3 HDRColor)
{
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float s = 0.14;
    return (HDRColor * (a * HDRColor + b)) / (HDRColor * (c * HDRColor + d) + s);
}

float3 ApplySRGBCurve(float3 RGB)
{
    // Approximately pow(x, 1.0 / 2.2)
#if XIN_SPIRV
    return select(RGB < 0.0031308, 12.92 * RGB, 1.055 * pow(RGB, 1.0 / 2.4) - 0.055);
#else
    return RGB < 0.0031308 ? 12.92 * RGB : 1.055 * pow(RGB, 1.0 / 2.4) - 0.055;
#endif
}

#endif // _XIN_SHADER_HEADER_UTILITY_
