#pragma once

#include "FBX.Types.h"

namespace Xin::FBX
{
	XIN_FBX_API FFbxModel LoadStaticMeshFromFile(FStringV FileName, const FMatrix & Transform = FMatrix::Identity);
}