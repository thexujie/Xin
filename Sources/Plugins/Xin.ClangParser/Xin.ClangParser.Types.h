#pragma once

#include "Xin.Base/Xin.Base.h"

using namespace Xin;

#if XIN_LIB_DYNAMIC
#if _Xin_ClangParser_Module_
#define REFLECTION_API XIN_EXPORT
#else
#define REFLECTION_API XIN_IMPORT
#endif
#else
#define REFLECTION_API
#endif

namespace Xin
{
	class FReflectSettings
	{
	public:
		FString LineEnd = u8"\r\n"V;
		FString TabChar = u8"	"V;

		FString ProjectRoot;
		FString ModuleRoot;

		FString PCHIncludeFile;

		bool ForceGenerate = false;
	};
}
