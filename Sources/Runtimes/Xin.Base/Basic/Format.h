#pragma once

#include "Basic.Types.h"
#include "Vec3.h"
#include "Vec4.h"

namespace Xin
{
    forceinline constexpr unsigned int MakeFormatValue_RGB(unsigned short index)
    {
        return (static_cast<unsigned int>(index)) | 0x7eee0000;
    }

    enum class EColorSpace
    {
	    sRGB = 0,
        LinearsRGB,

        DisplayP3,
        LinearDisplayP3,

        BT709,
        LinearBT709,

        BT2020,

        HDR10ST2084,
        HDR10HLG,

        DolbyVision,

        AdobeRGB,
        LinearAdobeRGB
    };

    // Low bits --> High bits
    enum class EFormat
    {
        None = 0,

        R8G8B8A8,
        R8G8B8A8UF,
        R8G8B8A8SF,
        R8G8B8A8UFsRGB,
        R8G8B8A8UI,
        R8G8B8A8SI,
        R8G8B8X8,

        B8G8R8A8,
        B8G8R8A8UF,
        B8G8R8A8UFsRGB,
        B8G8R8A8SF,

        B8G8R8X8,
        B8G8R8X8UF,
        B8G8R8X8UFsRGB,

        R10G10B10A2,
        R10G10B10A2UF,
        R10G10B10A2UI,
        R11G11B10F,

        R16G16,
        R16G16F,
        R16G16UF,
        R16G16SF,
        R16G16UI,
        R16G16SI,

        R16G16B16A16,
        R16G16B16A16F,
        R16G16B16A16UF,
        R16G16B16A16SF,
        R16G16B16A16UI,
        R16G16B16A16SI,

        R32G32,
        R32G32F,
        R32G32UI,
        R32G32SI,

        R32G32B32,
        R32G32B32F,
        R32G32B32UI,
        R32G32B32SI,

        R32G32B32A32,
        R32G32B32A32F,
        R32G32B32A32UF,
        R32G32B32A32SF,
        R32G32B32A32UI,
        R32G32B32A32SI,

        A8,
        A8UF,

        R8,
        R8UI,
        R8SI,
        R8UF,
        R8SF,

        R16,
        R16F,
        R16UI,
        R16SI,
        R16UF,
        R16SF,

        R24G8,
        R24X8,

        R32,
        R32F,
        R32UI,
        R32SI,
        //R32UF, // 32 bit float point, doesn't need normalized float point.
        //R32SF,

    	// DepthStencil
        D16UF,
        D32F,
        D24UF_S8UI,

        // Special Formats
        //---------------------------------8 Bits
        R3G3B2,

        //---------------------------------16 Bits
        R8G8,
        R8G8UF,
        R8G8SF,
        R8G8UI,
        R8G8SI,

        R5G6B5,
        R5G6B5UF,
        R5G5B5A1,
        R5G5B5A1UF,

        B5G6R5,
        B5G5R5A1,
        A1R5G5B5,
        A1B5G5R5,
        X1R5G5B5,
        X1B5G5R5,
        A4R4G4B4,
        X4R4G4B4,
        A8R3G3B2,
        B4G4R4A4,

        //---------------------------------24 Bits
        R8G8B8,
        B8G8R8,

        A8R5G6B5,
        A8B5G6R5,
        A8X1R5G5B5,
        A8X1B5G5R5,

        A2R10G10B10,
        A2B10G10R10,

        //---------------------------------Block Compression
        BC1,
        BC1UF,
        BC1UFsRGB,
        BC1SF,

        BC2,
        BC2UF,
        BC2UFsRGB,

        BC3,
        BC3UF,
        BC3UFsRGB,

        BC4,
        BC4UF,
        BC4SF,

        BC5,
        BC5UF,
        BC5SF,

        BC6H,
        BC6H_UF16,
        BC6H_SF16,

        BC7,
        BC7UF,
        BC7UFsRGB,

        //---------------------------------Indexed
        INDEX4_A8R8G8B8,
        INDEX8_A8R8G8B8,

        // Alias
        RGB = R8G8B8,
        BGR = B8G8R8,

        RGBA = R8G8B8A8,
        RGBAUF = R8G8B8A8UF,
        RGBASF = R8G8B8A8SF,
        RGBAUFsRGB = R8G8B8A8UFsRGB,
        RGBAUI = R8G8B8A8UI,
        RGBASI = R8G8B8A8SI,
        RGBX = R8G8B8X8,

        BGRA = B8G8R8A8,
        BGRAUF = B8G8R8A8UF,
        BGRAUFsRGB = B8G8R8A8UFsRGB,
        BGRASF = B8G8R8A8SF,
        BGRX = B8G8R8X8,
        BGRXUFsRGB = B8G8R8X8UFsRGB,

		//---------------------------------ASTC
		ASTC4x4,
		ASTC4x4sRGB,
		ASTC5x4,
		ASTC5x4sRGB,
        ASTC5x5,
        ASTC5x5sRGB,
        ASTC6x5,
        ASTC6x5sRGB,
        ASTC6x6,
        ASTC6x6sRGB,
        ASTC8x5,
        ASTC8x5sRGB,
        ASTC8x6,
        ASTC8x6sRGB,

        // Common FourCC Formats
        Short1 = MakeFourCC("00S1"),
        Short2 = MakeFourCC("00S2"),
        Short3 = MakeFourCC("00S3"),
        Short4 = MakeFourCC("00S4"),
        UShort1 = MakeFourCC("0US1"),
        UShort2 = MakeFourCC("0US2"),
        UShort3 = MakeFourCC("0US3"),
        UShort4 = MakeFourCC("0US4"),
        Int1 = MakeFourCC("00I1"),
        Int2 = MakeFourCC("00I2"),
        Int3 = MakeFourCC("00I3"),
        Int4 = MakeFourCC("00I4"),
        UInt1 = MakeFourCC("0UI1"),
        UInt2 = MakeFourCC("0UI2"),
        UInt3 = MakeFourCC("0UI3"),
        UInt4 = MakeFourCC("0UI4"),
        Half1 = MakeFourCC("00H1"),
        Half2 = MakeFourCC("00H2"),
        Half3 = MakeFourCC("00H3"),
        Half4 = MakeFourCC("00H4"),
        Float1 = MakeFourCC("00F1"),
        Float2 = MakeFourCC("00F2"),
        Float3 = MakeFourCC("00F3"),
        Float4 = MakeFourCC("00F4"),
        Struct = MakeFourCC("STRU"),

        // Simple RGB Formats
        GRAY1 = MakeFormatValue_RGB(0),
        GRAY2,
        GRAY4,
        GRAY8,

        // FourCC Formats
        I420 = MakeFourCC("I420"),
        I422 = MakeFourCC("I422"),
        I444 = MakeFourCC("I444"),
        NV11 = MakeFourCC("NV11"),
        NV12 = MakeFourCC("NV12"),
        NV21 = MakeFourCC("NV21"),

        YU12 = MakeFourCC("YU12"),
        YUV2 = MakeFourCC("YUV2"),
        YUY2 = MakeFourCC("YUY2"),
        YUYV = MakeFourCC("YUYV"),
        YV12 = MakeFourCC("YV12"),
        YVYU = MakeFourCC("YVYU"),
        UYUV = MakeFourCC("UYUV"),
        UYVY = MakeFourCC("UYVY"),
        VYVU = MakeFourCC("VYVU"),
        VUVY = MakeFourCC("VUVY"),

        CMYK = MakeFourCC("CMYK"),
        YCCK = MakeFourCC("YCCK"),

        Y420 = MakeFourCC("Y420"),

        P010 = MakeFourCC("P010"),
        P016 = MakeFourCC("P016"),

        FOURCC_Y8 = MakeFourCC("Y8  "),
        FOURCC_RGB = MakeFourCC("24RG"),
        FOURCC_BGR = MakeFourCC("24BG"),
        FOURCC_ARGB = MakeFourCC("ARGB"),
        FOURCC_ABGR = MakeFourCC("ABGR"),
        FOURCC_BGRA = MakeFourCC("BGRA"),
        FOURCC_RGBA = MakeFourCC("RGBA"),
        FOURCC_XRGB = MakeFourCC("XRGB"),
        FOURCC_XBGR = MakeFourCC("XBGR"),
        FOURCC_BGRX = MakeFourCC("BGRX"),
        FOURCC_RGBX = MakeFourCC("RGBX"),


        FOURCC_I420 = MakeFourCC("I420"),
        FOURCC_I422 = MakeFourCC("I422"),
        FOURCC_I444 = MakeFourCC("I444"),
        FOURCC_I400 = MakeFourCC("I400"),
        FOURCC_NV21 = MakeFourCC("NV21"),
        FOURCC_NV12 = MakeFourCC("NV12"),
        FOURCC_YUY2 = MakeFourCC("YUY2"),
        FOURCC_UYVY = MakeFourCC("UYVY"),
        FOURCC_M420 = MakeFourCC("M420"),
        FOURCC_AR30 = MakeFourCC("AR30"), // 10 BIT PER CHANNEL. 2101010.
        FOURCC_AB30 = MakeFourCC("AB30"), // ABGR VERSION OF 10 BIT
        FOURCC_RGBP = MakeFourCC("RGBP"), // RGB565 LE.
        FOURCC_RGB0 = MakeFourCC("RGB0"), // ARGB1555 LE.
        FOURCC_R444 = MakeFourCC("R444"), // ARGB4444 LE.
        FOURCC_MJPG = MakeFourCC("MJPG"),
        FOURCC_YV12 = MakeFourCC("YV12"),
        FOURCC_YV16 = MakeFourCC("YV16"),
        FOURCC_YV24 = MakeFourCC("YV24"),
        FOURCC_YU12 = MakeFourCC("YU12"),
        FOURCC_YU16 = MakeFourCC("YU16"), // ALIAS FOR I422.
        FOURCC_YU24 = MakeFourCC("YU24"), // ALIAS FOR I444.
        FOURCC_J420 = MakeFourCC("J420"),
        FOURCC_J400 = MakeFourCC("J400"),
        FOURCC_H420 = MakeFourCC("H420"),
        FOURCC_H422 = MakeFourCC("H422"),
        FOURCC_IYUV = MakeFourCC("IYUV"), // ALIAS FOR I420.
        FOURCC_YUYV = MakeFourCC("YUYV"), // ALIAS FOR YUY2.
        FOURCC_YUVS = MakeFourCC("YUVS"), // ALIAS FOR YUY2 ON MAC.
        FOURCC_HDYC = MakeFourCC("HDYV"), // ALIAS FOR UYVY.
        FOURCC_JPEG = MakeFourCC("JPEG"), // ALIAS FOR JPEG.
        FOURCC_DMB1 = MakeFourCC("DMB1"), // ALIAS FOR MJPG ON MAC.
        FOURCC_BA81 = MakeFourCC("BA81"), // ALIAS FOR BGGR.
        FOURCC_BGB3 = MakeFourCC("BGB3"), // ALIAS FOR RAW.
        FOURCC_BGR3 = MakeFourCC("BGR3"), // ALIAS FOR 24BG.
        FOURCC_CM32 = MakeFourCC(0, 0, 0, 32), // ALIAS FOR BGRA KCMPIXELFORMAT_32ARGB
        FOURCC_CM24 = MakeFourCC(0, 0, 0, 24), // ALIAS FOR RAW KCMPIXELFORMAT_24RGB
        FOURCC_L555 = MakeFourCC("L555"), // ALIAS FOR RGBO.
        FOURCC_L565 = MakeFourCC("L565"), // ALIAS FOR RGBP.
        FOURCC_L551 = MakeFourCC("L551"), // ALIAS FOR RGB0.

        PNG = MakeFourCC("PNG "),
        JPEG = MakeFourCC("JPEG"),
        MJPG = MakeFourCC("MJPG"),
        H264 = MakeFourCC("H264"),
        H265 = MakeFourCC("H265"),
        HEVC = MakeFourCC("HEVC"),
        X264 = MakeFourCC("X264"),
        RAW = MakeFourCC("RAW "),
    };

    inline EFormat AddFormatsRGB(EFormat Format)
    {
	    switch (Format)
	    {
        default:
            return Format;

        case EFormat::R8G8B8A8:
        case EFormat::R8G8B8A8UF:
        case EFormat::R8G8B8A8SF:
        case EFormat::R8G8B8A8UI:
        case EFormat::R8G8B8A8SI:
	    case EFormat::R8G8B8X8:
            return EFormat::R8G8B8A8UFsRGB;

        case EFormat::B8G8R8A8:
        case EFormat::B8G8R8A8UF:
        case EFormat::B8G8R8A8SF:
            return EFormat::B8G8R8A8UFsRGB;

        case EFormat::B8G8R8X8:
        case EFormat::B8G8R8X8UF:
            return EFormat::B8G8R8X8UFsRGB;

        case EFormat::BC1:
        case EFormat::BC1UF:
        case EFormat::BC1SF:
            return EFormat::BC1UFsRGB;

        case EFormat::BC2:
        case EFormat::BC2UF:
            return EFormat::BC2UFsRGB;

        case EFormat::BC3:
        case EFormat::BC3UF:
            return EFormat::BC3UFsRGB;

        case EFormat::BC7:
        case EFormat::BC7UF:
            return EFormat::BC7UFsRGB;
	    }
    }

    inline EFormat DelFormatsRGB(EFormat Format)
    {
        switch (Format)
        {
        default:
            return Format;

        case EFormat::R8G8B8A8UFsRGB:
            return EFormat::R8G8B8A8UF;

        case EFormat::B8G8R8A8UFsRGB:
            return EFormat::B8G8R8A8UF;

        case EFormat::B8G8R8X8UFsRGB:
            return EFormat::B8G8R8X8UF;

        case EFormat::BC1UFsRGB:
            return EFormat::BC1UF;

        case EFormat::BC2UFsRGB:
            return EFormat::BC2UF;

        case EFormat::BC3UFsRGB:
            return EFormat::BC3UF;

        case EFormat::BC7UFsRGB:
            return EFormat::BC7UF;
        }
    }

    inline UByte4 GetPixelFormatComponents(EFormat PixelFormat)
    {
        switch (PixelFormat)
        {
		case EFormat::R8G8B8A8:
		case EFormat::R8G8B8A8UF:
		case EFormat::R8G8B8A8SF:
		case EFormat::R8G8B8A8UFsRGB:
		case EFormat::R8G8B8A8UI:
		case EFormat::R8G8B8A8SI:
		case EFormat::B8G8R8A8:
		case EFormat::B8G8R8A8UF:
		case EFormat::B8G8R8A8UFsRGB:
        case EFormat::B8G8R8A8SF:
        case EFormat::R8G8B8X8:
            return { 1, 1, 1, 1 };
		case EFormat::B8G8R8X8:
		case EFormat::B8G8R8X8UF:
		case EFormat::B8G8R8X8UFsRGB:
            return { 1, 1, 1, 0 };
		case EFormat::R10G10B10A2:
		case EFormat::R10G10B10A2UF:
		case EFormat::R10G10B10A2UI:
            return { 1, 1, 1, 1 };
		case EFormat::R11G11B10F:
            return { 1, 1, 1, 0 };
		case EFormat::R16G16:
		case EFormat::R16G16F:
		case EFormat::R16G16UF:
		case EFormat::R16G16SF:
		case EFormat::R16G16UI:
		case EFormat::R16G16SI:
            return { 1, 1, 0, 0 };
		case EFormat::R16G16B16A16:
		case EFormat::R16G16B16A16F:
		case EFormat::R16G16B16A16UF:
		case EFormat::R16G16B16A16SF:
		case EFormat::R16G16B16A16UI:
		case EFormat::R16G16B16A16SI:
            return { 1, 1, 1, 1 };
		case EFormat::R32G32:
		case EFormat::R32G32F:
		case EFormat::R32G32UI:
		case EFormat::R32G32SI:
            return { 1, 1, 0, 0 };
		case EFormat::R32G32B32:
		case EFormat::R32G32B32F:
		case EFormat::R32G32B32UI:
		case EFormat::R32G32B32SI:
            return { 1, 1, 1, 0 };
		case EFormat::R32G32B32A32:
		case EFormat::R32G32B32A32F:
		case EFormat::R32G32B32A32UF:
		case EFormat::R32G32B32A32SF:
		case EFormat::R32G32B32A32UI:
		case EFormat::R32G32B32A32SI:
            return { 1, 1, 1, 1 };
		case EFormat::A8:
		case EFormat::A8UF:
            return { 0, 0, 0, 1 };
		case EFormat::R8:
		case EFormat::R8UI:
		case EFormat::R8SI:
		case EFormat::R8UF:
		case EFormat::R8SF:
		case EFormat::R16:
		case EFormat::R16F:
		case EFormat::R16UI:
		case EFormat::R16SI:
		case EFormat::R16UF:
		case EFormat::R16SF:
		case EFormat::R24G8:
		case EFormat::R24X8:
		case EFormat::R32:
		case EFormat::R32F:
		case EFormat::R32UI:
		case EFormat::R32SI:
            return { 1, 0, 0, 0 };

        	// https://learn.microsoft.com/zh-cn/windows/win32/direct3d11/texture-block-compression-in-direct3d-11
		case EFormat::BC1:
		case EFormat::BC1UF:
		case EFormat::BC1UFsRGB:
		case EFormat::BC1SF:
            return { 1, 1, 1, 1 };

		case EFormat::BC2:
		case EFormat::BC2UF:
		case EFormat::BC2UFsRGB:
            return { 1, 1, 1, 1 };
		case EFormat::BC3:
		case EFormat::BC3UF:
		case EFormat::BC3UFsRGB:
            return { 1, 1, 1, 1 };
		case EFormat::BC4:
		case EFormat::BC4UF:
		case EFormat::BC4SF:
            return { 1, 0, 0, 0 };
		case EFormat::BC5:
		case EFormat::BC5UF:
		case EFormat::BC5SF:
            return { 1, 1, 0, 0 };
		case EFormat::BC6H:
		case EFormat::BC6H_UF16:
		case EFormat::BC6H_SF16:
            return { 1, 1, 1, 0 };
		case EFormat::BC7:
		case EFormat::BC7UF:
		case EFormat::BC7UFsRGB:
            return { 1, 1, 1, 1 };
		case EFormat::R3G3B2:
            return { 1, 1, 1, 0 };
		case EFormat::R8G8:
		case EFormat::R8G8UF:
		case EFormat::R8G8SF:
		case EFormat::R8G8UI:
		case EFormat::R8G8SI:
            return { 1, 1, 0, 0 };
		case EFormat::R5G6B5:
		case EFormat::R5G6B5UF:
            return { 1, 1, 1, 0 };
		case EFormat::R5G5B5A1:
		case EFormat::R5G5B5A1UF:
            return { 1, 1, 1, 1 };
		case EFormat::B5G6R5:
            return { 1, 1, 1, 0 };
		case EFormat::B5G5R5A1:
		case EFormat::A1R5G5B5:
		case EFormat::A1B5G5R5:
            return { 1, 1, 1, 1 };
		case EFormat::X1R5G5B5:
		case EFormat::X1B5G5R5:
            return { 1, 1, 1, 0 };
		case EFormat::A4R4G4B4:
            return { 1, 1, 1, 1 };
		case EFormat::X4R4G4B4:
            return { 1, 1, 1, 0 };
		case EFormat::A8R3G3B2:
            return { 1, 1, 1, 1 };
		case EFormat::B4G4R4A4:
            return { 1, 1, 1, 1 };
        case EFormat::R8G8B8:
        case EFormat::B8G8R8:
            return { 1, 1, 1, 0 };

        case EFormat::A8R5G6B5:
		case EFormat::A8B5G6R5:
		case EFormat::A8X1R5G5B5:
		case EFormat::A8X1B5G5R5:
            return { 1, 1, 1, 1 };
		case EFormat::A2R10G10B10:
		case EFormat::A2B10G10R10:
            return { 1, 1, 1, 1 };
		case EFormat::INDEX4_A8R8G8B8:
		case EFormat::INDEX8_A8R8G8B8:
            return { 1, 1, 1, 1 };
        default: 
            return /*UByte4::Zero*/{};
		}
    }

    inline Vec4U Get32BitsFormatComponentMasks(EFormat PixelFormat)
    {
	    switch(PixelFormat)
	    {
		case EFormat::None:
		case EFormat::R8G8B8A8:
		case EFormat::R8G8B8A8UF:
		case EFormat::R8G8B8A8SF:
		case EFormat::R8G8B8A8UFsRGB:
		case EFormat::R8G8B8A8UI:
        case EFormat::R8G8B8A8SI:
        case EFormat::R8G8B8X8:
            return { 0xFF, 0xFF00u, 0xFF0000u, 0x00000000u };
		case EFormat::B8G8R8A8:
		case EFormat::B8G8R8A8UF:
		case EFormat::B8G8R8A8UFsRGB:
		case EFormat::B8G8R8A8SF:
            return { 0xFF000000u, 0xFF0000u, 0xFF00u, 0xFF };
		case EFormat::B8G8R8X8:
		case EFormat::B8G8R8X8UF:
		case EFormat::B8G8R8X8UFsRGB:
            return { 0xFF000000u, 0xFF0000u, 0xFF00u, 0x00000000u };

		case EFormat::R10G10B10A2:
		case EFormat::R10G10B10A2UF:
		case EFormat::R10G10B10A2UI:
            return { 0x3FFu, 0x3FFu << 10, 0x3FFu << 20, 0b11u << 30};

		case EFormat::R11G11B10F:
            return { 0x7FFu, 0x7FFu << 11, 0x3FFu << 22, 0 };
		case EFormat::R16G16:
		case EFormat::R16G16F:
		case EFormat::R16G16UF:
		case EFormat::R16G16SF:
		case EFormat::R16G16UI:
		case EFormat::R16G16SI:
            return { 0x0000FFFFu, 0xFFFF0000u, 0, 0 };
		//case EFormat::R16G16B16A16:
		//case EFormat::R16G16B16A16F:
		//case EFormat::R16G16B16A16UF:
		//case EFormat::R16G16B16A16SF:
		//case EFormat::R16G16B16A16UI:
		//case EFormat::R16G16B16A16SI:
		//case EFormat::R32G32:
		//case EFormat::R32G32F:
		//case EFormat::R32G32UI:
		//case EFormat::R32G32SI:
  //          return { 0x0000FFFFu, 0xFFFF0000u, 0, 0 };
		//case EFormat::R32G32B32:
		//case EFormat::R32G32B32F:
		//case EFormat::R32G32B32UI:
		//case EFormat::R32G32B32SI:
  //          return { 0x000000FFu, 0x0000FF00u, 0x00FF0000u, 0 };

		//case EFormat::R32G32B32A32:
		//case EFormat::R32G32B32A32F:
		//case EFormat::R32G32B32A32UF:
		//case EFormat::R32G32B32A32SF:
		//case EFormat::R32G32B32A32UI:
		//case EFormat::R32G32B32A32SI:
  //          return { 0x000000FFu, 0x0000FF00u, 0x00FF0000u, 0xFF000000u };
		case EFormat::A8:
		case EFormat::A8UF:
            return { 0, 0, 0, 0x000000FFu };
		case EFormat::R8:
		case EFormat::R8UI:
		case EFormat::R8SI:
		case EFormat::R8UF:
		case EFormat::R8SF:
            return { 0x000000FFu, 0, 0, 0 };
		case EFormat::R16:
		case EFormat::R16F:
		case EFormat::R16UI:
		case EFormat::R16SI:
		case EFormat::R16UF:
		case EFormat::R16SF:
            return { 0x000000FFu, 0, 0, 0 };
		case EFormat::R24G8:
            return { 0x00FFFFFFu, 0xFF000000u, 0, 0 };
		case EFormat::R24X8:
            return { 0x00FFFFFFu, 0, 0, 0 };
		case EFormat::R32:
		case EFormat::R32F:
		case EFormat::R32UI:
		case EFormat::R32SI:
            return { 0xFFFFFFFFu, 0, 0, 0 };
		case EFormat::R3G3B2:
            return { 0b111u, 0b111u << 2, 0b11 << 6, 0 };
		case EFormat::R8G8:
		case EFormat::R8G8UF:
		case EFormat::R8G8SF:
		case EFormat::R8G8UI:
		case EFormat::R8G8SI:
            return { 0x000000FFu, 0x0000FF00u, 0, 0 };
		case EFormat::R5G6B5:
		case EFormat::R5G6B5UF:
            return { 0b11111u, 0b111111u << 5, 0b11111u << 11, 0 };
		case EFormat::R5G5B5A1:
		case EFormat::R5G5B5A1UF:
            return { 0b11111u, 0b11111u << 5, 0b11111u << 10, 0b1 << 15 };
		case EFormat::B5G6R5:
            return { 0b11111u << 11, 0b111111u << 5, 0b11111u, 0 };
		case EFormat::B5G5R5A1:
            return { 0b11111u << 10, 0b111111u << 5, 0b11111u, 0b1 << 15 };
		case EFormat::A1R5G5B5:
            return { 0b11111u << 1, 0b111111u << 6, 0b11111u << 11, 0b1 };
		case EFormat::A1B5G5R5:
            return { 0b11111u << 11, 0b111111u << 6, 0b11111u << 1, 0b1 };
		case EFormat::X1R5G5B5:
            return { 0b11111u << 1, 0b111111u << 6, 0b11111u << 11, 0 };
		case EFormat::X1B5G5R5:
            return { 0b11111u << 11, 0b111111u << 6, 0b11111u << 1, 0 };
		case EFormat::A4R4G4B4:
            return { 0b1111u << 12, 0b1111u << 8, 0b1111u << 4, 0b1111u };
		case EFormat::X4R4G4B4:
            return { 0b1111u << 12, 0b1111u << 8, 0b1111u << 4, 0 };
		case EFormat::A8R3G3B2:
            return { 0b111u << 8, 0b111u << 11, 0b11u << 14, 0xF };
		case EFormat::B4G4R4A4:
            return { 0b1111u << 8, 0b1111u << 4, 0b1111u, 0b1111u << 12 };
        case EFormat::R8G8B8:
            return { 0xFF, 0xFF00, 0xFF0000, 0 };
        case EFormat::B8G8R8:
            return { 0xFF0000, 0xFF00, 0xFF, 0 };
		case EFormat::A8R5G6B5:
            return { 0b11111u << 8, 0b111111u << 13, 0b11111u << 19, 0xF };
		case EFormat::A8B5G6R5:
            return { 0b11111u << 19, 0b111111u << 13, 0b11111u << 8, 0xF };
		case EFormat::A8X1R5G5B5:
            return { 0b11111u << 9, 0b11111u << 14, 0b11111u << 19, 0xF };
		case EFormat::A8X1B5G5R5:
            return { 0b11111u << 19, 0b11111u << 14, 0b11111u << 9, 0xF };
		case EFormat::A2R10G10B10:
            return { 0x3FFu << 2, 0x3FFu << 12, 0x3FFu << 22, 0xb11 };
		case EFormat::A2B10G10R10:
            return { 0x3FFu << 22, 0x3FFu << 12, 0x3FFu << 2, 0xb11 };
		case EFormat::INDEX4_A8R8G8B8:
		case EFormat::INDEX8_A8R8G8B8:
            return { 0xFF00u, 0xFF0000u, 0xFF000000u, 0xFF };
		default:
            return Vec4U::Zero;
		}
    }

    BASE_API EFormat FormatFromString(TView<char8_t> String);

    BASE_API TView<char8_t> FormatToString(EFormat Format);

    struct FFormatInfo
    {
        uint32 BlockWidth;
        uint32 BlockHeight;
        uint32 BlockDepth;
        uint32 BlockBits;
    };

    BASE_API FFormatInfo GetFormatInfo(EFormat Format);
    constexpr uint32 GetFormatBitsPerPixel(EFormat Format)
    {
        switch (Format)
        {
        case EFormat::R32G32B32A32:
        case EFormat::R32G32B32A32F:
        case EFormat::R32G32B32A32UF:
        case EFormat::R32G32B32A32SF:
        case EFormat::R32G32B32A32UI:
        case EFormat::R32G32B32A32SI:
            return 128;

        case EFormat::R32G32B32:
        case EFormat::R32G32B32F:
        case EFormat::R32G32B32UI:
        case EFormat::R32G32B32SI:
            return 96;

        case EFormat::R16G16B16A16:
        case EFormat::R16G16B16A16F:
        case EFormat::R16G16B16A16UF:
        case EFormat::R16G16B16A16SF:
        case EFormat::R16G16B16A16UI:
        case EFormat::R16G16B16A16SI:
        case EFormat::R32G32:
        case EFormat::R32G32F:
        case EFormat::R32G32UI:
        case EFormat::R32G32SI:
            //case DXGI_FORMAT_R32G8X24_TYPELESS:
            //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
            //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
            //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
            //case DXGI_FORMAT_Y416:
            //case DXGI_FORMAT_Y210:
            //case DXGI_FORMAT_Y216:
            return 64;

        case EFormat::R10G10B10A2:
        case EFormat::R10G10B10A2UF:
        case EFormat::R10G10B10A2UI:
        case EFormat::R11G11B10F:
        case EFormat::R8G8B8A8:
        case EFormat::R8G8B8A8UF:
        case EFormat::R8G8B8A8UFsRGB:
        case EFormat::R8G8B8A8SF:
        case EFormat::R8G8B8A8UI:
        case EFormat::R8G8B8A8SI:
        case EFormat::R8G8B8X8:
        case EFormat::R16G16:
        case EFormat::R16G16F:
        case EFormat::R16G16UF:
        case EFormat::R16G16SF:
        case EFormat::R16G16UI:
        case EFormat::R16G16SI:
        case EFormat::R32:
        case EFormat::R32F:
        case EFormat::R32UI:
        case EFormat::R32SI:
        case EFormat::R24G8:
        case EFormat::R24X8:
        case EFormat::D24UF_S8UI:
            //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
            //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
            //case DXGI_FORMAT_R8G8_B8G8_UNORM:
            //case DXGI_FORMAT_G8R8_G8B8_UNORM:
        case EFormat::B8G8R8A8:
        case EFormat::B8G8R8A8UF:
        case EFormat::B8G8R8A8UFsRGB:
        case EFormat::B8G8R8A8SF:
            //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
        case EFormat::B8G8R8X8:
        case EFormat::B8G8R8X8UF:
        case EFormat::B8G8R8X8UFsRGB:
            //case DXGI_FORMAT_AYUV:
            //case DXGI_FORMAT_Y410:
            //case DXGI_FORMAT_YUY2
            return 32;

        //case DXGI_FORMAT_P010:
        //case DXGI_FORMAT_P016:
        //return 24;

        case EFormat::R8G8:
        case EFormat::R8G8UF:
        case EFormat::R8G8SF:
        case EFormat::R8G8UI:
        case EFormat::R8G8SI:
        case EFormat::R16:
        case EFormat::R16F:
        case EFormat::R16UF:
        case EFormat::R16SF:
        case EFormat::R16UI:
        case EFormat::R16SI:
        case EFormat::R5G6B5:
        case EFormat::R5G6B5UF:
        case EFormat::R5G5B5A1:
        case EFormat::R5G5B5A1UF:
            //case DXGI_FORMAT_A8P8:
            //case DXGI_FORMAT_B4G4R4A4_UNORM:
            return 16;

            //case DXGI_FORMAT_NV12:
            //case DXGI_FORMAT_420_OPAQUE:
            //case DXGI_FORMAT_NV11:
            //    return 12;

        case EFormat::A8:
        case EFormat::A8UF:
        case EFormat::R8:
        case EFormat::R8UI:
        case EFormat::R8SI:
        case EFormat::R8UF:
        case EFormat::R8SF:
            return 8;
#if 0
        case DXGI_FORMAT_AI44:
        case DXGI_FORMAT_IA44:
        case DXGI_FORMAT_P8:
            return 8;

        case DXGI_FORMAT_R1_UNORM:
            return 1;

#endif

        case EFormat::Int1:
        case EFormat::UInt1:
        case EFormat::Float1:
            return 32;

        case EFormat::Int2:
        case EFormat::UInt2:
        case EFormat::Float2:
            return 32 * 2;

        case EFormat::Int3:
        case EFormat::UInt3:
        case EFormat::Float3:
            return 32 * 3;

        case EFormat::Int4:
        case EFormat::UInt4:
        case EFormat::Float4:
            return 32 * 4;

        case EFormat::BC1:
        case EFormat::BC1UF:
        case EFormat::BC1UFsRGB:
        case EFormat::BC4:
        case EFormat::BC4UF:
        case EFormat::BC4SF:
            return 4;

        case EFormat::BC2:
        case EFormat::BC2UF:
        case EFormat::BC2UFsRGB:

        case EFormat::BC3:
        case EFormat::BC3UF:
        case EFormat::BC3UFsRGB:

        case EFormat::BC5:
        case EFormat::BC5UF:
        case EFormat::BC5SF:

        case EFormat::BC6H:
        case EFormat::BC6H_UF16:
        case EFormat::BC6H_SF16:

        case EFormat::BC7:
        case EFormat::BC7UF:
        case EFormat::BC7UFsRGB:
            return 8;

        default:
            return 0;
        }
    }

    inline uint32 GetFormatBytesPerPixel(EFormat Format) { return GetFormatBitsPerPixel(Format) / 8; }

	struct FImageInfo
    {
        uint64 TotalBytes;
        uint32 RowPitch;
        uint32 RowCount;
        uint32 BitsPerPixel;
    };
    BASE_API FImageInfo GetImageInfo(EFormat Format, uint32 Width, uint32 Height);

    inline FImageInfo GetImageInfo(EFormat Format, SizeU Size)
    {
        return GetImageInfo(Format, Size.Width, Size.Height);
    }
    
	struct FTilingInfo
    {
        uint64 TotalBytes;

        uint32 TexelWidth;
        uint32 TexelHeight;

        uint32 TileBytes;
        uint32 TileWidth;
        uint32 TileHeight;
    };
    BASE_API FTilingInfo GetTilingInfo4K(EFormat Format, uint32 Width, uint32 Height);
    inline FTilingInfo GetTilingInfo4K(EFormat Format, SizeU Size)
    {
        return GetTilingInfo4K(Format, Size.Width, Size.Height);
    }

    enum class EDXGIFormat : uint32
    {
        DXGI_FORMAT_UNKNOWN = 0,
        DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
        DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
        DXGI_FORMAT_R32G32B32A32_UINT = 3,
        DXGI_FORMAT_R32G32B32A32_SINT = 4,
        DXGI_FORMAT_R32G32B32_TYPELESS = 5,
        DXGI_FORMAT_R32G32B32_FLOAT = 6,
        DXGI_FORMAT_R32G32B32_UINT = 7,
        DXGI_FORMAT_R32G32B32_SINT = 8,
        DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
        DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
        DXGI_FORMAT_R16G16B16A16_UNORM = 11,
        DXGI_FORMAT_R16G16B16A16_UINT = 12,
        DXGI_FORMAT_R16G16B16A16_SNORM = 13,
        DXGI_FORMAT_R16G16B16A16_SINT = 14,
        DXGI_FORMAT_R32G32_TYPELESS = 15,
        DXGI_FORMAT_R32G32_FLOAT = 16,
        DXGI_FORMAT_R32G32_UINT = 17,
        DXGI_FORMAT_R32G32_SINT = 18,
        DXGI_FORMAT_R32G8X24_TYPELESS = 19,
        DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
        DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
        DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
        DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
        DXGI_FORMAT_R10G10B10A2_UNORM = 24,
        DXGI_FORMAT_R10G10B10A2_UINT = 25,
        DXGI_FORMAT_R11G11B10_FLOAT = 26,
        DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
        DXGI_FORMAT_R8G8B8A8_UNORM = 28,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
        DXGI_FORMAT_R8G8B8A8_UINT = 30,
        DXGI_FORMAT_R8G8B8A8_SNORM = 31,
        DXGI_FORMAT_R8G8B8A8_SINT = 32,
        DXGI_FORMAT_R16G16_TYPELESS = 33,
        DXGI_FORMAT_R16G16_FLOAT = 34,
        DXGI_FORMAT_R16G16_UNORM = 35,
        DXGI_FORMAT_R16G16_UINT = 36,
        DXGI_FORMAT_R16G16_SNORM = 37,
        DXGI_FORMAT_R16G16_SINT = 38,
        DXGI_FORMAT_R32_TYPELESS = 39,
        DXGI_FORMAT_D32_FLOAT = 40,
        DXGI_FORMAT_R32_FLOAT = 41,
        DXGI_FORMAT_R32_UINT = 42,
        DXGI_FORMAT_R32_SINT = 43,
        DXGI_FORMAT_R24G8_TYPELESS = 44,
        DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
        DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
        DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
        DXGI_FORMAT_R8G8_TYPELESS = 48,
        DXGI_FORMAT_R8G8_UNORM = 49,
        DXGI_FORMAT_R8G8_UINT = 50,
        DXGI_FORMAT_R8G8_SNORM = 51,
        DXGI_FORMAT_R8G8_SINT = 52,
        DXGI_FORMAT_R16_TYPELESS = 53,
        DXGI_FORMAT_R16_FLOAT = 54,
        DXGI_FORMAT_D16_UNORM = 55,
        DXGI_FORMAT_R16_UNORM = 56,
        DXGI_FORMAT_R16_UINT = 57,
        DXGI_FORMAT_R16_SNORM = 58,
        DXGI_FORMAT_R16_SINT = 59,
        DXGI_FORMAT_R8_TYPELESS = 60,
        DXGI_FORMAT_R8_UNORM = 61,
        DXGI_FORMAT_R8_UINT = 62,
        DXGI_FORMAT_R8_SNORM = 63,
        DXGI_FORMAT_R8_SINT = 64,
        DXGI_FORMAT_A8_UNORM = 65,
        DXGI_FORMAT_R1_UNORM = 66,
        DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
        DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
        DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
        DXGI_FORMAT_BC1_TYPELESS = 70,
        DXGI_FORMAT_BC1_UNORM = 71,
        DXGI_FORMAT_BC1_UNORM_SRGB = 72,
        DXGI_FORMAT_BC2_TYPELESS = 73,
        DXGI_FORMAT_BC2_UNORM = 74,
        DXGI_FORMAT_BC2_UNORM_SRGB = 75,
        DXGI_FORMAT_BC3_TYPELESS = 76,
        DXGI_FORMAT_BC3_UNORM = 77,
        DXGI_FORMAT_BC3_UNORM_SRGB = 78,
        DXGI_FORMAT_BC4_TYPELESS = 79,
        DXGI_FORMAT_BC4_UNORM = 80,
        DXGI_FORMAT_BC4_SNORM = 81,
        DXGI_FORMAT_BC5_TYPELESS = 82,
        DXGI_FORMAT_BC5_UNORM = 83,
        DXGI_FORMAT_BC5_SNORM = 84,
        DXGI_FORMAT_B5G6R5_UNORM = 85,
        DXGI_FORMAT_B5G5R5A1_UNORM = 86,
        DXGI_FORMAT_B8G8R8A8_UNORM = 87,
        DXGI_FORMAT_B8G8R8X8_UNORM = 88,
        DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
        DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
        DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
        DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
        DXGI_FORMAT_BC6H_TYPELESS = 94,
        DXGI_FORMAT_BC6H_UF16 = 95,
        DXGI_FORMAT_BC6H_SF16 = 96,
        DXGI_FORMAT_BC7_TYPELESS = 97,
        DXGI_FORMAT_BC7_UNORM = 98,
        DXGI_FORMAT_BC7_UNORM_SRGB = 99,
        DXGI_FORMAT_AYUV = 100,
        DXGI_FORMAT_Y410 = 101,
        DXGI_FORMAT_Y416 = 102,
        DXGI_FORMAT_NV12 = 103,
        DXGI_FORMAT_P010 = 104,
        DXGI_FORMAT_P016 = 105,
        DXGI_FORMAT_420_OPAQUE = 106,
        DXGI_FORMAT_YUY2 = 107,
        DXGI_FORMAT_Y210 = 108,
        DXGI_FORMAT_Y216 = 109,
        DXGI_FORMAT_NV11 = 110,
        DXGI_FORMAT_AI44 = 111,
        DXGI_FORMAT_IA44 = 112,
        DXGI_FORMAT_P8 = 113,
        DXGI_FORMAT_A8P8 = 114,
        DXGI_FORMAT_B4G4R4A4_UNORM = 115,

        DXGI_FORMAT_P208 = 130,
        DXGI_FORMAT_V208 = 131,
        DXGI_FORMAT_V408 = 132,

        DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE = 189,
        DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190,

        DXGI_FORMAT_FORCE_UINT = 0xffffffff
    };

    constexpr EFormat DXGIFormatMapping[] =
    {
    	EFormat::None,                      				// DXGI_FORMAT_UNKNOWN = 0
		EFormat::R32G32B32A32,								// DXGI_FORMAT_R32G32B32A32_TYPELESS = 1
		EFormat::R32G32B32A32F,								// DXGI_FORMAT_R32G32B32A32_FLOAT	= 2
		EFormat::R32G32B32A32UI,							// DXGI_FORMAT_R32G32B32A32_UINT = 3
		EFormat::R32G32B32A32SI,							// DXGI_FORMAT_R32G32B32A32_SINT = 4
		EFormat::R32G32B32,									// DXGI_FORMAT_R32G32B32_TYPELESS	= 5
		EFormat::R32G32B32F,								// DXGI_FORMAT_R32G32B32_FLOAT = 6
		EFormat::R32G32B32UI,								// DXGI_FORMAT_R32G32B32_UINT	= 7
		EFormat::R32G32B32SI,								// DXGI_FORMAT_R32G32B32_SINT	= 8
		EFormat::R16G16B16A16,								// DXGI_FORMAT_R16G16B16A16_TYPELESS = 9
		EFormat::R16G16B16A16F,								// DXGI_FORMAT_R16G16B16A16_FLOAT	= 10
		EFormat::R16G16B16A16UF,							// DXGI_FORMAT_R16G16B16A16_UNORM	= 11
		EFormat::R16G16B16A16UI,							// DXGI_FORMAT_R16G16B16A16_UINT = 12
		EFormat::R16G16B16A16SF,							// DXGI_FORMAT_R16G16B16A16_SNORM	= 13
		EFormat::R16G16B16A16SI,							// DXGI_FORMAT_R16G16B16A16_SINT = 14
		EFormat::R32G32,									// DXGI_FORMAT_R32G32_TYPELESS = 15
		EFormat::R32G32F,									// DXGI_FORMAT_R32G32_FLOAT =	16
		EFormat::R32G32UI,									// DXGI_FORMAT_R32G32_UINT = 17
		EFormat::R32G32SI,									// DXGI_FORMAT_R32G32_SINT = 18
		EFormat::None,										// DXGI_FORMAT_R32G8X24_TYPELESS = 19
		EFormat::None,										// DXGI_FORMAT_D32_FLOAT_S8X24_UINT =	20
		EFormat::None,										// DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS =	21
		EFormat::None,										// DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22
		EFormat::R10G10B10A2,								// DXGI_FORMAT_R10G10B10A2_TYPELESS =	23
		EFormat::R10G10B10A2UF,								// DXGI_FORMAT_R10G10B10A2_UNORM = 24
		EFormat::R10G10B10A2UI,								// DXGI_FORMAT_R10G10B10A2_UINT =	25
		EFormat::R11G11B10F,								// DXGI_FORMAT_R11G11B10_FLOAT = 26
		EFormat::R8G8B8A8,									// DXGI_FORMAT_R8G8B8A8_TYPELESS = 27
		EFormat::R8G8B8A8UF,								// DXGI_FORMAT_R8G8B8A8_UNORM	= 28
		EFormat::R8G8B8A8UFsRGB,							// DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29
		EFormat::R8G8B8A8UI,								// DXGI_FORMAT_R8G8B8A8_UINT = 30
		EFormat::R8G8B8A8SF,								// DXGI_FORMAT_R8G8B8A8_SNORM	= 31
        EFormat::R8G8B8A8SI,								// DXGI_FORMAT_R8G8B8A8_SINT = 32
		EFormat::R16G16,									// DXGI_FORMAT_R16G16_TYPELESS = 336
		EFormat::R16G16F,									// DXGI_FORMAT_R16G16_FLOAT =	34
		EFormat::R16G16UF,									// DXGI_FORMAT_R16G16_UNORM =	35
		EFormat::R16G16UI,									// DXGI_FORMAT_R16G16_UINT = 36
		EFormat::R16G16SF,									// DXGI_FORMAT_R16G16_SNORM =	37
		EFormat::R16G16SI,									// DXGI_FORMAT_R16G16_SINT = 38
		EFormat::R32,										// DXGI_FORMAT_R32_TYPELESS =	39
		EFormat::D32F,										// DXGI_FORMAT_D32_FLOAT = 40
		EFormat::R32F,										// DXGI_FORMAT_R32_FLOAT = 41
		EFormat::R32UI,										// DXGI_FORMAT_R32_UINT =	42
		EFormat::R32SI,										// DXGI_FORMAT_R32_SINT =	43
		EFormat::R24G8,										// DXGI_FORMAT_R24G8_TYPELESS	= 44
		EFormat::D24UF_S8UI,										// DXGI_FORMAT_D24_UNORM_S8_UINT = 45
		EFormat::R24X8,										// DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46
		EFormat::None,										// DXGI_FORMAT_X24_TYPELESS_G8_UINT =	47
		EFormat::R8G8,										// DXGI_FORMAT_R8G8_TYPELESS = 48
		EFormat::R8G8UF,									// DXGI_FORMAT_R8G8_UNORM	= 49
		EFormat::R8G8UI,									// DXGI_FORMAT_R8G8_UINT = 50
		EFormat::R8G8SF,									// DXGI_FORMAT_R8G8_SNORM	= 51
		EFormat::R8G8SI,									// DXGI_FORMAT_R8G8_SINT = 52
		EFormat::R16,										// DXGI_FORMAT_R16_TYPELESS =	53
		EFormat::R16F,										// DXGI_FORMAT_R16_FLOAT = 54
		EFormat::D16UF,										// DXGI_FORMAT_D16_UNORM = 55
		EFormat::R16UF,										// DXGI_FORMAT_R16_UNORM = 56
		EFormat::R16UI,										// DXGI_FORMAT_R16_UINT =	57
		EFormat::R16SF,										// DXGI_FORMAT_R16_SNORM = 58
		EFormat::R16SI,										// DXGI_FORMAT_R16_SINT =	59
		EFormat::R8,										// DXGI_FORMAT_R8_TYPELESS = 60
		EFormat::R8UF,										// DXGI_FORMAT_R8_UNORM =	61
		EFormat::R8UI,										// DXGI_FORMAT_R8_UINT = 62
		EFormat::R8SF,										// DXGI_FORMAT_R8_SNORM =	63
		EFormat::R8SI,										// DXGI_FORMAT_R8_SINT = 64
		EFormat::A8UF,										// DXGI_FORMAT_A8_UNORM =	65
		EFormat::None,										// DXGI_FORMAT_R1_UNORM =	66
		EFormat::None,										// DXGI_FORMAT_R9G9B9E5_SHAREDEXP	= 67
		EFormat::None,										// DXGI_FORMAT_R8G8_B8G8_UNORM = 68
		EFormat::None,										// DXGI_FORMAT_G8R8_G8B8_UNORM = 69
		EFormat::BC1,										// DXGI_FORMAT_BC1_TYPELESS =	70
		EFormat::BC1UF, 									// DXGI_FORMAT_BC1_UNORM = 71
		EFormat::BC1UFsRGB,									// DXGI_FORMAT_BC1_UNORM_SRGB	= 72
		EFormat::BC2,										// DXGI_FORMAT_BC2_TYPELESS =	73
		EFormat::BC2UF,										// DXGI_FORMAT_BC2_UNORM = 74
		EFormat::BC2UFsRGB,									// DXGI_FORMAT_BC2_UNORM_SRGB	= 75
		EFormat::BC3,										// DXGI_FORMAT_BC3_TYPELESS =	76
		EFormat::BC3UF,										// DXGI_FORMAT_BC3_UNORM = 77
		EFormat::BC3UFsRGB,									// DXGI_FORMAT_BC3_UNORM_SRGB	= 78
		EFormat::BC4,										// DXGI_FORMAT_BC4_TYPELESS =	79
		EFormat::BC4UF,										// DXGI_FORMAT_BC4_UNORM = 80
		EFormat::BC4SF,										// DXGI_FORMAT_BC4_SNORM = 81
		EFormat::BC5,										// DXGI_FORMAT_BC5_TYPELESS =	82
		EFormat::BC5UF,										// DXGI_FORMAT_BC5_UNORM = 83
		EFormat::BC5SF,										// DXGI_FORMAT_BC5_SNORM = 84
		EFormat::None,										// DXGI_FORMAT_B5G6R5_UNORM =	85
		EFormat::None,										// DXGI_FORMAT_B5G5R5A1_UNORM	= 86
		EFormat::B8G8R8A8UF,								// DXGI_FORMAT_B8G8R8A8_UNORM	= 87
		EFormat::B8G8R8X8UF,								// DXGI_FORMAT_B8G8R8X8_UNORM	= 88
		EFormat::None,										// DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM	= 89
		EFormat::B8G8R8A8,									// DXGI_FORMAT_B8G8R8A8_TYPELESS = 90
		EFormat::B8G8R8A8UFsRGB,							// DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91
		EFormat::B8G8R8X8,									// DXGI_FORMAT_B8G8R8X8_TYPELESS = 92
		EFormat::B8G8R8X8UFsRGB,							// DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93
		EFormat::BC6H,										// DXGI_FORMAT_BC6H_TYPELESS = 94
		EFormat::BC6H_UF16,									// DXGI_FORMAT_BC6H_UF16 = 95
		EFormat::BC6H_SF16,									// DXGI_FORMAT_BC6H_SF16 = 96
		EFormat::BC7,										// DXGI_FORMAT_BC7_TYPELESS =	97
		EFormat::BC7UF,										// DXGI_FORMAT_BC7_UNORM = 98
		EFormat::BC7UFsRGB,									// DXGI_FORMAT_BC7_UNORM_SRGB	= 99
		EFormat::None,										// DXGI_FORMAT_AYUV =	100
		EFormat::None,										// DXGI_FORMAT_Y410 =	101
		EFormat::None,										// DXGI_FORMAT_Y416 =	102
		EFormat::None,										// DXGI_FORMAT_NV12 =	103
		EFormat::None,										// DXGI_FORMAT_P010 =	104
		EFormat::None,										// DXGI_FORMAT_P016 =	105
		EFormat::None,										// DXGI_FORMAT_420_OPAQUE	= 106
		EFormat::None,										// DXGI_FORMAT_YUY2 =	107
		EFormat::None,										// DXGI_FORMAT_Y210 =	108
		EFormat::None,										// DXGI_FORMAT_Y216 =	109
		EFormat::None,										// DXGI_FORMAT_NV11 =	110
		EFormat::None,										// DXGI_FORMAT_AI44 =	111
		EFormat::None,										// DXGI_FORMAT_IA44 =	112
		EFormat::None,										// DXGI_FORMAT_P8	= 113
		EFormat::None,										// DXGI_FORMAT_A8P8 =	114
		EFormat::None,										// DXGI_FORMAT_B4G4R4A4_UNORM	= 115

		EFormat::None,										// DXGI_FORMAT_P208 =	130
		EFormat::None,										// DXGI_FORMAT_V208 =	131
		EFormat::None,										// DXGI_FORMAT_V408 =	132

		EFormat::None,										// DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE = 189
		EFormat::None,								        // DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190
    };
    static_assert(DXGIFormatMapping[(int)EDXGIFormat::DXGI_FORMAT_BC7_UNORM_SRGB] == EFormat::BC7UFsRGB);
    constexpr EFormat FormatFromDXGIFormat(EDXGIFormat DXGIFormat)
    {
        return (uintx)DXGIFormat < ArraySize(DXGIFormatMapping) ? DXGIFormatMapping[(int)DXGIFormat] : EFormat::None;
    }

    constexpr EDXGIFormat DXGIFormatFromFormat(EFormat Format)
    {
        switch(Format)
        {
        case EFormat::Short1: return EDXGIFormat::DXGI_FORMAT_R16_SINT;
        case EFormat::Short2: return EDXGIFormat::DXGI_FORMAT_R16G16_SINT;
        //case EFormat::Short3: return EDXGIFormat::DXGI_FORMAT_R16G16B16_SINT; // !!!
        case EFormat::Short4: return EDXGIFormat::DXGI_FORMAT_R16G16B16A16_SINT;

        case EFormat::UShort1: return EDXGIFormat::DXGI_FORMAT_R16_UINT;
        case EFormat::UShort2: return EDXGIFormat::DXGI_FORMAT_R16G16_UINT;
        //case EFormat::UShort3: return EDXGIFormat::DXGI_FORMAT_R16G16B16_UINT; // !!!
        case EFormat::UShort4: return EDXGIFormat::DXGI_FORMAT_R16G16B16A16_UINT;

        case EFormat::Int1: return EDXGIFormat::DXGI_FORMAT_R32_SINT;
        case EFormat::Int2: return EDXGIFormat::DXGI_FORMAT_R32G32_SINT;
        case EFormat::Int3: return EDXGIFormat::DXGI_FORMAT_R32G32B32_SINT;
        case EFormat::Int4: return EDXGIFormat::DXGI_FORMAT_R32G32B32A32_SINT;

        case EFormat::UInt1: return EDXGIFormat::DXGI_FORMAT_R32_UINT;
        case EFormat::UInt2: return EDXGIFormat::DXGI_FORMAT_R32G32_UINT;
        case EFormat::UInt3: return EDXGIFormat::DXGI_FORMAT_R32G32B32_UINT;
        case EFormat::UInt4: return EDXGIFormat::DXGI_FORMAT_R32G32B32A32_UINT;

        case EFormat::Half1: return EDXGIFormat::DXGI_FORMAT_R16_FLOAT;
        case EFormat::Half2: return EDXGIFormat::DXGI_FORMAT_R16G16_FLOAT;
        //case EFormat::Half3: return EDXGIFormat::DXGI_FORMAT_R16G16B16_FLOAT; // !!!
        case EFormat::Half4: return EDXGIFormat::DXGI_FORMAT_R16G16B16A16_FLOAT;

        case EFormat::Float1: return EDXGIFormat::DXGI_FORMAT_R32_FLOAT;
        case EFormat::Float2: return EDXGIFormat::DXGI_FORMAT_R32G32_FLOAT;
        case EFormat::Float3: return EDXGIFormat::DXGI_FORMAT_R32G32B32_FLOAT;
        case EFormat::Float4: return EDXGIFormat::DXGI_FORMAT_R32G32B32A32_FLOAT;

        default:
            break;
        }
	    for (uint32 FormatIndex = 0; FormatIndex < ArraySize(DXGIFormatMapping); ++FormatIndex)
	    {
            if (DXGIFormatMapping[FormatIndex] == Format)
                return (EDXGIFormat)FormatIndex;
	    }
        return EDXGIFormat::DXGI_FORMAT_UNKNOWN;
    }

    forceinline bool IsTypelessFormat(EFormat Format)
    {
        switch (Format)
        {
        case EFormat::R8G8B8A8:
        case EFormat::B8G8R8A8:
        case EFormat::B8G8R8X8:
        case EFormat::R10G10B10A2:
        case EFormat::R16G16:
        case EFormat::R16G16B16A16:
        case EFormat::R32G32:
        case EFormat::R32G32B32:
        case EFormat::R32G32B32A32:
        case EFormat::A8:
        case EFormat::R8:
        case EFormat::R16:
        case EFormat::R32:
        case EFormat::BC1:
        case EFormat::BC2:
        case EFormat::BC3:
        case EFormat::BC4:
        case EFormat::BC5:
        case EFormat::BC6H:
        case EFormat::BC7:
        case EFormat::R8G8:
        case EFormat::R5G6B5:
        case EFormat::R8G8B8:
        case EFormat::B8G8R8:
            return true;
        default:
            return false;
        }
    }

    forceinline bool IsAlphaFormat(EFormat Format)
    {
        switch (Format)
        {
		case EFormat::R8G8B8A8:
		case EFormat::R8G8B8A8UF:
		case EFormat::R8G8B8A8SF:
		case EFormat::R8G8B8A8UFsRGB:
		case EFormat::R8G8B8A8UI:
		case EFormat::R8G8B8A8SI:
            return true;
		case EFormat::R8G8B8X8:
			break;
		case EFormat::B8G8R8A8:
		case EFormat::B8G8R8A8UF:
		case EFormat::B8G8R8A8UFsRGB:
		case EFormat::B8G8R8A8SF:
            return true;
		case EFormat::B8G8R8X8:
			break;
		case EFormat::B8G8R8X8UF:
			break;
		case EFormat::B8G8R8X8UFsRGB:
			break;
		case EFormat::R10G10B10A2:
		case EFormat::R10G10B10A2UF:
		case EFormat::R10G10B10A2UI:
            return true;
		case EFormat::R11G11B10F:
			break;
		case EFormat::R16G16:
			break;
		case EFormat::R16G16F:
			break;
		case EFormat::R16G16UF:
			break;
		case EFormat::R16G16SF:
			break;
		case EFormat::R16G16UI:
			break;
		case EFormat::R16G16SI:
			break;
		case EFormat::R16G16B16A16:
		case EFormat::R16G16B16A16F:
		case EFormat::R16G16B16A16UF:
		case EFormat::R16G16B16A16SF:
		case EFormat::R16G16B16A16UI:
		case EFormat::R16G16B16A16SI:
            return true;
		case EFormat::R32G32:
			break;
		case EFormat::R32G32F:
			break;
		case EFormat::R32G32UI:
			break;
		case EFormat::R32G32SI:
			break;
		case EFormat::R32G32B32:
			break;
		case EFormat::R32G32B32F:
			break;
		case EFormat::R32G32B32UI:
			break;
		case EFormat::R32G32B32SI:
			break;
		case EFormat::R32G32B32A32:
		case EFormat::R32G32B32A32F:
		case EFormat::R32G32B32A32UF:
		case EFormat::R32G32B32A32SF:
		case EFormat::R32G32B32A32UI:
		case EFormat::R32G32B32A32SI:
            return true;
		case EFormat::A8:
		case EFormat::A8UF:
            return true;
		case EFormat::R8:
			break;
		case EFormat::R8UI:
			break;
		case EFormat::R8SI:
			break;
		case EFormat::R8UF:
			break;
		case EFormat::R8SF:
			break;
		case EFormat::R16:
			break;
		case EFormat::R16F:
			break;
		case EFormat::R16UI:
			break;
		case EFormat::R16SI:
			break;
		case EFormat::R16UF:
			break;
		case EFormat::R16SF:
			break;
		case EFormat::R24G8:
			break;
		case EFormat::R24X8:
			break;
		case EFormat::R32:
			break;
		case EFormat::R32F:
			break;
		case EFormat::R32UI:
			break;
		case EFormat::R32SI:
			break;
		case EFormat::BC1:
			break;
		case EFormat::BC1UF:
			break;
		case EFormat::BC1UFsRGB:
			break;
		case EFormat::BC1SF:
			break;
		case EFormat::BC2:
			break;
		case EFormat::BC2UF:
			break;
		case EFormat::BC2UFsRGB:
			break;
		case EFormat::BC3:
			break;
		case EFormat::BC3UF:
			break;
		case EFormat::BC3UFsRGB:
			break;
		case EFormat::BC4:
			break;
		case EFormat::BC4UF:
			break;
		case EFormat::BC4SF:
			break;
		case EFormat::BC5:
			break;
		case EFormat::BC5UF:
			break;
		case EFormat::BC5SF:
			break;
		case EFormat::BC6H:
			break;
		case EFormat::BC6H_UF16:
			break;
		case EFormat::BC6H_SF16:
			break;
		case EFormat::BC7:
			break;
		case EFormat::BC7UF:
			break;
		case EFormat::BC7UFsRGB:
			break;
		case EFormat::D16UF:
			break;
		case EFormat::D32F:
			break;
		case EFormat::D24UF_S8UI:
			break;
		case EFormat::R3G3B2:
			break;
		case EFormat::R8G8:
			break;
		case EFormat::R8G8UF:
			break;
		case EFormat::R8G8SF:
			break;
		case EFormat::R8G8UI:
			break;
		case EFormat::R8G8SI:
			break;
		case EFormat::R5G6B5:
			break;
		case EFormat::R5G6B5UF:
			break;
		case EFormat::R5G5B5A1:
		case EFormat::R5G5B5A1UF:
            return true;
		case EFormat::B5G6R5:
			break;
		case EFormat::B5G5R5A1:
            return true;
		case EFormat::A1R5G5B5:
		case EFormat::A1B5G5R5:
            return true;
		case EFormat::X1R5G5B5:
			break;
		case EFormat::X1B5G5R5:
			break;
		case EFormat::A4R4G4B4:
            return true;
		case EFormat::X4R4G4B4:
			break;
		case EFormat::A8R3G3B2:
            return true;
		case EFormat::B4G4R4A4:
            return true;
		case EFormat::R8G8B8:
			break;
		case EFormat::B8G8R8:
			break;
		case EFormat::A8R5G6B5:
		case EFormat::A8B5G6R5:
            return true;
		case EFormat::A8X1R5G5B5:
		case EFormat::A8X1B5G5R5:
            return true;
		case EFormat::A2R10G10B10:
		case EFormat::A2B10G10R10:
            return true;
		case EFormat::INDEX4_A8R8G8B8:
			break;
		case EFormat::INDEX8_A8R8G8B8:
			break;
			break;
		case EFormat::Short1:
			break;
		case EFormat::Short2:
			break;
		case EFormat::Short3:
			break;
		case EFormat::Short4:
			break;
		case EFormat::UShort1:
			break;
		case EFormat::UShort2:
			break;
		case EFormat::UShort3:
			break;
		case EFormat::UShort4:
			break;
		case EFormat::Int1:
			break;
		case EFormat::Int2:
			break;
		case EFormat::Int3:
			break;
		case EFormat::Int4:
			break;
		case EFormat::UInt1:
			break;
		case EFormat::UInt2:
			break;
		case EFormat::UInt3:
			break;
		case EFormat::UInt4:
			break;
		case EFormat::Half1:
			break;
		case EFormat::Half2:
			break;
		case EFormat::Half3:
			break;
		case EFormat::Half4:
			break;
		case EFormat::Float1:
			break;
		case EFormat::Float2:
			break;
		case EFormat::Float3:
			break;
		case EFormat::Float4:
			break;
		case EFormat::Struct:
			break;
		case EFormat::GRAY1:
			break;
		case EFormat::GRAY2:
			break;
		case EFormat::GRAY4:
			break;
		case EFormat::GRAY8:
			break;
		case EFormat::I420:
			break;
		case EFormat::I422:
			break;
		case EFormat::I444:
			break;
		case EFormat::NV11:
			break;
		case EFormat::NV12:
			break;
		case EFormat::NV21:
			break;
		case EFormat::YU12:
			break;
		case EFormat::YUV2:
			break;
		case EFormat::YUY2:
			break;
		case EFormat::YUYV:
			break;
		case EFormat::YV12:
			break;
		case EFormat::YVYU:
			break;
		case EFormat::UYUV:
			break;
		case EFormat::UYVY:
			break;
		case EFormat::VYVU:
			break;
		case EFormat::VUVY:
			break;
		case EFormat::CMYK:
			break;
		case EFormat::YCCK:
			break;
		case EFormat::Y420:
			break;
		case EFormat::P010:
			break;
		case EFormat::P016:
			break;
		default:
            return false;
        }

        return false;
    }


    forceinline bool IsDepthStencilFormat(EFormat Format)
    {
        switch (Format)
        {
        case EFormat::D16UF:
        case EFormat::D24UF_S8UI:
        case EFormat::D32F:
            return true;
        default:
            return false;
        }
    }

    forceinline bool IsStencilFormat(EFormat Format)
    {
        switch (Format)
        {
        case EFormat::D24UF_S8UI:
            return true;
        default:
            return false;
        }
    }

    forceinline bool IsSwizzleFormat(EFormat Format)
    {
        return EFormat::R8G8B8A8 <= Format && Format <= EFormat::A2B10G10R10;
    }

    [[deprecated("use IsSwizzleFormat instead.")]]
    forceinline bool IsRGBFormat(EFormat Format)
    {
        return IsSwizzleFormat(Format);
    }

    forceinline bool IsDXTCompressFormat(EFormat Format)
    {
        switch (Format)
        {
        case EFormat::BC1:
        case EFormat::BC1UF:
        case EFormat::BC1UFsRGB:
        case EFormat::BC1SF:
            return true;

        case EFormat::BC2:
        case EFormat::BC2UF:
        case EFormat::BC2UFsRGB:
            return true;

        case EFormat::BC3:
        case EFormat::BC3UF:
        case EFormat::BC3UFsRGB:
            return true;

        case EFormat::BC4:
        case EFormat::BC4UF:
        case EFormat::BC4SF:
            return true;

        case EFormat::BC5:
        case EFormat::BC5UF:
        case EFormat::BC5SF:
            return true;

        case EFormat::BC6H:
        case EFormat::BC6H_UF16:
        case EFormat::BC6H_SF16:
            return true;

        case EFormat::BC7:
        case EFormat::BC7UF:
        case EFormat::BC7UFsRGB:
            return true;

        default:
            return false;
        }
    }

    forceinline bool IsASTCompressFormat(EFormat Format)
    {
        switch (Format)
        {
        case EFormat::ASTC4x4:
        case EFormat::ASTC5x4:
        case EFormat::ASTC5x5:
        case EFormat::ASTC6x5:
        case EFormat::ASTC6x6:
        case EFormat::ASTC8x5:
        case EFormat::ASTC8x6:
            return true;

        default:
            return false;
        }
    }

    forceinline EFormat MappingDepthStencilFormat(EFormat Format)
    {
        switch (Format)
        {
        case EFormat::R16:
        case EFormat::R16F:
        case EFormat::R16UF:
        case EFormat::R16SF:
        case EFormat::R16UI:
        case EFormat::R16SI:
            return EFormat::D16UF;
        case EFormat::R32:
        case EFormat::R32F:
        case EFormat::R32UI:
        case EFormat::R32SI:
            return EFormat::D32F;
        default:
            return Format;
        }
    }

    enum class EImageFormat
    {
	    None = 0,
        ASTC     = MakeFourCC("ASTC"),
        BMP     = MakeFourCC("BMP "),
        HEIF    = MakeFourCC("HEIF"),
        ICO     = MakeFourCC("ICO "),
        JPEG    = MakeFourCC("JPEG"),
        JNG     = MakeFourCC("JNG "),
        KOALA   = MakeFourCC("KOAL"),
        LBM     = MakeFourCC("LBM "),
        IFF     = MakeFourCC("IFF "),
        MNG     = MakeFourCC("MNG "),
        PBM     = MakeFourCC("PBM "),
        PBMRAW  = MakeFourCC("PBMR"),
        PCD     = MakeFourCC("PCD "),
        PCX     = MakeFourCC("PCX "),
        PGM     = MakeFourCC("PGM "),
        PGMRAW  = MakeFourCC("PGMR"),
        PNG     = MakeFourCC("PNG "),
        PPM     = MakeFourCC("PPM "),
        PPMRAW  = MakeFourCC("PPMR"),
        RAS     = MakeFourCC("RAS "),
        TARGA   = MakeFourCC("TARG"),
        TIFF    = MakeFourCC("TIFF"),
        WBMP    = MakeFourCC("WBMP"),
        PSD     = MakeFourCC("PSD "),
        CUT     = MakeFourCC("CUT "),
        XBM     = MakeFourCC("XBM "),
        XPM     = MakeFourCC("XPM "),
        DDS     = MakeFourCC("DDS "),
        GIF     = MakeFourCC("GIF "),
        HDR     = MakeFourCC("HDR "),
        FAXG3   = MakeFourCC("FAXG"),
        SGI     = MakeFourCC("SGI "),
        EXR     = MakeFourCC("EXR "),
        J2K     = MakeFourCC("J2K "),
        JP2     = MakeFourCC("JP2 "),
        PFM     = MakeFourCC("PFM "),
        PICT    = MakeFourCC("PICT"),
        RAW     = MakeFourCC("RAW "),
        WEBP    = MakeFourCC("WEBP"),
        JXR     = MakeFourCC("JXR "),
    };
}
