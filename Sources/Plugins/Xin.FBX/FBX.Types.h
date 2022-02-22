#pragma once

#include "Xin.Core/Xin.Core.h"

#if XIN_WINDOWS
#ifdef _Xin_FBX_Module_
#define XIN_FBX_API __declspec(dllexport)
#else
#define XIN_FBX_API __declspec(dllimport)
#endif
#endif

namespace Xin::FBX
{
	enum class EVertexElements
	{
		None = 0,
		Position = 0x0001,
		UV = 0x0002,
		UV2 = 0x0004,
		Normal = 0x0008,
		BiNormal = 0x0010,
		Color = 0x0020,
	};

	struct alignas(sizeof(uint32)) FMeshVertex
	{
		FVector3 Position;
		FVector2 UV;
		FVector2 UV2;
		FVector3 Normal;
		FVector3 BiNormal;
		FVector4 Color;
	};

	struct FFbxMeshElement
	{
		FString NodeName;
		//FString MaterialName;
		uint32 MaterialIndex = 0;

		uint32 IndexOffset = 0;
		uint32 VertexOffset = 0;
		uint32 IndexCount = 0;

		friend IDataStream & operator >>(IDataStream & Stream, FFbxMeshElement & This)
		{
			Stream >> This.NodeName;
			Stream >> This.MaterialIndex;
			Stream >> This.IndexOffset;
			Stream >> This.VertexOffset;
			Stream >> This.IndexCount;
			return Stream;
		}

		friend IDataStream & operator <<(IDataStream & Stream, const FFbxMeshElement & This)
		{
			Stream << This.NodeName;
			Stream << This.MaterialIndex;
			Stream << This.IndexOffset;
			Stream << This.VertexOffset;
			Stream << This.IndexCount;
			return Stream;
		}
	};

	enum class EFbxIndexFormat
	{
		None,
		Index16,
		Index32,
	};

	struct FFbxIndices
	{
		// ! Do not modifiy IndexFormat directly !!!, Use Reset instead.
		bool Index32 = false;
		TList<uint16> Indices16;
		TList<uint32> Indices32;

		FFbxIndices() = default;
		FFbxIndices(bool Index32) : Index32(Index32) {}
		FFbxIndices(const FFbxIndices &) = default;
		FFbxIndices(FFbxIndices &&) noexcept = default;
		FFbxIndices & operator=(const FFbxIndices &) = default;
		FFbxIndices & operator=(FFbxIndices &&) noexcept = default;

		void AddIndices(TList<uint32> Indices)
		{
			if (Index32)
				Indices32.AddRange(Indices);
			else
				std::transform(Indices.Begin(), Indices.End(), std::back_inserter(Indices16), [](uint32 Index) { return uint16(Index); });
		}

		void AddIndices(const FFbxIndices & FbxIndices)
		{
			if (Index32)
			{
				if (FbxIndices.Index32)
					Indices32.AddRange(FbxIndices.Indices32);
				else
					std::transform(FbxIndices.Indices16.Begin(), FbxIndices.Indices16.End(), std::back_inserter(Indices32), [](uint16 Index) { return uint32(Index); });
			}
			else
			{
				if (FbxIndices.Index32)
					std::transform(FbxIndices.Indices32.Begin(), FbxIndices.Indices32.End(), std::back_inserter(Indices16), [](uint32 Index) { return uint16(Index); });
				else
					Indices16.AddRange(FbxIndices.Indices16);
			}
		}

		uint32 GetNumIndices() const
		{
			return Index32 ? Indices32.Size : Indices16.Size;
		}

		TView<byte> GetIndices() const
		{
			return Index32 ? ReinterpretView<byte>(Indices32) : ReinterpretView<byte>(Indices16);
		}

		friend IDataStream & operator >>(IDataStream & Stream, FFbxIndices & This)
		{
			Stream >> This.Index32;
			Stream >> This.Indices16;
			Stream >> This.Indices32;
			return Stream;
		}

		friend IDataStream & operator <<(IDataStream & Stream, const FFbxIndices & This)
		{
			Stream << This.Index32;
			Stream << This.Indices16;
			Stream << This.Indices32;
			return Stream;
		}
	};

	struct FFbxMaterial
	{
		FString MaterialName;
		FString DiffuseTexture;

		friend IDataStream & operator >>(IDataStream & Stream, FFbxMaterial & This)
		{
			Stream >> This.MaterialName;
			Stream >> This.DiffuseTexture;
			return Stream;
		}

		friend IDataStream & operator <<(IDataStream & Stream, const FFbxMaterial & This)
		{
			Stream << This.MaterialName;
			Stream << This.DiffuseTexture;
			return Stream;
		}
	};


	struct FFbxMesh
	{
		TList<FMeshVertex> Vertices;
		FFbxIndices Indices;
		TList<FFbxMeshElement> Elements;

		friend IDataStream & operator >>(IDataStream & Stream, FFbxMesh & This)
		{
			Stream >> This.Vertices;
			Stream >> This.Indices;
			Stream >> This.Elements;
			return Stream;
		}

		friend IDataStream & operator <<(IDataStream & Stream, const FFbxMesh & This)
		{
			Stream << This.Vertices;
			Stream << This.Indices;
			Stream << This.Elements;
			return Stream;
		}
	};

	struct FFbxModel
	{
		TList<FFbxMaterial> Materials;
		TList<FFbxMesh> LodMeshs;

		friend IDataStream & operator >>(IDataStream & Stream, FFbxModel & This)
		{
			Stream >> This.Materials;
			Stream >> This.LodMeshs;
			return Stream;
		}

		friend IDataStream & operator <<(IDataStream & Stream, const FFbxModel & This)
		{
			Stream << This.Materials;
			Stream << This.LodMeshs;
			return Stream;
		}
	};
}
