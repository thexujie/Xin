#pragma once

#include "FBX.Types.h"

#include <fbxsdk.h>

namespace Xin
{
	template<>
	struct Xin::TFormatter<FbxLayerElementUV::EMappingMode>
	{
		FStringV Format(FbxLayerElementUV::EMappingMode MappingMode, FStringV) const
		{
			ENUM_STRING_BEGIN(MappingMode)
			ENUM_STRING_CASE(FbxLayerElementUV, eNone)
			ENUM_STRING_CASE(FbxLayerElementUV, eByControlPoint)
			ENUM_STRING_CASE(FbxLayerElementUV, eByPolygonVertex)
			ENUM_STRING_CASE(FbxLayerElementUV, eByPolygon)
			ENUM_STRING_CASE(FbxLayerElementUV, eByEdge)
			ENUM_STRING_CASE(FbxLayerElementUV, eAllSame)
			ENUM_STRING_END
		}
	};

	template<>
	struct TFormatter<FbxLayerElementUV::EReferenceMode>
	{
		FStringV Format(FbxLayerElementUV::EReferenceMode ReferenceMode, FStringV) const
		{
			ENUM_STRING_BEGIN(ReferenceMode)
			ENUM_STRING_CASE(FbxLayerElementUV, eDirect)
			ENUM_STRING_CASE(FbxLayerElementUV, eIndex)
			ENUM_STRING_CASE(FbxLayerElementUV, eIndexToDirect)
			ENUM_STRING_END
		}
	};
}

namespace Xin::FBX
{
	template<typename T>
	void TFbxObjectDeleter(T * ptr)
	{
		if (ptr)
			ptr->Destroy();
	}
	
	struct FFbxVersion
	{
		int Major = 0;
		int Minor = 0;
		int Revision = 0;
	};

	
	struct FFbxLoadConfig
	{
		bool BindToT0 = false;
		FMatrix Transform = FMatrix::Identity;
	};
	
	class FFbxLoader
	{
	public:
		FFbxLoader(FFbxLoadConfig LoadConfig) : LoadConfig(LoadConfig) {}
		~FFbxLoader() = default;

		FFbxModel LoadMeshFromFile(FStringV FileName);

		FFbxMesh LoadMeshNode(FbxNode * MeshNode, TList<FFbxMaterial> & Materials);
		FFbxMesh LoadMeshNodes(TView<FbxNode *> MeshNodes, TList<FFbxMaterial> & Materials);

		template<typename FbxElementT, typename ElementT, typename ConverterT>
		static void ExtractLayerElements(const FbxLayerElementTemplate<FbxElementT> * FbLayerElements, TList<ElementT> & Elements, ConverterT Converter, int32 NumPolygonVertices, TView<int32> PointIndices)
		{
			FbxLayerElementUV::EReferenceMode ReferenceMode = FbLayerElements->GetReferenceMode();
			FbxLayerElementUV::EMappingMode MappingMode = FbLayerElements->GetMappingMode();

			for (int32 PolygonVertexIndex = 0; PolygonVertexIndex < NumPolygonVertices; ++PolygonVertexIndex)
			{
				int32 ElementIndex = 0;
				if (MappingMode == FbxLayerElement::eByControlPoint)
					ElementIndex = PointIndices[PolygonVertexIndex];
				else if (MappingMode == FbxLayerElement::eByPolygonVertex)
					ElementIndex = PolygonVertexIndex;
				else
				{
					LogWarning(u8"Unsorported LayerElementUV MappingMode {}"V, MappingMode);
					return;
				}

				if (ReferenceMode == FbxLayerElement::eDirect)
				{
					FbxElementT FbxElement = FbLayerElements->GetDirectArray().GetAt(ElementIndex);
					Elements[PolygonVertexIndex] = Converter(FbxElement);
				}
				else if (ReferenceMode == FbxLayerElement::eIndexToDirect)
				{
					int32 FbxElementIndex = FbLayerElements->GetIndexArray().GetAt(ElementIndex);
					FbxElementT FbxElement = FbLayerElements->GetDirectArray().GetAt(FbxElementIndex);
					Elements[PolygonVertexIndex] = Converter(FbxElement);
				}
				else
				{
					LogWarning(u8"Unsorported LayerElementUV ReferenceMode {}"V, ReferenceMode);
					return;
				}
			}
		}
		
	public:
		FFbxLoadConfig LoadConfig;
		TList<FbxNode *> AllNodes;
		TList<FbxNode *> AllMeshNodes;
		TList<FbxNode *> AllCollisionNodes;
		TList<FbxNode *> AllSkeletonNodes;
		EVertexElements VertexElements = EVertexElements::None;


		TList<FbxNode *> LodGroups;

		bool ReverseCW = false;

		FbxAMatrix AxisConversionMatrix;
	};
}