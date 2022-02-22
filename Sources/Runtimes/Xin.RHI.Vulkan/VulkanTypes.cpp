#include "PCH.h"
#include "VulkanTypes.h"

namespace Xin::RHI::Vulkan
{
	FVulkanShaderParameter FVulkanShaderParameter::None;

	VkFormat FromFormat(EFormat Format)
	{
		static const VkFormat UndefinedFormat = VK_FORMAT_UNDEFINED;
		switch (Format)
		{
		case EFormat::None: return UndefinedFormat;
		case EFormat::R8G8B8A8: return VK_FORMAT_R8G8B8A8_UINT;
		case EFormat::R8G8B8A8UF: return VK_FORMAT_R8G8B8A8_UNORM;
		case EFormat::R8G8B8A8SF: return VK_FORMAT_R8G8B8A8_SNORM;
		case EFormat::R8G8B8A8UFsRGB: return VK_FORMAT_R8G8B8A8_SRGB;
		case EFormat::R8G8B8A8UI: return VK_FORMAT_R8G8B8A8_UINT;
		case EFormat::R8G8B8A8SI: return VK_FORMAT_R8G8B8A8_SINT;
		case EFormat::R8G8B8X8: return UndefinedFormat;
		case EFormat::B8G8R8A8: return VK_FORMAT_B8G8R8A8_UINT;
		case EFormat::B8G8R8A8UF: return VK_FORMAT_B8G8R8A8_UNORM;
		case EFormat::B8G8R8A8UFsRGB: return VK_FORMAT_B8G8R8A8_SRGB;
		case EFormat::B8G8R8A8SF: return VK_FORMAT_B8G8R8A8_SNORM;
		case EFormat::B8G8R8X8: return /*UndefinedFormat*/VK_FORMAT_B8G8R8A8_UINT;
		case EFormat::B8G8R8X8UF: return /*UndefinedFormat*/VK_FORMAT_B8G8R8A8_UNORM;
		case EFormat::B8G8R8X8UFsRGB: return /*UndefinedFormat*/VK_FORMAT_B8G8R8A8_SRGB;
		//case EFormat::R10G10B10A2: return VK_FORMAT_A2R10G10B10_UINT_PACK32;
		//case EFormat::R10G10B10A2UF: return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
		//case EFormat::R10G10B10A2UI: return VK_FORMAT_A2R10G10B10_UINT_PACK32;
		case EFormat::R10G10B10A2: return VK_FORMAT_A2B10G10R10_UINT_PACK32;
		case EFormat::R10G10B10A2UF: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
		case EFormat::R10G10B10A2UI: return VK_FORMAT_A2B10G10R10_UINT_PACK32;
		case EFormat::R11G11B10F: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
		case EFormat::R16G16: return VK_FORMAT_R16G16_UINT;
		case EFormat::R16G16F: return VK_FORMAT_R16G16_SFLOAT;
		case EFormat::R16G16UF: return VK_FORMAT_R16G16_UNORM;
		case EFormat::R16G16SF: return VK_FORMAT_R16G16_SNORM;
		case EFormat::R16G16UI: return VK_FORMAT_R16G16_UINT;
		case EFormat::R16G16SI: return VK_FORMAT_R16G16_SINT;
		case EFormat::R16G16B16A16: return VK_FORMAT_R16G16B16A16_UINT;
		case EFormat::R16G16B16A16F: return VK_FORMAT_R16G16B16A16_SFLOAT;
		case EFormat::R16G16B16A16UF: return VK_FORMAT_R16G16B16A16_UNORM;
		case EFormat::R16G16B16A16SF: return VK_FORMAT_R16G16B16A16_SNORM;
		case EFormat::R16G16B16A16UI: return VK_FORMAT_R16G16B16A16_UINT;
		case EFormat::R16G16B16A16SI: return VK_FORMAT_R16G16B16A16_SINT;
		case EFormat::R32G32: return VK_FORMAT_R32G32_UINT;
		case EFormat::R32G32F: return VK_FORMAT_R32G32_SFLOAT;
		case EFormat::R32G32UI: return VK_FORMAT_R32G32_UINT;
		case EFormat::R32G32SI: return VK_FORMAT_R32G32_SINT;
		case EFormat::R32G32B32: return VK_FORMAT_R32G32B32_UINT;
		case EFormat::R32G32B32F: return VK_FORMAT_R32G32B32_SFLOAT;
		case EFormat::R32G32B32UI: return VK_FORMAT_R32G32B32_UINT;
		case EFormat::R32G32B32SI: return VK_FORMAT_R32G32B32_SINT;
		case EFormat::R32G32B32A32: return VK_FORMAT_R32G32B32A32_UINT;
		case EFormat::R32G32B32A32F: return VK_FORMAT_R32G32B32A32_SFLOAT;
		case EFormat::R32G32B32A32UF: return UndefinedFormat;
		case EFormat::R32G32B32A32SF: return UndefinedFormat;
		case EFormat::R32G32B32A32UI: return VK_FORMAT_R32G32B32A32_UINT;
		case EFormat::R32G32B32A32SI: return VK_FORMAT_R32G32B32A32_SINT;
		case EFormat::A8: return VK_FORMAT_A8_UNORM_KHR;
		case EFormat::A8UF: return VK_FORMAT_A8_UNORM_KHR;
		case EFormat::R8: return VK_FORMAT_R8_UINT;
		case EFormat::R8UI: return VK_FORMAT_R8_UINT;
		case EFormat::R8SI: return VK_FORMAT_R8_SINT;
		case EFormat::R8UF: return VK_FORMAT_R8_UNORM;
		case EFormat::R8SF: return VK_FORMAT_R8_SNORM;
		case EFormat::R16: return VK_FORMAT_R16_UINT;
		case EFormat::R16F: return VK_FORMAT_R16_SFLOAT;
		case EFormat::R16UI: return VK_FORMAT_R16_UINT;
		case EFormat::R16SI: return VK_FORMAT_R16_SINT;
		case EFormat::R16UF: return VK_FORMAT_R16_UNORM;
		case EFormat::R16SF: return VK_FORMAT_R16_SNORM;
		case EFormat::R24G8: return UndefinedFormat;
		case EFormat::R24X8: return UndefinedFormat;
		case EFormat::R32: return VK_FORMAT_R32_UINT;
		case EFormat::R32F: return VK_FORMAT_R32_SFLOAT;
		case EFormat::R32UI: return VK_FORMAT_R32_UINT;
		case EFormat::R32SI: return VK_FORMAT_R32_SINT;

		case EFormat::D16UF: return VK_FORMAT_D16_UNORM;
		case EFormat::D32F: return VK_FORMAT_D32_SFLOAT;
		case EFormat::D24UF_S8UI: return VK_FORMAT_D24_UNORM_S8_UINT;
		case EFormat::R3G3B2: return VK_FORMAT_D16_UNORM;
		case EFormat::R8G8: return VK_FORMAT_R8G8_UINT;
		case EFormat::R8G8UF: return VK_FORMAT_R8G8_UNORM;
		case EFormat::R8G8SF: return VK_FORMAT_R8G8_SNORM;
		case EFormat::R8G8UI: return VK_FORMAT_R8G8_UINT;
		case EFormat::R8G8SI: return VK_FORMAT_R8G8_SINT;
		case EFormat::R5G6B5: return VK_FORMAT_R5G6B5_UNORM_PACK16;
		case EFormat::R5G6B5UF: return VK_FORMAT_R5G6B5_UNORM_PACK16;
		case EFormat::R5G5B5A1: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
		case EFormat::R5G5B5A1UF: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
		case EFormat::B5G6R5: return VK_FORMAT_B5G6R5_UNORM_PACK16;
		case EFormat::B5G5R5A1: return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
		case EFormat::A1R5G5B5: return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
		case EFormat::A1B5G5R5: return UndefinedFormat;
		case EFormat::X1R5G5B5: return UndefinedFormat;
		case EFormat::X1B5G5R5: return UndefinedFormat;
		case EFormat::A4R4G4B4: return VK_FORMAT_A4R4G4B4_UNORM_PACK16;
		case EFormat::X4R4G4B4: return UndefinedFormat;
		case EFormat::A8R3G3B2: return UndefinedFormat;
		case EFormat::B4G4R4A4: return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
		case EFormat::R8G8B8: return VK_FORMAT_R8G8B8_UINT;
		case EFormat::B8G8R8: return VK_FORMAT_B8G8R8_UINT;
		case EFormat::A8R5G6B5: return UndefinedFormat;
		case EFormat::A8B5G6R5: return UndefinedFormat;
		case EFormat::A8X1R5G5B5: return UndefinedFormat;
		case EFormat::A8X1B5G5R5: return UndefinedFormat;
		case EFormat::A2R10G10B10: return VK_FORMAT_A2R10G10B10_UINT_PACK32;
		case EFormat::A2B10G10R10: return VK_FORMAT_A2B10G10R10_UINT_PACK32;

		case EFormat::BC1: return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
		case EFormat::BC1UF: return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
		case EFormat::BC1UFsRGB: return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
		case EFormat::BC1SF: return UndefinedFormat;
		case EFormat::BC2: return VK_FORMAT_BC2_UNORM_BLOCK;
		case EFormat::BC2UF: return VK_FORMAT_BC2_UNORM_BLOCK;
		case EFormat::BC2UFsRGB: return VK_FORMAT_BC2_SRGB_BLOCK;
		case EFormat::BC3: return VK_FORMAT_BC3_UNORM_BLOCK;
		case EFormat::BC3UF: return VK_FORMAT_BC3_UNORM_BLOCK;
		case EFormat::BC3UFsRGB: return VK_FORMAT_BC3_SRGB_BLOCK;
		case EFormat::BC4: return VK_FORMAT_BC4_UNORM_BLOCK;
		case EFormat::BC4UF: return VK_FORMAT_BC4_UNORM_BLOCK;
		case EFormat::BC4SF: return VK_FORMAT_BC4_SNORM_BLOCK;
		case EFormat::BC5: return VK_FORMAT_BC5_UNORM_BLOCK;
		case EFormat::BC5UF: return VK_FORMAT_BC5_UNORM_BLOCK;
		case EFormat::BC5SF: return VK_FORMAT_BC5_SNORM_BLOCK;
		case EFormat::BC6H: return UndefinedFormat;
		case EFormat::BC6H_UF16: return VK_FORMAT_BC6H_UFLOAT_BLOCK;
		case EFormat::BC6H_SF16: return VK_FORMAT_BC6H_SFLOAT_BLOCK;
		case EFormat::BC7: return VK_FORMAT_BC7_UNORM_BLOCK;
		case EFormat::BC7UF: return VK_FORMAT_BC7_UNORM_BLOCK;
		case EFormat::BC7UFsRGB: return VK_FORMAT_BC7_SRGB_BLOCK;

		case EFormat::INDEX4_A8R8G8B8: return UndefinedFormat;
		case EFormat::INDEX8_A8R8G8B8: return UndefinedFormat;
		//case EFormat::RGB: return UndefinedFormat;
		//case EFormat::BGR: return UndefinedFormat;
		//case EFormat::RGBA: return UndefinedFormat;
		//case EFormat::RGBAUF: return UndefinedFormat;
		//case EFormat::RGBASF: return UndefinedFormat;
		//case EFormat::RGBAUFsRGB: return UndefinedFormat;
		//case EFormat::RGBAUI: return UndefinedFormat;
		//case EFormat::RGBASI: return UndefinedFormat;
		//case EFormat::RGBX: return UndefinedFormat;
		//case EFormat::BGRA: return UndefinedFormat;
		//case EFormat::BGRAUF: return UndefinedFormat;
		//case EFormat::BGRAUFsRGB: return UndefinedFormat;
		//case EFormat::BGRASF: return UndefinedFormat;
		//case EFormat::BGRX: return UndefinedFormat;
		//case EFormat::BGRXUFsRGB: return UndefinedFormat;
		case EFormat::Short1: return VK_FORMAT_R16_SINT;
		case EFormat::Short2: return VK_FORMAT_R16G16_SINT;
		case EFormat::Short3: return VK_FORMAT_R16G16B16_SINT;
		case EFormat::Short4: return VK_FORMAT_R16G16B16A16_SINT;
		case EFormat::UShort1: return VK_FORMAT_R16_UINT;
		case EFormat::UShort2: return VK_FORMAT_R16G16_UINT;
		case EFormat::UShort3: return VK_FORMAT_R16G16B16_UINT;
		case EFormat::UShort4: return VK_FORMAT_R16G16B16A16_UINT;
		case EFormat::Int1: return VK_FORMAT_R32_SINT;
		case EFormat::Int2: return VK_FORMAT_R32G32_SINT;
		case EFormat::Int3: return VK_FORMAT_R32G32B32_SINT;
		case EFormat::Int4: return VK_FORMAT_R32G32B32A32_SINT;
		case EFormat::UInt1: return VK_FORMAT_R32_UINT;
		case EFormat::UInt2: return VK_FORMAT_R32_UINT;
		case EFormat::UInt3: return VK_FORMAT_R32_UINT;
		case EFormat::UInt4: return VK_FORMAT_R32_UINT;
		case EFormat::Half1: return VK_FORMAT_R16_SFLOAT;
		case EFormat::Half2: return VK_FORMAT_R16G16_SFLOAT;
		case EFormat::Half3: return VK_FORMAT_R16G16B16_SFLOAT;
		case EFormat::Half4: return VK_FORMAT_R16G16B16A16_SFLOAT;
		case EFormat::Float1: return VK_FORMAT_R32_SFLOAT;
		case EFormat::Float2: return VK_FORMAT_R32G32_SFLOAT;
		case EFormat::Float3: return VK_FORMAT_R32G32B32_SFLOAT;
		case EFormat::Float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		case EFormat::Struct: return UndefinedFormat;
		case EFormat::GRAY1: return UndefinedFormat;
		case EFormat::GRAY2: return UndefinedFormat;
		case EFormat::GRAY4: return UndefinedFormat;
		case EFormat::GRAY8: return UndefinedFormat;
		case EFormat::I420: return UndefinedFormat;
		case EFormat::I422: return UndefinedFormat;
		case EFormat::I444: return UndefinedFormat;
		case EFormat::NV11: return UndefinedFormat;
		case EFormat::NV12: return UndefinedFormat;
		case EFormat::NV21: return UndefinedFormat;
		case EFormat::YU12: return UndefinedFormat;
		case EFormat::YUV2: return UndefinedFormat;
		case EFormat::YUY2: return UndefinedFormat;
		case EFormat::YUYV: return UndefinedFormat;
		case EFormat::YV12: return UndefinedFormat;
		case EFormat::YVYU: return UndefinedFormat;
		case EFormat::UYUV: return UndefinedFormat;
		case EFormat::UYVY: return UndefinedFormat;
		case EFormat::VYVU: return UndefinedFormat;
		case EFormat::VUVY: return UndefinedFormat;
		case EFormat::CMYK: return UndefinedFormat;
		case EFormat::YCCK: return UndefinedFormat;
		case EFormat::Y420: return UndefinedFormat;
		case EFormat::P010: return UndefinedFormat;
		case EFormat::P016: return UndefinedFormat;
		case EFormat::FOURCC_Y8: return UndefinedFormat;
		case EFormat::FOURCC_RGB: return UndefinedFormat;
		case EFormat::FOURCC_BGR: return UndefinedFormat;
		case EFormat::FOURCC_ARGB: return UndefinedFormat;
		case EFormat::FOURCC_ABGR: return UndefinedFormat;
		case EFormat::FOURCC_BGRA: return UndefinedFormat;
		case EFormat::FOURCC_RGBA: return UndefinedFormat;
		case EFormat::FOURCC_XRGB: return UndefinedFormat;
		case EFormat::FOURCC_XBGR: return UndefinedFormat;
		case EFormat::FOURCC_BGRX: return UndefinedFormat;
		case EFormat::FOURCC_RGBX: return UndefinedFormat;
		//case EFormat::FOURCC_I420: return UndefinedFormat;
		//case EFormat::FOURCC_I422: return UndefinedFormat;
		//case EFormat::FOURCC_I444: return UndefinedFormat;
		//case EFormat::FOURCC_I400: return UndefinedFormat;
		//case EFormat::FOURCC_NV21: return UndefinedFormat;
		//case EFormat::FOURCC_NV12: return UndefinedFormat;
		//case EFormat::FOURCC_YUY2: return UndefinedFormat;
		//case EFormat::FOURCC_UYVY: return UndefinedFormat;
		case EFormat::FOURCC_M420: return UndefinedFormat;
		case EFormat::FOURCC_AR30: return UndefinedFormat;
		case EFormat::FOURCC_AB30: return UndefinedFormat;
		case EFormat::FOURCC_RGBP: return UndefinedFormat;
		case EFormat::FOURCC_RGB0: return UndefinedFormat;
		case EFormat::FOURCC_R444: return UndefinedFormat;
		case EFormat::FOURCC_MJPG: return UndefinedFormat;
		//case EFormat::FOURCC_YV12: return UndefinedFormat;
		case EFormat::FOURCC_YV16: return UndefinedFormat;
		case EFormat::FOURCC_YV24: return UndefinedFormat;
		//case EFormat::FOURCC_YU12: return UndefinedFormat;
		case EFormat::FOURCC_YU16: return UndefinedFormat;
		case EFormat::FOURCC_YU24: return UndefinedFormat;
		case EFormat::FOURCC_J420: return UndefinedFormat;
		case EFormat::FOURCC_J400: return UndefinedFormat;
		case EFormat::FOURCC_H420: return UndefinedFormat;
		case EFormat::FOURCC_H422: return UndefinedFormat;
		case EFormat::FOURCC_IYUV: return UndefinedFormat;
		//case EFormat::FOURCC_YUYV: return UndefinedFormat;
		case EFormat::FOURCC_YUVS: return UndefinedFormat;
		case EFormat::FOURCC_HDYC: return UndefinedFormat;
		case EFormat::FOURCC_JPEG: return UndefinedFormat;
		case EFormat::FOURCC_DMB1: return UndefinedFormat;
		case EFormat::FOURCC_BA81: return UndefinedFormat;
		case EFormat::FOURCC_BGB3: return UndefinedFormat;
		case EFormat::FOURCC_BGR3: return UndefinedFormat;
		case EFormat::FOURCC_CM32: return UndefinedFormat;
		case EFormat::FOURCC_CM24: return UndefinedFormat;
		case EFormat::FOURCC_L555: return UndefinedFormat;
		case EFormat::FOURCC_L565: return UndefinedFormat;
		case EFormat::FOURCC_L551: return UndefinedFormat;
		case EFormat::PNG: return UndefinedFormat;
		//case EFormat::JPEG: return UndefinedFormat;
		//case EFormat::MJPG: return UndefinedFormat;
		case EFormat::H264: return UndefinedFormat;
		case EFormat::H265: return UndefinedFormat;
		case EFormat::HEVC: return UndefinedFormat;
		case EFormat::X264: return UndefinedFormat;
		case EFormat::RAW: return UndefinedFormat;
		default: return UndefinedFormat;
		}
	}

	EFormat ToFormat(VkFormat Format)
	{
		constexpr EFormat InvalidFormat = EFormat::None;
		switch (Format)
		{
		case VK_FORMAT_R5G6B5_UNORM_PACK16: return EFormat::R5G6B5;
		case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return EFormat::R5G5B5A1;
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return EFormat::B5G5R5A1;
		case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return EFormat::A1R5G5B5;
		case VK_FORMAT_R8_UNORM: return EFormat::R8UF;
		case VK_FORMAT_R8_SNORM: return EFormat::R8SF;
		case VK_FORMAT_R8_USCALED: return EFormat::R8UF;
		case VK_FORMAT_R8_SSCALED: return EFormat::R8SF;
		case VK_FORMAT_R8_UINT: return EFormat::R8UI;
		case VK_FORMAT_R8_SINT: return EFormat::R8SI;
		case VK_FORMAT_R8_SRGB: return EFormat::R8UI;
		case VK_FORMAT_R8G8_UNORM: return EFormat::R8G8UF;
		case VK_FORMAT_R8G8_SNORM: return EFormat::R8G8SF;
		case VK_FORMAT_R8G8_USCALED: return EFormat::R8G8UF;
		case VK_FORMAT_R8G8_SSCALED: return EFormat::R8G8SF;
		case VK_FORMAT_R8G8_UINT: return EFormat::R8G8UI;
		case VK_FORMAT_R8G8_SINT: return EFormat::R8G8SI;
		case VK_FORMAT_R8G8_SRGB: return EFormat::R8G8UI;
		case VK_FORMAT_R8G8B8_UNORM: return EFormat::R8G8B8;
		case VK_FORMAT_R8G8B8_SNORM: return EFormat::R8G8B8;
		case VK_FORMAT_R8G8B8_USCALED: return EFormat::R8G8B8;
		case VK_FORMAT_R8G8B8_SSCALED: return EFormat::R8G8B8;
		case VK_FORMAT_R8G8B8_UINT: return EFormat::R8G8B8;
		case VK_FORMAT_R8G8B8_SINT: return EFormat::R8G8B8;
		case VK_FORMAT_R8G8B8_SRGB: return EFormat::R8G8B8;

		case VK_FORMAT_B8G8R8_UNORM: return EFormat::B8G8R8;
		case VK_FORMAT_B8G8R8_SNORM: return EFormat::B8G8R8;
		case VK_FORMAT_B8G8R8_USCALED: return EFormat::B8G8R8;
		case VK_FORMAT_B8G8R8_SSCALED: return EFormat::B8G8R8;
		case VK_FORMAT_B8G8R8_UINT: return EFormat::B8G8R8;
		case VK_FORMAT_B8G8R8_SINT: return EFormat::B8G8R8;
		case VK_FORMAT_B8G8R8_SRGB: return EFormat::B8G8R8;

		case VK_FORMAT_R8G8B8A8_UNORM: return EFormat::R8G8B8A8UF;
		case VK_FORMAT_R8G8B8A8_SNORM: return EFormat::R8G8B8A8SF;
		case VK_FORMAT_R8G8B8A8_USCALED: return EFormat::R8G8B8A8UF;
		case VK_FORMAT_R8G8B8A8_SSCALED: return EFormat::R8G8B8A8SF;
			break;
		case VK_FORMAT_R8G8B8A8_UINT: return EFormat::R8G8B8A8UI;
		case VK_FORMAT_R8G8B8A8_SINT: return EFormat::R8G8B8A8SI;
		case VK_FORMAT_R8G8B8A8_SRGB: return EFormat::R8G8B8A8UFsRGB;

		case VK_FORMAT_B8G8R8A8_UNORM: return EFormat::B8G8R8A8UF;
		case VK_FORMAT_B8G8R8A8_SNORM: return EFormat::B8G8R8A8SF;
		case VK_FORMAT_B8G8R8A8_USCALED: return EFormat::B8G8R8A8UF;
		case VK_FORMAT_B8G8R8A8_SSCALED: return EFormat::B8G8R8A8SF;
		case VK_FORMAT_B8G8R8A8_UINT: return InvalidFormat;
		case VK_FORMAT_B8G8R8A8_SINT: return InvalidFormat;

		case VK_FORMAT_B8G8R8A8_SRGB: return EFormat::B8G8R8A8UFsRGB;

		case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return InvalidFormat;
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return InvalidFormat;
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return InvalidFormat;
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return InvalidFormat;
		case VK_FORMAT_A8B8G8R8_UINT_PACK32: return InvalidFormat;
		case VK_FORMAT_A8B8G8R8_SINT_PACK32: return InvalidFormat;
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return InvalidFormat;

		case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return InvalidFormat;
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return InvalidFormat;
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return InvalidFormat;
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return InvalidFormat;
		case VK_FORMAT_A2R10G10B10_UINT_PACK32: return InvalidFormat;
		case VK_FORMAT_A2R10G10B10_SINT_PACK32: return InvalidFormat;
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return InvalidFormat;
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return InvalidFormat;
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return InvalidFormat;
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return InvalidFormat;
		case VK_FORMAT_A2B10G10R10_UINT_PACK32: return InvalidFormat;
		case VK_FORMAT_A2B10G10R10_SINT_PACK32: return InvalidFormat;

		case VK_FORMAT_R16_UNORM: return EFormat::R16UF;
		case VK_FORMAT_R16_SNORM: return EFormat::R16SF;
		case VK_FORMAT_R16_USCALED: return EFormat::R16UF;
		case VK_FORMAT_R16_SSCALED: return EFormat::R16SF;
		case VK_FORMAT_R16_UINT: return EFormat::R16UI;
		case VK_FORMAT_R16_SINT: return EFormat::R16SI;
		case VK_FORMAT_R16_SFLOAT: return EFormat::R16F;

		case VK_FORMAT_R16G16_UNORM: return EFormat::R16G16UF;
		case VK_FORMAT_R16G16_SNORM: return EFormat::R16G16SF;
		case VK_FORMAT_R16G16_USCALED: return EFormat::R16G16UF;
		case VK_FORMAT_R16G16_SSCALED: return EFormat::R16G16SF;
		case VK_FORMAT_R16G16_UINT: return EFormat::R16G16UI;
		case VK_FORMAT_R16G16_SINT: return EFormat::R16G16SI;
		case VK_FORMAT_R16G16_SFLOAT: return EFormat::R16G16F;

		case VK_FORMAT_R16G16B16A16_UNORM: return EFormat::R16G16B16A16UF;
		case VK_FORMAT_R16G16B16A16_SNORM: return EFormat::R16G16B16A16SF;
		case VK_FORMAT_R16G16B16A16_USCALED: return EFormat::R16G16B16A16UF;
		case VK_FORMAT_R16G16B16A16_SSCALED: return EFormat::R16G16B16A16SF;
		case VK_FORMAT_R16G16B16A16_UINT: return EFormat::R16G16B16A16UI;
		case VK_FORMAT_R16G16B16A16_SINT: return EFormat::R16G16B16A16SI;
		case VK_FORMAT_R16G16B16A16_SFLOAT: return EFormat::R16G16B16A16F;

		case VK_FORMAT_R32_UINT: return EFormat::R32UI;
		case VK_FORMAT_R32_SINT: return EFormat::R32SI;
		case VK_FORMAT_R32_SFLOAT: return EFormat::R32F;

		case VK_FORMAT_R32G32_UINT: return EFormat::R32G32UI;
		case VK_FORMAT_R32G32_SINT: return EFormat::R32G32SI;
		case VK_FORMAT_R32G32_SFLOAT: return EFormat::R32G32F;

		case VK_FORMAT_R32G32B32_UINT: return EFormat::R32G32B32UI;
		case VK_FORMAT_R32G32B32_SINT: return EFormat::R32G32B32SI;
		case VK_FORMAT_R32G32B32_SFLOAT: return EFormat::R32G32B32F;

		case VK_FORMAT_R32G32B32A32_UINT: return EFormat::R32G32B32A32UI;
		case VK_FORMAT_R32G32B32A32_SINT: return EFormat::R32G32B32A32SI;
		case VK_FORMAT_R32G32B32A32_SFLOAT: return EFormat::R32G32B32A32F;

		case VK_FORMAT_D16_UNORM: return EFormat::D16UF;
		case VK_FORMAT_X8_D24_UNORM_PACK32: return EFormat::D24UF_S8UI;
		case VK_FORMAT_D32_SFLOAT: return EFormat::D32F;
		case VK_FORMAT_S8_UINT: return InvalidFormat;
		case VK_FORMAT_D16_UNORM_S8_UINT: return InvalidFormat;
		case VK_FORMAT_D24_UNORM_S8_UINT: return EFormat::D24UF_S8UI;
		case VK_FORMAT_D32_SFLOAT_S8_UINT: return InvalidFormat;

		case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return EFormat::BC1UF;
		case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return EFormat::BC1UFsRGB;
		case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return EFormat::BC1UF;
		case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return EFormat::BC1UFsRGB;

		case VK_FORMAT_BC2_UNORM_BLOCK: return EFormat::BC2UF;
		case VK_FORMAT_BC2_SRGB_BLOCK: return EFormat::BC2UFsRGB;

		case VK_FORMAT_BC3_UNORM_BLOCK: return EFormat::BC3UF;
		case VK_FORMAT_BC3_SRGB_BLOCK: return EFormat::BC3UFsRGB;

		case VK_FORMAT_BC4_UNORM_BLOCK: return EFormat::BC4UF;
		case VK_FORMAT_BC4_SNORM_BLOCK: return EFormat::BC4SF;

		case VK_FORMAT_BC5_UNORM_BLOCK: return EFormat::BC5UF;
		case VK_FORMAT_BC5_SNORM_BLOCK: return EFormat::BC5SF;

		case VK_FORMAT_BC6H_UFLOAT_BLOCK: return EFormat::BC6H_UF16;
		case VK_FORMAT_BC6H_SFLOAT_BLOCK: return EFormat::BC6H_SF16;

		case VK_FORMAT_BC7_UNORM_BLOCK: return EFormat::BC7UF;
		case VK_FORMAT_BC7_SRGB_BLOCK: return EFormat::BC7UFsRGB;

		case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return EFormat::ASTC4x4;
		case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return EFormat::ASTC4x4sRGB;

		case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return EFormat::ASTC5x4;
		case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return EFormat::ASTC5x4sRGB;

		case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return EFormat::ASTC5x5;
		case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return EFormat::ASTC5x5sRGB;

		case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return EFormat::ASTC6x5;
		case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return EFormat::ASTC6x5sRGB;

		case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return EFormat::ASTC6x6;
		case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return EFormat::ASTC6x6sRGB;

		case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return EFormat::ASTC8x5;
		case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return EFormat::ASTC8x5sRGB;

		case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return EFormat::ASTC8x6;
		case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return EFormat::ASTC8x6sRGB;

		case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
		case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
		case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
		case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
		case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
		case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
		case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
		case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
		case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
		case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
		case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
		case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
		case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
		case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
			return InvalidFormat;

		case VK_FORMAT_A4R4G4B4_UNORM_PACK16: return EFormat::A4R4G4B4;
		case VK_FORMAT_A4B4G4R4_UNORM_PACK16: return InvalidFormat;

		case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK:
		case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK:
			return InvalidFormat;

		case VK_FORMAT_A1B5G5R5_UNORM_PACK16: return EFormat::A1B5G5R5;
		case VK_FORMAT_A8_UNORM: return EFormat::A8UF;

		default:
			return InvalidFormat;
		}
	}

	VkFormat FromFormatSRV(EFormat Format)
	{
		switch (Format)
		{
		case EFormat::R8G8B8A8: return FromFormat(EFormat::R8G8B8A8UF);
		case EFormat::R8G8B8X8: return FromFormat(EFormat::R8G8B8A8UF);
		case EFormat::B8G8R8A8: return FromFormat(EFormat::B8G8R8A8UF);
		case EFormat::B8G8R8X8: return FromFormat(EFormat::B8G8R8X8UF);
		case EFormat::R10G10B10A2: return FromFormat(EFormat::R10G10B10A2UF);
		case EFormat::R16G16: return FromFormat(EFormat::R16G16UF);
		case EFormat::R16G16B16A16: return FromFormat(EFormat::R16G16B16A16UF);
		case EFormat::R32G32: return FromFormat(EFormat::R32G32F);
		case EFormat::R32G32B32: return FromFormat(EFormat::R32G32B32F);
		case EFormat::R32G32B32A32: return FromFormat(EFormat::R32G32B32A32F);
		case EFormat::A8: return FromFormat(EFormat::A8UF);
		case EFormat::R8: return FromFormat(EFormat::R8UF);
		case EFormat::R16: return FromFormat(EFormat::R16UF);
		case EFormat::R24G8: return FromFormat(EFormat::R16UF);
		case EFormat::R32: return FromFormat(EFormat::R32F);
		case EFormat::BC1: return FromFormat(EFormat::BC1UF);
		case EFormat::BC2: return FromFormat(EFormat::BC2UF);
		case EFormat::BC3: return FromFormat(EFormat::BC3UF);
		case EFormat::BC4: return FromFormat(EFormat::BC4UF);
		case EFormat::BC5: return FromFormat(EFormat::BC5UF);
		case EFormat::BC6H: return FromFormat(EFormat::BC6H_UF16);
		case EFormat::BC7: return FromFormat(EFormat::BC7UF);
		case EFormat::R8G8: return FromFormat(EFormat::R8G8UF);
		case EFormat::R5G6B5: return FromFormat(EFormat::R5G6B5UF);

		case EFormat::D16UF: return FromFormat(EFormat::R16UF);
		case EFormat::D24UF_S8UI: return FromFormat(EFormat::R24G8);
		case EFormat::D32F: return FromFormat(EFormat::R32F);
		default: return FromFormat(Format);
		}
	}

	VkPresentModeKHR FromPresentMode(EPresentMode PresentMode)
	{
		switch (PresentMode)
		{
		default:
		case EPresentMode::Discard: return VK_PRESENT_MODE_IMMEDIATE_KHR;
		case EPresentMode::Sequential: return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		case EPresentMode::FlipSequential: return VK_PRESENT_MODE_FIFO_KHR;
		case EPresentMode::FlipDiscard: return VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}
}
