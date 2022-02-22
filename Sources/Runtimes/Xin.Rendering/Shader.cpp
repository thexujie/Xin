#include "PCH.h"
#include "Shader.h"

namespace Xin::Rendering
{
	IShaderSource::IShaderSource(EShaderStage ShaderStage, FStringV FilePath, FStringV EntryName)
		: ShaderStage(ShaderStage), FilePath(FilePath), EntryName(EntryName)
	{
		
	}
}
