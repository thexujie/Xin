#include "PCH.h"
#include "Xin.FBX.h"
#include "FbxLoader.h"


namespace Xin::FBX
{
	FFbxModel LoadStaticMeshFromFile(FStringV FileName, const FMatrix & Transform)
	{
		FFbxLoadConfig LoadConfig = {};
		LoadConfig.Transform = Transform;
		FFbxLoader Loader(LoadConfig);
		return Loader.LoadMeshFromFile(FileName);
	}
}
