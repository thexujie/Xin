#include "PCH.h"
#include "Format.h"
#include "Formatter.h"

namespace Xin
{
    template<>
    struct TFormatter<EFormat>
    {
        FStringV Format(EFormat Format, FStringV Formal = FStringV::None) const
        {
            return FormatToString(Format);
        }

        EFormat Parse(FStringV String) const
        {
            return FormatFromString(String);
        }
    };

	EFormat FormatFromString(TView<char8_t> String)
	{
        if (String.Size < 4)
			return EFormat::None;

        if (String.Size == 4)
        {
            EFormat Format = (EFormat)MakeFourCC(String[0], String[1], String[2], String[3]);
            return Format;
        }
        else if (String.Size == 5)
        {
            if (StringEqualIC(String, u8"short"V))
                return EFormat::Short1;
            else if (StringEqualIC(String, u8"ushort"V))
                return EFormat::UShort1;
            else if (StringEqualIC(String, u8"int"V))
                return EFormat::Int1;
            else if (StringEqualIC(String, u8"uint"V))
                return EFormat::UInt1;
            else if (StringEqualIC(String, u8"float"V))
                return EFormat::Float1;
            else
                return EFormat::None;
        }
        else if (String.Size == 6)
        {
            enum EArrayFormatTypeIndex
            {
                AFTI_Short = 0,
                AFTI_UShort,
                AFTI_Int,
                AFTI_UInt,
                AFTI_Float,
                AFTI_Count,
            };

            EFormat BaseFormats[AFTI_Count][4] =
            {
                { EFormat::Short1, EFormat::Short2, EFormat::Short3, EFormat::Short4 },
                { EFormat::UShort1, EFormat::UShort2, EFormat::UShort3, EFormat::UShort4 },
                { EFormat::Int1, EFormat::Int2, EFormat::Int3, EFormat::Int4 },
                { EFormat::UInt1, EFormat::UInt2, EFormat::UInt3, EFormat::UInt4 },
                { EFormat::Float1, EFormat::Float2, EFormat::Float3, EFormat::Float4 },
            };
            EArrayFormatTypeIndex BaseType = AFTI_Short;
            uint32 ArraySize = 0;
	        if (StringEqualIC(String.SubView(0, 5), u8"short"V))
	        {
                BaseType = AFTI_Short;
                ArraySize = String[5] - u8'0';
	        }
            else if (StringEqualIC(String.SubView(0, 5), u8"ushort"V))
            {
                BaseType = AFTI_UShort;
                ArraySize = String[5] - u8'0';
            }
            else if (StringEqualIC(String.SubView(0, 5), u8"int"V))
            {
                BaseType = AFTI_Int;
                ArraySize = String[5] - u8'0';
            }
            else if (StringEqualIC(String.SubView(0, 5), u8"uint"V))
            {
                BaseType = AFTI_UInt;
                ArraySize = String[5] - u8'0';
            }
            else if (StringEqualIC(String.SubView(0, 5), u8"float"V))
            {
                BaseType = AFTI_Float;
                ArraySize = String[5] - u8'0';
            }
            else
                return EFormat::None;

            if (ArraySize < 1 || 4 < ArraySize)
                return EFormat::None;

            return BaseFormats[BaseType][ArraySize - 1];
        }
        else
			return EFormat::None;
	}

	TView<char8_t> FormatToString(EFormat Format)
	{
		return {};
	}

    FFormatInfo GetFormatInfo(EFormat Format)
	{
        FFormatInfo FormatInfo { };
        switch (Format)
        {
        case EFormat::A8:
        case EFormat::A8UF:
        case EFormat::R8:
        case EFormat::R8UI:
        case EFormat::R8SI:
        case EFormat::R8UF:
        case EFormat::R8SF:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 8;
            break;
        case EFormat::R16:
        case EFormat::R16F:
        case EFormat::R16UI:
        case EFormat::R16SI:
        case EFormat::R16UF:
        case EFormat::R16SF:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 16;
            break;
        case EFormat::R32:
        case EFormat::R32F:
        case EFormat::R32UI:
        case EFormat::R32SI:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 32;
            break;
        case EFormat::R8G8B8:
        case EFormat::B8G8R8:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 24;
            break;
        case EFormat::R8G8B8A8:
        case EFormat::R8G8B8A8UF:
        case EFormat::R8G8B8A8SF:
        case EFormat::R8G8B8A8UFsRGB:
        case EFormat::R8G8B8A8UI:
        case EFormat::R8G8B8A8SI:
        case EFormat::R8G8B8X8:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 32;
            break;
        case EFormat::B8G8R8A8:
        case EFormat::B8G8R8A8UF:
        case EFormat::B8G8R8A8UFsRGB:
        case EFormat::B8G8R8A8SF:
        case EFormat::B8G8R8X8:
        case EFormat::B8G8R8X8UF:
        case EFormat::B8G8R8X8UFsRGB:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 32;
            break;
        case EFormat::R10G10B10A2:
        case EFormat::R10G10B10A2UF:
        case EFormat::R10G10B10A2UI:
        case EFormat::R11G11B10F:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 32;
            break;
        case EFormat::R16G16:
        case EFormat::R16G16F:
        case EFormat::R16G16UF:
        case EFormat::R16G16SF:
        case EFormat::R16G16UI:
        case EFormat::R16G16SI:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 32;
            break;

        case EFormat::R16G16B16A16:
        case EFormat::R16G16B16A16F:
        case EFormat::R16G16B16A16UF:
        case EFormat::R16G16B16A16SF:
        case EFormat::R16G16B16A16UI:
        case EFormat::R16G16B16A16SI:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 64;
            break;

        case EFormat::R32G32:
        case EFormat::R32G32F:
        case EFormat::R32G32UI:
        case EFormat::R32G32SI:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 64;
            break;

        case EFormat::R32G32B32:
        case EFormat::R32G32B32F:
        case EFormat::R32G32B32UI:
        case EFormat::R32G32B32SI:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 96;
            break;

        case EFormat::R32G32B32A32:
        case EFormat::R32G32B32A32F:
        case EFormat::R32G32B32A32UF:
        case EFormat::R32G32B32A32SF:
        case EFormat::R32G32B32A32UI:
        case EFormat::R32G32B32A32SI:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 1;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 128;
            break;

        case EFormat::BC1:
        case EFormat::BC1UF:
        case EFormat::BC1UFsRGB:
        case EFormat::BC1SF:
        case EFormat::BC4:
        case EFormat::BC4UF:
        case EFormat::BC4SF:
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 4;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 4 * 16;
            break;

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
            FormatInfo.BlockWidth = FormatInfo.BlockHeight = 4;
            FormatInfo.BlockDepth = 1;
            FormatInfo.BlockBits = 8 * 16;
            break;
        default:
            break;
        }

        return FormatInfo;
	}

    FImageInfo GetImageInfo(EFormat Format, uint32 Width, uint32 Height)
    {
        uint32 BitsPerPixel = 0;
        uint64 TotalBytes = 0;
        uint32 RowPitch = 0;
        uint32 RowCount = 0;

        bool IsBC = false;
        bool IsPacked = false;
        bool IsPlanar = false;
        uint32 BytesPerE = 0;
        switch (Format)
        {
        case EFormat::BC1:
        case EFormat::BC1UF:
        case EFormat::BC1UFsRGB:
        case EFormat::BC1SF:
        case EFormat::BC4:
        case EFormat::BC4UF:
        case EFormat::BC4SF:
            IsBC = true;
            BytesPerE = 8;
            break;

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
            IsBC = true;
            BytesPerE = 16;
            break;
#if 0
        case DXGI_FORMAT_R8G8_B8G8_UNORM:
        case DXGI_FORMAT_G8R8_G8B8_UNORM:
        case DXGI_FORMAT_YUY2:
            IsPacked = true;
            bpe = 4;
            break;

        case DXGI_FORMAT_Y210:
        case DXGI_FORMAT_Y216:
            IsPacked = true;
            bpe = 8;
            break;

        case DXGI_FORMAT_420_OPAQUE:
            IsPlanar = true;
            bpe = 2;
            break;
#endif
        case EFormat::NV12:
            IsPlanar = true;
            BytesPerE = 2;
            break;

        case EFormat::P010:
        case EFormat::P016:
            IsPlanar = true;
            BytesPerE = 4;
            break;
        default:
            break;
        }

        if (IsBC)
        {
            uint32 BlocksWideCount = 0;
            if (Width > 0)
                BlocksWideCount = Max<uint32>(1, (Width + 3) / 4);

        	uint32 BlocksHighCount = 0;
            if (Height > 0)
            {
                BlocksHighCount = Max<uint32>(1, (Height + 3) / 4);
            }
            RowPitch = BlocksWideCount * BytesPerE;
            RowCount = BlocksHighCount;
            TotalBytes = (uint64)RowPitch * BlocksHighCount;
        }
        else if (IsPacked)
        {
            RowPitch = ((Width + 1) >> 1) * BytesPerE;
            RowCount = Height;
            TotalBytes = (uint64)RowPitch * Height;
        }
        else if (Format == EFormat::NV11)
        {
            RowPitch = ((Width + 3) >> 2) * 4;
            RowCount = Width * 2; // Direct3D makes this simplifying assumption, although it is larger than the 4:1:1 data
            TotalBytes = (uint64)RowPitch * RowCount;
        }
        else if (IsPlanar)
        {
            RowPitch = ((Width + 1) >> 1) * BytesPerE;
            TotalBytes = ((uint64)RowPitch * Height) + ((RowPitch * Height + 1) >> 1);
            RowCount = Height + ((Height + 1) >> 1);
        }
        else
        {
            BitsPerPixel = GetFormatBitsPerPixel(Format);
            RowPitch = (Width * BitsPerPixel + 7) / 8; // round up to nearest byte
            RowCount = Height;
            TotalBytes = (uint64)RowPitch * Height;
        }

        return { TotalBytes , RowPitch, RowCount, BitsPerPixel };
    }

    FTilingInfo GetTilingInfo4K(EFormat Format, uint32 Width, uint32 Height)
	{
        // A 4KB tile shape for D3D12_RESOURCE_DIMENSION_TEXTURE2D and R8_UNORM is 64 texels wide, 64 texels high, and 1 texels deep.
        // A 4KB tile shape for D3D12_RESOURCE_DIMENSION_TEXTURE2D and R8G8B8A8_UNORM is 32 texels wide, 32 texels high, and 1 texels deep.
        FTilingInfo TilingInfo;
        TilingInfo.TileBytes = 4096;
        switch (Format)
        {
        case EFormat::R32G32B32A32:
        case EFormat::R32G32B32A32F:
        case EFormat::R32G32B32A32UF:
        case EFormat::R32G32B32A32SF:
        case EFormat::R32G32B32A32UI:
        case EFormat::R32G32B32A32SI:
            // 16 * 16 * 16 = 4096
            TilingInfo.TexelWidth = 16;
        	TilingInfo.TexelHeight = 16;
            break;

        //case EFormat::R32G32B32:
        //case EFormat::R32G32B32F:
        //case EFormat::R32G32B32UI:
        //case EFormat::R32G32B32SI:
        //    return 96;

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
            // 8 * 32 * 16 = 4096
            TilingInfo.TexelWidth = 32;
            TilingInfo.TexelHeight = 16;
            break;

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
            // 4 * 32 * 32 = 4096
            TilingInfo.TexelWidth = 32;
            TilingInfo.TexelHeight = 32;
            break;

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
            // 2 * 64 * 32 = 4096
            TilingInfo.TexelWidth = 64;
            TilingInfo.TexelHeight = 32;
            break;

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
            // 1 * 64 * 64 = 4096
            TilingInfo.TexelWidth = 64;
            TilingInfo.TexelHeight = 64;
            break;
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
            TilingInfo.TexelWidth = 32;
            TilingInfo.TexelHeight = 32;
            break;

        case EFormat::Int4:
        case EFormat::UInt4:
        case EFormat::Float4:
            TilingInfo.TexelWidth = 16;
            TilingInfo.TexelHeight = 16;
            break;

        case EFormat::BC1:
        case EFormat::BC1UF:
        case EFormat::BC1UFsRGB:
        case EFormat::BC4:
        case EFormat::BC4UF:
        case EFormat::BC4SF:
            TilingInfo.TexelWidth = 64;
            TilingInfo.TexelHeight = 64;
            break;

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
            TilingInfo.TexelWidth = 64;
            TilingInfo.TexelHeight = 32;
            break;
        default:
            TilingInfo.TexelWidth = 64;
            TilingInfo.TexelHeight = 64;
            break;
        }

        TilingInfo.TileWidth = (Width + TilingInfo.TexelWidth - 1) / TilingInfo.TexelWidth;
        TilingInfo.TileHeight = (Height + TilingInfo.TexelHeight - 1) / TilingInfo.TexelHeight;
        TilingInfo.TotalBytes = TilingInfo.TileWidth * TilingInfo.TileHeight * TilingInfo.TileBytes;
        return TilingInfo;
	}
}
