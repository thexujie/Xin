#pragma once

#include "Xin.Core/Xin.Core.h"
#include "Xin.RHI/Xin.RHI.h"

#if XIN_LIB_DYNAMIC
#if _Xin_ShaderCompiler_Module_
#define XIN_SHADERCOMILER_API XIN_EXPORT
#else
#define XIN_SHADERCOMILER_API
#endif
#else
#define XIN_SHADERCOMILER_API
#endif

namespace Xin::ShaderCompiler
{
	using namespace Xin::RHI;

#ifdef GetTargetProfile
#undef GetTargetProfile
#endif

#define GetTargetProfile(main, minor) \
	inline FStringV GetTargetProfile_##main##_##minor(EShaderStage ShaderStage)\
	{\
		switch (ShaderStage)\
		{\
		case EShaderStage::Vertex: return u8"vs_" #main "_" #minor ##V;\
		case EShaderStage::Hull: return u8"hs_" #main "_" #minor ##V;\
		case EShaderStage::Domain: return u8"ds_" #main "_" #minor ##V;\
		case EShaderStage::Geometry: return u8"gs_" #main "_" #minor ##V;\
		case EShaderStage::Pixel: return u8"ps_" #main "_" #minor ##V;\
		case EShaderStage::Compute: return u8"cs_" #main "_" #minor ##V;\
		default: return u8"unknown"V;\
		}\
	}

	GetTargetProfile(5, 0);
	GetTargetProfile(5, 1);
	GetTargetProfile(6, 0);
	GetTargetProfile(6, 1);
	GetTargetProfile(6, 2);
	GetTargetProfile(6, 3);
	GetTargetProfile(6, 4);
	GetTargetProfile(6, 5);

#undef GetTargetProfile
}
