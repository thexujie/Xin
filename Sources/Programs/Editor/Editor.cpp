#include "PCH.h"
#include "Xin.MeshLoader/MeshLoader.h"
#include "Xin.ImageCodec.FreeImage/ImageCodec.FreeImage.h"

using namespace Xin;
using namespace Xin::Engine;

const FStringView DefaultMaterialShader = u8"Engine/Shaders/Default.hlsl"S;
const FStringView DiffuseTexture = u8"DiffuseTexture"S;
class FLocalAsset
{
public:
	FLocalAsset(FStringView FilePath) : FilePath(FilePath) { }

	FStream & Read(FJsonObject & Json)
	{
		FFileStream XxxxStream { FilePath, EFileModes::Input };
		XxxxStream >> Json;

		Stream.Close();
		Stream.Open(FilePath + u8".data"S, EFileModes::Input);
		return Stream;
	}

	FStream & Write(const FJsonObject & Json)
	{
		FDirectory::EnsureExists(FPath::GetDirectory(FilePath));

		FFileStream XxxxStream { FilePath, EFileModes::Output };
		XxxxStream << Json;

		Stream.Close();
		Stream.Open(FilePath + u8".data"S, EFileModes::Output);
		return Stream;
	}

private:
	FStringView FilePath;
	FFileStream Stream;
};

EError ImportTexture(FStringView Source, FStringView Dest)
{
	FLocalAsset LocalAsset { Dest };
	IImageCodec * ImageCodec = GApplication.GetService<IImageCodec>();
	if (!ImageCodec)
		return EError::False;

	IImageDataRef ImageData = nullptr;
	if (Source.EndsWith(u8".tga"S))
	{
		FString SourceDDSFilePath = FPath::Combine(FPath::GetDirectory(Source), FPath::GetBaseName(Source) + u8".dds"S);
		if (FFile::Exists(SourceDDSFilePath))
			ImageData = ImageCodec->DecodeImage(SourceDDSFilePath);
	}
	if (ImageData == nullptr)
		ImageData = ImageCodec->DecodeImage(Source);

	if (!ImageData)
		return EError::False;

	FTextureData TextureData(ImageData.Ref());

	FJsonObject OutputJson;
	OutputJson[u8"Version"S].Value = u8"1.0"S;

	FStream & OutputStream = LocalAsset.Write(OutputJson);
	FAssetHeader AssetHeader = { AssetMagic, AssetVersion, EAssetType::Texture };
	OutputStream << AssetHeader;
	OutputStream << TextureData;
	LogInfo(u8"ImportTexture: {0}", Source);

	return EError::OK;
}

EError ImportMaterial(FStringView Shader, FStringView Dest, TView<TPair<FString, FString>> MaterialParameters, TView<TPair<FString, FString>> Defines = {})
{
	FJsonObject OutputJson;
	OutputJson[u8"Version"S].Value = u8"1.0"S;
	OutputJson[u8"Shader"S].Value = Shader;

	OutputJson[u8"Parameters"S].Value = MakeRefer<FJsonObject>();

	FJsonObject & Parameters = OutputJson[u8"Parameters"S].Value.Get<FJsonObject &>();
	for (const auto & Pair : MaterialParameters)
		Parameters[Pair.First].Value = Pair.Second;

	if (Defines.Size)
	{
		OutputJson[u8"Defines"S].Value = MakeRefer<FJsonObject>();
		FJsonObject & DefinesObject = OutputJson[u8"Defines"S].Value.Get<FJsonObject &>();
		for (const auto & Pair : Defines)
			DefinesObject[Pair.First].Value = Pair.Second;
	}

	FFileStream XxxxStream { Dest, EFileModes::Output };
	XxxxStream << OutputJson;
	return EError::OK;
}

#include "H3DModel.h"

EError ImportH3DModel(FStringView Source, FStringView DestFolderPath)
{
	if (!FDirectory::EnsureExists(DestFolderPath))
		return EError::Failed;

	FString ModelName = FPath::GetBaseName(Source);
	FString ModelDirectoryPath= FPath::GetDirectory(Source);
	H3D::FH3DModel H3DModel;
	H3DModel.Load(Source);

	FString AssetRelativePath = FPath::GetRelativePath(u8"../../Contents"S, DestFolderPath);

	TList<FString> MaterialAssetPaths(H3DModel.Materials.Size);
	// Materials
	{
		for (size_t MaterialIndex = 0; MaterialIndex < H3DModel.Materials.Size; ++MaterialIndex)
		{
			const H3D::FH3DMaterial & H3DMaterial = H3DModel.Materials[MaterialIndex];
			if (!H3DMaterial.TextureDiffusePath[0])
				continue;

			FStringView MaterialAssetName = H3DMaterial.Name;
			FString MaterialAssetPath = FPath::Combine(DestFolderPath, MaterialAssetName + u8".xmtl"S);
			FString XmtlAssetPath = FPath::GetRelativePath(u8"../../Contents"S, MaterialAssetPath);

			FStringView DiffuseFileName = H3DMaterial.TextureDiffusePath;
			FString DiffuseBaseName = FPath::GetBaseName(H3DMaterial.TextureDiffusePath);
			TList<TPair<FString, FString>> Parameters;
			TList<TPair<FString, FString>> Defines;
			// Diffuse
			{
				FString SourceFileName = DiffuseBaseName + u8".dds";
				FString TextureFileName = DiffuseBaseName + u8"_D.dds";
				FString XtexFileName = DiffuseBaseName + u8"_D.xtex";

				FString SourceFilePath = FPath::Combine(ModelDirectoryPath, SourceFileName);
				FString BackupFilePath = FPath::Combine(DestFolderPath, TextureFileName);

				//FFile::Copy(SourceFilePath, BackupFilePath);
				ImportTexture(SourceFilePath, FPath::Combine(DestFolderPath, XtexFileName));

				Parameters.Add({ u8"DiffuseTexture"S, AssetsPrefix + FPath::Combine(AssetRelativePath, XtexFileName) });
			}
			// Normal
			if (FString SourceFilePath = FPath::Combine(ModelDirectoryPath, DiffuseBaseName + u8"_normal.dds"S); FFile::Exists(SourceFilePath))
			{
				FString TextureFileName = DiffuseBaseName + u8"_N.dds";
				FString XtexFileName = DiffuseBaseName + u8"_N.xtex";

				FString BackupFilePath = FPath::Combine(DestFolderPath, TextureFileName);

				//FFile::Copy(SourceFilePath, BackupFilePath);
				ImportTexture(SourceFilePath, FPath::Combine(DestFolderPath, XtexFileName));

				Parameters.Add({ u8"NormalTexture"S, AssetsPrefix + FPath::Combine(AssetRelativePath, XtexFileName) });
			}
			// Specular
			if (FString SourceFilePath = FPath::Combine(ModelDirectoryPath, DiffuseBaseName + u8"_specular.dds"S); FFile::Exists(SourceFilePath))
			{
				FString TextureFileName = DiffuseBaseName + u8"_S.dds";
				FString XtexFileName = DiffuseBaseName + u8"_S.xtex";

				FString BackupFilePath = FPath::Combine(DestFolderPath, TextureFileName);

				//FFile::Copy(SourceFilePath, BackupFilePath);
				ImportTexture(SourceFilePath, FPath::Combine(DestFolderPath, XtexFileName));

				Parameters.Add({ u8"SpecularTexture"S, AssetsPrefix + FPath::Combine(AssetRelativePath, XtexFileName) });
			}

			MaterialAssetPaths[MaterialIndex] = XmtlAssetPath;
			ImportMaterial(DefaultMaterialShader, MaterialAssetPath, Parameters, Defines);
		}
	}

	// StaticMesh
	{
		EMeshDataComponet MeshDataComponets = EMeshDataComponet::Position | EMeshDataComponet::Normal
		| EMeshDataComponet::Texcoord | EMeshDataComponet::Tangent | EMeshDataComponet::BiTangent;

		FString XmshFilePath = FPath::Combine(DestFolderPath, ModelName + u8".xmsh"S);
		Engine::FStaticMeshData StaticMeshData;
		StaticMeshData.Components = MeshDataComponets;

		StaticMeshData.Positions.Resize(H3DModel.Vertices.Size, EInitializeMode::None);
		StaticMeshData.Normals.Resize(H3DModel.Vertices.Size, EInitializeMode::None);
		StaticMeshData.Vertices.Resize(H3DModel.Vertices.Size, EInitializeMode::None);

		for (size_t VertexIndex = 0; VertexIndex < H3DModel.Vertices.Size; ++VertexIndex)
		{
			StaticMeshData.Positions[VertexIndex] = H3DModel.Vertices[VertexIndex].Position;
			StaticMeshData.Normals[VertexIndex] = H3DModel.Vertices[VertexIndex].Normal;
			StaticMeshData.Vertices[VertexIndex].Texcoord = H3DModel.Vertices[VertexIndex].Texcoord;
			StaticMeshData.Vertices[VertexIndex].Tangent = H3DModel.Vertices[VertexIndex].Tangent;
			StaticMeshData.Vertices[VertexIndex].BiTangent = H3DModel.Vertices[VertexIndex].BiTangent;
		}

		StaticMeshData.IndexFormat = EIndexFormat::Index16;
		StaticMeshData.Indices16 = H3DModel.Indices;

		StaticMeshData.Elements.Resize(H3DModel.Meshs.Size);


		FJsonObject OutputJson;
		OutputJson[u8"Version"S].Value = u8"1.0"S;
		OutputJson[u8"Elements"S].Value = MakeRefer<FJsonArray>();

		FJsonArray & Elements = OutputJson[u8"Elements"S].Value.Get<FJsonArray &>();

		for (size_t ElementIndex = 0; ElementIndex < H3DModel.Meshs.Size; ++ElementIndex)
		{
			const H3D::FH3DMesh & H3DMesh = H3DModel.Meshs[ElementIndex];

			StaticMeshData.Elements[ElementIndex].Name;
			if (H3DMesh.MaterialIndex != NullIndexU32)
			{
				const H3D::FH3DMaterial & H3DMaterial = H3DModel.Materials[H3DMesh.MaterialIndex];
				StaticMeshData.Elements[ElementIndex].MaterialName = H3DMaterial.Name;
			}
			StaticMeshData.Elements[ElementIndex].VertexOffset = H3DMesh.VertexDataByteOffset / sizeof(H3D::FH3DVertex);
			StaticMeshData.Elements[ElementIndex].VertexCount = H3DMesh.VertexCount;
			StaticMeshData.Elements[ElementIndex].IndexOffset = H3DMesh.IndexDataByteOffset / 2;
			StaticMeshData.Elements[ElementIndex].IndexCount = H3DMesh.IndexCount;

			FJsonObject & Element = Elements.Items.Add(MakeRefer<FJsonObject>()).Get<FJsonObject &>();
			Element[u8"Material"S].Value = AssetsPrefix + MaterialAssetPaths[H3DMesh.MaterialIndex];
		}


		FLocalAsset LocalAsset { XmshFilePath };
		FStream & OutputStream = LocalAsset.Write(OutputJson);
		FAssetHeader AssetHeader = { AssetMagic, AssetVersion, EAssetType::Mesh };
		OutputStream << AssetHeader;
		OutputStream << StaticMeshData;
	}

	// Textures
	{
		
	}

	return EError::OK;
}

EError ImportMesh(FStringView Source, FStringView Dest, TView<FStringView> Materials, const FMatrix & Transform = FMatrix::Identity)
{
	Xin::TSharedPtr<Engine::FStaticMeshData> StaticMesh = FMeshLoader::LoadStaticMeshFromFile(Source, Transform);
	if (!StaticMesh)
		return EError::False;

	FBoundingBox BoundingBox = StaticMesh->CalculateBoundingBox();
	FJsonObject OutputJson;
	OutputJson[u8"Version"S].Value = u8"1.0"S;
	OutputJson[u8"BoundingBox"S].Value = Format(u8"({0}, {1}, {2}), ({3}, {4}, {5})", 
		BoundingBox.MinPoint.X, BoundingBox.MinPoint.Y, BoundingBox.MinPoint.Z,
		BoundingBox.MaxPoint.X, BoundingBox.MaxPoint.Y, BoundingBox.MaxPoint.Z
	);
	OutputJson[u8"Elements"S].Value = MakeRefer<FJsonArray>();

	FJsonArray & Elements = OutputJson[u8"Elements"S].Value.Get<FJsonArray &>();
	for (size_t ElementIndex = 0; ElementIndex < Materials.Size; ++ElementIndex)
	{
		FJsonObject & Element = Elements.Items.Add(MakeRefer<FJsonObject>()).Get<FJsonObject &>();
		Element[u8"Material"S].Value = Materials[ElementIndex];
	}

	FLocalAsset LocalAsset { Dest };
	FStream & OutputStream = LocalAsset.Write(OutputJson);
	FAssetHeader AssetHeader = { AssetMagic, AssetVersion, EAssetType::Mesh };
	OutputStream << AssetHeader;
	OutputStream << StaticMesh.Ref();
	return EError::OK;
}

int _tmain(int argc, const char * args[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(399);
	auto FImageCodec_FreeImage = Xin::MakeRefer<Xin::FImageCodec_FreeImage>();
	Xin::GApplication.RegisterService<Xin::IImageCodec>(FImageCodec_FreeImage);

	ImportH3DModel(u8R"(D:\Projects\DirectX-Graphics-Samples\MiniEngine\ModelViewer\Sponza\sponza.h3d)", u8"../../Contents/Sponza"S);
	return 0;

	//ImportMesh(Str("../../.Data/Cube.fbx"), Str("../../Contents/Cube.xmsh"));
	{
		ImportTexture(u8"../../Data/Wolf_Body.jpg"S, u8"../../Contents/Wolf/Wolf_Body.xtex"S);
		ImportTexture(u8"../../Data/Wolf_Eyes.jpg"S, u8"../../Contents/Wolf/Wolf_Eyes.xtex"S);
		ImportTexture(u8"../../Data/Wolf_Fur.jpg"S, u8"../../Contents/Wolf/Wolf_Fur.xtex"S);

		//ImportMaterial(DefaultMaterialShader, u8"../../Contents/Wolf/Wolf_Body.xmtl"S, u8"assets://Wolf/Wolf_Body.xtex"S);
		//ImportMaterial(DefaultMaterialShader, u8"../../Contents/Wolf/Wolf_Eyes.xmtl"S, u8"assets://Wolf/Wolf_Eyes.xtex"S);
		//ImportMaterial(DefaultMaterialShader, u8"../../Contents/Wolf/Wolf_Fur.xmtl"S, u8"assets://Wolf/Wolf_Fur.xtex"S);

		ImportMesh(Str("../../Data/Wolf.fbx"), Str("../../Contents/Wolf/Wolf.xmsh"),
			{
				u8"assets://Wolf/Wolf_Body.xmtl"S,
				u8"assets://Wolf/Wolf_Eyes.xmtl"S,
				u8"assets://Wolf/Wolf_Fur.xmtl"S
			},
			FMatrix::Scale(10, 10, 10));
	}

	//ImportTexture(Str("../../Data/Cat.jpg"), Str("../Contents/Cat.xtex"));

	//ImportMaterial(Str("Contents/Shaders/Default.hlsl"), Str("../Contents/Default.xmtl"));
	//ImportMaterial(Str("Contents/Shaders/BaseColor.hlsl"), Str("../Contents/BaseColor.xmtl"));
	return 0;
}
