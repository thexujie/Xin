#pragma once

#include "Asset.h"
#include "Texture.h"
#include "Material.h"

namespace Xin::Engine
{
	struct FStaticMeshElement
	{
		FString Name;
		uint32_t VertexOffset = 0;
		uint32_t VertexCount = 0;
		uint32_t IndexOffset = 0;
		uint32_t IndexCount = 0;

		friend IDataStream & operator <<(IDataStream & Stream, const FStaticMeshElement & Element)
		{
			Stream << Element.Name;
			Stream << Element.VertexOffset;
			Stream << Element.VertexCount;
			Stream << Element.IndexOffset;
			Stream << Element.IndexCount;
			return Stream;
		}

		friend IDataStream & operator >>(IDataStream & Stream, FStaticMeshElement & Element)
		{
			Stream >> Element.Name;
			Stream >> Element.VertexOffset;
			Stream >> Element.VertexCount;
			Stream >> Element.IndexOffset;
			Stream >> Element.IndexCount;
			return Stream;
		}
	};

	enum class EMeshDataComponets : uint32
	{
		None = 0,
		Position  = 0x0001,
		Normal    = 0x0002,
		Texcoord  = 0x0004,
		Tangent   = 0x0008,
		BiTangent = 0x0010,
	};

	struct alignas(float1) FStaticMeshVertex
	{
		FVector2 Texcoord;
		FVector3 Tangent;
		FVector3 BiTangent;

		friend IDataStream & operator <<(IDataStream & Stream, const FStaticMeshVertex & StaticMeshVertex)
		{
			Stream << StaticMeshVertex.Texcoord;
			Stream << StaticMeshVertex.Tangent;
			Stream << StaticMeshVertex.BiTangent;
			return Stream;
		}
		friend IDataStream & operator >>(IDataStream & Stream, FStaticMeshVertex & StaticMeshVertex)
		{
			Stream >> StaticMeshVertex.Texcoord;
			Stream >> StaticMeshVertex.Tangent;
			Stream >> StaticMeshVertex.BiTangent;
			return Stream;
		}
	};

	struct ENGINE_API FStaticMeshData
	{
		EMeshDataComponets Components = EMeshDataComponets::None;

		TList<FVector3> Positions;
		TList<FVector3> Normals;
		TList<FStaticMeshVertex> Vertices;

		EIndexFormat IndexFormat = EIndexFormat::Index16;
		TList<uint16_t> Indices16;
		TList<uint32_t> Indices32;

		TList<FStaticMeshElement> Elements;

		ENGINE_API friend IDataStream & operator <<(IDataStream & Stream, const FStaticMeshData & StaticMeshData);
		ENGINE_API friend IDataStream & operator >>(IDataStream & Stream, FStaticMeshData & StaticMeshData);

		FBoundingBox CalculateBoundingBox() const;
	};

	class ENGINE_API UStaticMesh : public IAssetResource
	{
	public:
		UStaticMesh(FEngine & Engine, FStringV ResourcePath);
		~UStaticMesh();

		bool LoadAsset(FJsonObject & Json) override;

	private:
		void Initialize();

	public:
		FStaticMeshData StaticMeshData;

		TList<UMaterialRef> Materials;
		FBoundingBox BoundingBox {};
	};
	using UStaticMeshRef = TReferPtr<UStaticMesh>;

	ENGINE_API UStaticMeshRef LoadStaticMesh(FEngine & Engine, FStringV AssetPath);
}
