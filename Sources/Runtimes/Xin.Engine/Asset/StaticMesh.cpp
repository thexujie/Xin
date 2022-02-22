#include "PCH.h"
#include "StaticMesh.h"

#include "Engine.h"

namespace Xin::Engine
{
	IDataStream & operator <<(IDataStream & Stream, const FStaticMeshData & StaticMeshData)
	{
		Stream << StaticMeshData.Positions;
		Stream << StaticMeshData.Normals;
		Stream << StaticMeshData.Vertices;
		Stream << StaticMeshData.IndexFormat;
		switch (StaticMeshData.IndexFormat)
		{
		case EIndexFormat::Index16:
			Stream << StaticMeshData.Indices16;
			break;
		case EIndexFormat::Index32:
			Stream << StaticMeshData.Indices32;
			break;
		default:
			break;
		}
		Stream << StaticMeshData.Elements;
		return Stream;
	}

	IDataStream & operator >>(IDataStream & Stream, FStaticMeshData & StaticMeshData)
	{
		Stream >> StaticMeshData.Positions;
		Stream >> StaticMeshData.Normals;
		Stream >> StaticMeshData.Vertices;
		Stream >> StaticMeshData.IndexFormat;
		switch (StaticMeshData.IndexFormat)
		{
		case EIndexFormat::Index16:
			Stream >> StaticMeshData.Indices16;
			break;
		case EIndexFormat::Index32:
			Stream >> StaticMeshData.Indices32;
			break;
		default:
			break;
		}
		Stream >> StaticMeshData.Elements;
		return Stream;
	}

	FBoundingBox FStaticMeshData::CalculateBoundingBox() const
	{
		FBoundingBox BoundingBox;
		if (Positions.Size)
		{
			BoundingBox.MinPoint = Positions[0];
			BoundingBox.MaxPoint = Positions[0];
			BoundingBox.AddPoints(Positions);
		}
		else
			BoundingBox = {};
		return BoundingBox;
	}

	UStaticMesh::UStaticMesh(FEngine & Engine, FStringV ResourcePath)
		: IAssetResource(Engine, ResourcePath)
	{
	}

	UStaticMesh::~UStaticMesh()
	{
		
	}

	bool UStaticMesh::LoadAsset(FJsonObject & Json)
	{
		if (FJsonItem & JsonElementsItem = Json[u8"Elements"V])
		{
			FJsonArray & JsonElements = JsonElementsItem.Value.Get<FJsonArray &>();
			Materials.ResizeTo(JsonElements.Items.Size);
			for (uintx ElementIndex = 0; ElementIndex < JsonElements.Items.Size; ++ElementIndex)
			{
				FJsonObject & Element = JsonElements[ElementIndex].Get<FJsonObject &>();
				FString MaterialAssetPath = Element[u8"Material"V].Value.StringValue;
				if (MaterialAssetPath.StartsWith(AssetsPrefix))
					Materials[ElementIndex] = LoadMaterial(Engine, MaterialAssetPath.View(AssetsPrefix.Size));
			}
		}

		{
			FStringV DataFilePath = AssetPath + u8".data"V;
			IAssetStreamRef DataStream = Engine.LoadStream(DataFilePath);
			if (!DataStream)
			{
				LogWarning(u8"Missing data file \"{}\" for StaticMesh \"{}\""V, DataFilePath, AssetPath);
				return false;
			}
			FAssetHeader AssetHeader;
			*DataStream >> AssetHeader;
			*DataStream >> StaticMeshData;
		}

		BoundingBox = FBoundingBox(FVector3::Max, FVector3::Min);
		for (const FStaticMeshElement & Element : StaticMeshData.Elements)
		{
			//BoundingBox.AddPoints(StaticMeshData.Positions.Sub(Element.VertexOffset, Element.VertexCount));
			for (uint32 Index = 0; Index < Element.IndexCount; ++Index)
			{
				if (StaticMeshData.IndexFormat == EIndexFormat::Index16)
					BoundingBox.AddPoint(StaticMeshData.Positions[StaticMeshData.Indices16[Element.IndexOffset + Index]]);
				else
					BoundingBox.AddPoint(StaticMeshData.Positions[StaticMeshData.Indices32[Element.IndexOffset + Index]]);
			}
		}
		return true;
	}

	void UStaticMesh::Initialize()
	{
		
	}
	UStaticMeshRef LoadStaticMesh(FEngine & Engine, FStringV AssetPath)
	{
		IAssetStreamRef AssetStream = Engine.LoadStream(AssetPath);
		if (!AssetStream)
			return nullptr;

		FJsonDocument Json;
		*AssetStream >> Json;

		UStaticMeshRef StaticMesh = new UStaticMesh(Engine, AssetPath);
		StaticMesh->LoadAsset(Json);
		return StaticMesh;
	}
}
