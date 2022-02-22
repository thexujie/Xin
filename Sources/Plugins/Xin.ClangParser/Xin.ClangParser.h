#pragma once

#include "Xin.ClangParser.Types.h"

namespace Xin
{
	REFLECTION_API bool ReflectModule(FReflectSettings & ReflectSettings);
	REFLECTION_API bool GeneratePCHFile(FReflectSettings & ReflectSettings, FStringV PCHIncludeFilePath, FStringV PCHFileName);
	REFLECTION_API bool ReflectFile(FReflectSettings & ReflectSettings, FStringV FilePath);
}
