#include "PCH.h"
#include "FbxLoader.h"

namespace Xin::FBX
{
	using namespace fbxsdk;

	bool IsCollisionMeshNode(const FbxNode * MeshNode)
	{
		auto NodeName = MeshNode->GetNameWithoutNameSpacePrefix();
		if (NodeName.Find("UCX") != -1 || NodeName.Find("MCDCX") != -1 ||
			NodeName.Find("UBX") != -1 || NodeName.Find("USP") != -1 || NodeName.Find("UCP") != -1)
			return true;
		else
			return false;
	}

	FbxAMatrix GetNodeGeometryTransform(const FbxNode & MeshNode)
	{
		FbxVector4 T = MeshNode.GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 R = MeshNode.GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 S = MeshNode.GetGeometricScaling(FbxNode::eSourcePivot);

		return FbxAMatrix(T, R, S);
	}

	FVector3 FromFbxPoint(const FbxVector4 & FbxPoint)
	{
		return { (float32)FbxPoint[0], (float32)FbxPoint[1], (float32)FbxPoint[2] };
	}

	FMatrix FromAMatrix(const FbxAMatrix & AMatrix)
	{
		FbxVector4 T = AMatrix.GetT();
		FbxQuaternion Q = AMatrix.GetQ();
		FbxVector4 S = AMatrix.GetS();

		return FMatrix::Transform(FVector3(T[0], T[1], T[2]), FQuat(Q[0], Q[1], Q[2], Q[3]), FVector3(S[0], S[1], S[2]));
	}

	static FStringV FbxStatusString(FbxStatus & Status)
	{
		return FStringV((const char8_t *)Status.GetErrorString());
	}

	FFbxModel FFbxLoader::LoadMeshFromFile(FStringV FileName)
	{
		FFbxVersion SdkVersin;
		FbxManager::GetFileFormatVersion(SdkVersin.Major, SdkVersin.Minor, SdkVersin.Revision);

		const char8 * FbxsdkVersion = (const char8 *)FbxManager::GetVersion();
		LogWarning(u8"fbxsdk version {}, {}.{}.{}.", FbxsdkVersion, SdkVersin.Major, SdkVersin.Minor, SdkVersin.Revision);


		TUniquePtr<FbxManager, void(*)(FbxManager *)> FbManager = TUniquePtr<FbxManager, void(*)(FbxManager *)>(FbxManager::Create(), TFbxObjectDeleter);
		TUniquePtr<FbxScene, void(*)(FbxScene *)> Scene = TUniquePtr<FbxScene, void(*)(FbxScene *)>(FbxScene::Create(FbManager.Get(), ""), TFbxObjectDeleter);
		TUniquePtr<FbxGeometryConverter> FbGeometryConverter = MakeUnique<FbxGeometryConverter>(FbManager.Get());
		TUniquePtr<FbxImporter, void(*)(FbxImporter *)> FbImporter = TUniquePtr<FbxImporter, void(*)(FbxImporter *)>(FbxImporter::Create(FbManager.Get(), ""), TFbxObjectDeleter);

		FString FileNameStr = FileName;
		bool InitializeOK = FbImporter->Initialize((const char *)(FileNameStr.Data), -1, nullptr);
		if (!InitializeOK)
		{
			FbxStatus Status = FbImporter->GetStatus();
			LogWarning(u8"FbxImporter::Initialize failed, {}", FbxStatusString(Status));
			return {};
		}

		bool ImportOK = FbImporter->Import(Scene.Get());
		if (!ImportOK)
		{
			FbxStatus Status = FbImporter->GetStatus();
			LogWarning(u8"FbxImporter::Import failed, {}", FbxStatusString(Status));
			return {};
		}

		// Objects in the FBX SDK are always created in the right handed, Y-Up axis system. 
		// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=files/GUID-CC93340E-C4A1-49EE-B048-E898F856CFBF.htm,topicNumber=d30e8478
		// Note that calls to ConvertScene() do not change the vertex values of meshes, and only affect node transforms and animations

		FbxAxisSystem XinImportAxis = FbxAxisSystem::DirectX;

		FbxSystemUnit SceneUnit = Scene->GetGlobalSettings().GetSystemUnit();
		FbxAxisSystem SceneAxisBefore = Scene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem::ECoordSystem CoorSystemBefore = SceneAxisBefore.GetCoorSystem();
		// it cannot represent changes in handedness (e.g. from right-handed to left-handed).
		//XinImportAxis.ConvertScene(Scene);
		FbxAxisSystem SceneAxisAfter = Scene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem::ECoordSystem CoorSystemAfter = SceneAxisAfter.GetCoorSystem();

		ReverseCW = SceneAxisBefore.GetCoorSystem() == FbxAxisSystem::eRightHanded;

		if (Scene->GetGlobalSettings().GetSystemUnit() != FbxSystemUnit::cm)
			FbxSystemUnit::cm.ConvertScene(Scene);

		Scene->GetAnimationEvaluator()->Reset();

		FbxAMatrix SourceMatrix;
		SceneAxisBefore.GetMatrix(SourceMatrix);
		FbxAMatrix TargetMatrix;
		XinImportAxis.GetMatrix(TargetMatrix);
		AxisConversionMatrix = TargetMatrix * SourceMatrix.Inverse();

		int FrontSignBefore = 1;
		int FrontSignAfter = 1;
		SceneAxisBefore.GetFrontVector(FrontSignBefore);
		XinImportAxis.GetFrontVector(FrontSignAfter);

		// Force towards
		if (FrontSignBefore != FrontSignAfter)
		{
			// This FrontMatrix is just mirrored, we shouldn't revert front direction by this way.
			//FbxAMatrix FrontMatrix({}, FbxVector4(), { 1, 1, -1 });
			//FbxAMatrix FrontMatrix({ }, FbxVector4(0, 180, 0), { 1, 1, 1 });
			//AxisConversionMatrix = FrontMatrix * TargetMatrix * SourceMatrix.Inverse();
		}

		FFbxVersion FileVersin;
		FbImporter->GetFileVersion(FileVersin.Major, FileVersin.Minor, FileVersin.Revision);

		bool Triangulated = FbGeometryConverter->Triangulate(Scene.Get(), true);
		if (!Triangulated)
		{
			LogError(u8"Can't triangulate geometry."V);
		}

		auto ParseNode = [&](this auto && This, FbxNode * Node)-> void
		{
			AllNodes.Add(Node);

			FbxNodeAttribute * NodeAttribute = Node->GetNodeAttribute();
			if (NodeAttribute)
			{
				FbxNodeAttribute::EType AttributeType = NodeAttribute->GetAttributeType();
				if (AttributeType == FbxNodeAttribute::eMesh)
				{
					//if (Node->GetMesh() && Node->GetMesh()->GetDeformerCount(FbxDeformer::eSkin) > 0)
					if (IsCollisionMeshNode(Node))
						AllCollisionNodes.Add(Node);
					else
						AllMeshNodes.Add(Node);
				}
				else if (AttributeType == FbxNodeAttribute::eLODGroup)
				{
					LodGroups.Add(Node);
				}
				else
				{
					
				}
			}

			if (Node->GetSkeleton())
				AllSkeletonNodes.Add(Node);

			for (int ChildIndex = 0; ChildIndex < Node->GetChildCount(); ++ChildIndex)
				This(Node->GetChild(ChildIndex));
		};

		FbxNode * RootNode = Scene->GetRootNode();
		ParseNode(RootNode);

		auto IsLodNodeOf = [](FbxNode * Parent, FbxNode * Node)
		{
			if (Parent == Node)
				return true;

			while (Node)
			{
				if (Node->GetParent() == Parent)
					return true;
				Node = Node->GetParent();
			}
			return false;
		};

		TList<FFbxMaterial> Materials;
		if (LodGroups.Size)
		{
			TList<TList<FbxNode *>> LodMeshNodes;
			for (uintx LodGroupIndex = 0; LodGroupIndex < LodGroups.Size; ++LodGroupIndex)
			{
				FbxNode * LodGroupNode = LodGroups[LodGroupIndex];
				int32 NumLods = LodGroupNode->GetChildCount(false);
				if (NumLods > LodMeshNodes.Size)
					LodMeshNodes.Resize(NumLods);

				for (int32 LodIndex = 0; LodIndex < NumLods; ++LodIndex)
				{
					FbxNode * LodNode = LodGroupNode->GetChild(LodIndex);
					for (FbxNode * MeshNode : AllMeshNodes)
					{
						if (IsLodNodeOf(LodNode, MeshNode))
							LodMeshNodes[LodIndex].Add(MeshNode);
					}
				}
			}

			FFbxModel FbxModel;
			FbxModel.Materials = Materials;
			FbxModel.LodMeshs.ResizeTo(LodMeshNodes.Size);

			for (uintx LodIndex = 0; LodIndex < LodMeshNodes.Size; ++LodIndex)
			{
				FFbxMesh FbxMesh = LoadMeshNodes(LodMeshNodes[LodIndex], Materials);
				FbxModel.LodMeshs[LodIndex] = FbxMesh;
			}
			return FbxModel;
		}
		else
		{
			FFbxMesh FbxMesh = LoadMeshNodes(AllMeshNodes, Materials);
			return { Materials, { FbxMesh } };
		}
	}

	FFbxMesh FFbxLoader::LoadMeshNodes(TView<FbxNode *> MeshNodes, TList<FFbxMaterial> & Materials)
	{
		TList<FMeshVertex> Vertices;
		FFbxIndices Indices;
		TList<FFbxMeshElement> MeshElements;

		if (!LoadConfig.BindToT0)
		{
			TList<FFbxMesh> NodeMeshs;
			for (uintx Index = 0; Index < MeshNodes.Size; ++Index)
			{
				FFbxMesh NodeMesh = LoadMeshNode(MeshNodes[Index], Materials);
				Indices.Index32 |= NodeMesh.Indices.Index32;
				NodeMeshs.Emplace(Move(NodeMesh));
			}

			for (uintx Index = 0; Index < NodeMeshs.Size; ++Index)
			{
				FFbxMesh & NodeMesh = NodeMeshs[Index];
				if (NodeMesh.Vertices.Size == 0)
					continue;

				uint32 NodeIndexOffset = uint32(Indices.GetNumIndices());
				uint32 NodeVertexOffset = uint32(Vertices.Size);
				Vertices.AddRange(NodeMesh.Vertices);
				Indices.AddIndices(NodeMesh.Indices);

				for (uintx MeshElementIndex = 0; MeshElementIndex < NodeMesh.Elements.Size; ++MeshElementIndex)
				{
					const FFbxMeshElement & NodeMeshElement = NodeMesh.Elements[MeshElementIndex];
					if (NodeMeshElement.IndexCount == 0)
						continue;

					FFbxMeshElement & MeshElement = MeshElements.AddDefault();
					MeshElement.NodeName = NodeMeshElement.NodeName;
					MeshElement.MaterialIndex = NodeMeshElement.MaterialIndex;
					MeshElement.IndexOffset = NodeIndexOffset + NodeMeshElement.IndexOffset;
					MeshElement.VertexOffset = NodeVertexOffset + NodeMeshElement.VertexOffset;
					MeshElement.IndexCount = NodeMeshElement.IndexCount;
				}
			}
		}
		return { Vertices, Indices, MeshElements };
	}

	FFbxMesh FFbxLoader::LoadMeshNode(FbxNode * MeshNode, TList<FFbxMaterial> & Materials)
	{
		FbxMesh * Mesh = MeshNode->GetMesh();
		MeshNode->mCullingType;
		if (!Mesh)
		{
			LogInfo(u8"No mesh in node {}", FStringV((const char8_t *)MeshNode->GetName()));
			return {};
		}
		const char * NodeName = MeshNode->GetName();

		// Column Major
		FbxAMatrix GeometryTransform = GetNodeGeometryTransform(*MeshNode);
		FbxAMatrix GlobalTransform = MeshNode->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);

		FbxAMatrix FinalNodeTransform = AxisConversionMatrix * GlobalTransform * GeometryTransform;
		//FbxDouble3 NodeScaling = FbMeshNode->LclScaling.Get();
		//FbxVector4 NodeTransform(NodeScaling[0], NodeScaling[1], NodeScaling[2], 1);
		uint32 LayerCount = Mesh->GetLayerCount();

		int NumPolygons = Mesh->GetPolygonCount();
		int32 NumPolygonVertices = Mesh->GetPolygonVertexCount();
		AssertExpr(NumPolygons * 3 == NumPolygonVertices);

		TList<int32> PolygonVertexIndices = TView<int32> { Mesh->GetPolygonVertices(), (uintx)NumPolygonVertices };

		TList<int32> PolygonMaterials;
		uint32 NumMaterials = 0;
		// We split this mesh to multi elements by material.
		if (FbxGeometryElementMaterial * GeometryElementMaterial = Mesh->GetElementMaterial())
		{
			FbxLayerElementArrayTemplate<int> & MaterialIndices = GeometryElementMaterial->GetIndexArray();
			FbxLayerElement::EMappingMode MappingMode = GeometryElementMaterial->GetMappingMode();
			switch (MappingMode)
			{
			case FbxGeometryElement::eByPolygon:
			{
				PolygonMaterials.ResizeTo(NumPolygons, EInitializeMode::Zero);
				AssertExpr(MaterialIndices.GetCount() == NumPolygons);
				if (MaterialIndices.GetCount() == NumPolygons)
				{
					for (int TriangleIndex = 0; TriangleIndex < NumPolygons; ++TriangleIndex)
					{
						int32 MaterialIndex = MaterialIndices.GetAt(TriangleIndex);
						AssertExpr(MaterialIndex >= 0);

						PolygonMaterials[TriangleIndex] = MaterialIndex;

						if (MaterialIndex >= 0)
						{
							NumMaterials = Max(NumMaterials, uint32(MaterialIndex + 1));
						}
					}

					//MeshElement.MaterialName = FStringView((const char8_t *)MaterialName);
					//MeshElement.MaterialIndex = MaterialIndex;

					//for (int LayerIndex = FbxLayerElement::eTextureDiffuse; LayerIndex <= FbxLayerElement::eTextureDisplacementVector; ++LayerIndex)
					//{
					//	FbxProperty DiffuseTextureProperty = SurfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[LayerIndex]);
					//	if (DiffuseTextureProperty.IsValid())
					//	{
					//		int TextureCount = DiffuseTextureProperty.GetSrcObjectCount<FbxTexture>();
					//		for (int TextureIndex = 0; TextureIndex < TextureCount; ++TextureIndex)
					//		{
					//			FbxTexture * Texture = static_cast<FbxTexture *>(DiffuseTextureProperty.GetSrcObject<FbxTexture>(TextureIndex));
					//			if (Texture)
					//			{
					//				FString TextureName = FStringView((const char8_t *)Texture->GetName());
					//			}
					//		}
					//	}
					//}
				}
			}
			break;
			case FbxGeometryElement::eAllSame:
				break;
			default:
				break;
			}
		}

		VertexElements = EVertexElements::Position;

		int32 NumControlPoints = Mesh->GetControlPointsCount();
		if (!NumControlPoints)
			return {};

		assert(Mesh->GetControlPoints());

		TView<FbxVector4> MeshControlPoints { Mesh->GetControlPoints(), uint32(NumControlPoints) };
		TList<FVector3> ControlPoints(NumControlPoints);

		for (int32 PointIndex = 0; PointIndex < NumControlPoints; ++PointIndex)
		{
			const FbxVector4 & ControlPoint = MeshControlPoints[PointIndex];
			ControlPoints[PointIndex] = FromFbxPoint(FinalNodeTransform.MultT(ControlPoint));
			//ControlPoints[PointIndex].Y = -ControlPoints[PointIndex].Y;
		}

		for (int32 PolygonIndex = 0; PolygonIndex < NumPolygons; ++PolygonIndex)
		{
			int32 PolygonSize = Mesh->GetPolygonSize(PolygonIndex);
			if (PolygonSize != 3)
			{
				LogWarning(u8"Only triangles supported!"V);
				return {};
			}
		}

		auto ConvertUV = [](const FbxVector2 & FbUV)-> FVector2
		{
			return FVector2(float32(FbUV[0]), float32(FbUV[1]));
		};

		int32 NumLayers = Mesh->GetLayerCount();
		if (NumLayers < 1)
		{
			LogWarning(u8"Invalid layer count {}"V, NumLayers);
			return {};
		}

		//for (int32 LayerIndex = 0; LayerIndex < NumLayers; ++LayerIndex)
		//{
		//}
		//

		TList<FVector2> PolygonUVs;
		if (int32 NumDiffuseUVs = Mesh->GetElementUVCount(FbxLayerElement::eTextureDiffuse); NumDiffuseUVs > 0)
		{
			FbxGeometryElementUV * GeometryElementUV = Mesh->GetElementUV(0, FbxLayerElement::eTextureDiffuse);
			if (GeometryElementUV)
			{
				PolygonUVs.Resize(NumPolygonVertices);
				ExtractLayerElements<FbxVector2, FVector2, decltype(ConvertUV)>(GeometryElementUV, PolygonUVs, ConvertUV, NumPolygonVertices, PolygonVertexIndices);
				VertexElements |= EVertexElements::UV;
			}
		}

		//FbxLayer * FbLayer0 = FbMesh->GetLayer(0);
		//assert(FbLayer0);
		//const FbxLayerElementUV * FbLayerUVs = FbLayer0->GetUVs(FbxLayerElement::eTextureDiffuse);
		//if (FbLayerUVs)
		//{
		//	UVs.Resize(NumPolygonVertices);
		//	ExtractLayerElements<FbxVector2, FVector2, decltype(ConvertUV)>(FbLayerUVs, UVs, ConvertUV, NumPolygonVertices, PointIndices);
		//	VertexElements |= EVertexElements::UV;
		//}

		FbxLayer * Layer0 = Mesh->GetLayer(0);
		FbxLayer * Layer1 = Mesh->GetLayer(1);

		TList<FVector2> PolygonUV2s;
		if (const FbxLayerElementUV * LayerElementUV2 = Layer1 ? Layer1->GetUVs(FbxLayerElement::eTextureDiffuse) : nullptr)
		{
			PolygonUV2s.Resize(NumPolygonVertices);
			ExtractLayerElements<FbxVector2, FVector2, decltype(ConvertUV)>(LayerElementUV2, PolygonUV2s, ConvertUV, NumPolygonVertices, PolygonVertexIndices);
			VertexElements |= EVertexElements::UV2;
		}


		// Normals
		auto ConvertNormal = [](const FbxVector4 & FbUV)-> FVector3
		{
			return FVector3(float32(FbUV[0]), float32(FbUV[1]), float32(FbUV[2]));
		};

		TList<FVector3> PolygonNormals;
		const FbxLayerElementNormal * FbLayerNormals = Layer0->GetNormals();
		if (FbLayerNormals)
		{
			PolygonNormals.Resize(NumPolygonVertices);
			ExtractLayerElements<FbxVector4, FVector3, decltype(ConvertNormal)>(FbLayerNormals, PolygonNormals, ConvertNormal, NumPolygonVertices, PolygonVertexIndices);
			VertexElements |= EVertexElements::Normal;
		}

		TList<FVector3> PolygonBinormals;
		const FbxLayerElementBinormal * FbLayerBiNormals = Layer0->GetBinormals();
		if (FbLayerBiNormals)
		{
			PolygonBinormals.Resize(NumPolygonVertices);
			ExtractLayerElements<FbxVector4, FVector3, decltype(ConvertNormal)>(FbLayerBiNormals, PolygonBinormals, ConvertNormal, NumPolygonVertices, PolygonVertexIndices);
			VertexElements |= EVertexElements::BiNormal;
		}

		// Vertex Colors
		auto ConvertColor = [](const FbxColor & FbUV)-> FColor
		{
			return FColor(float32(FbUV[0]), float32(FbUV[1]), float32(FbUV[2]));
		};

		TList<FColor> PolygonVertexColors;
		const FbxLayerElementVertexColor * LayerVertexColors = Layer0->GetVertexColors();
		if (LayerVertexColors)
		{
			PolygonVertexColors.Resize(NumPolygonVertices);
			ExtractLayerElements<FbxColor, FColor, decltype(ConvertColor)>(LayerVertexColors, PolygonVertexColors, ConvertColor, NumPolygonVertices, PolygonVertexIndices);
			VertexElements |= EVertexElements::Color;
		}

		auto IsVertexEqual = [&](const FMeshVertex & VertexA, const FMeshVertex & VertexB, EVertexElements VertexElements)
		{
			if ((VertexElements & EVertexElements::UV) && (VertexA.UV - VertexB.UV).LengthSquare() > 0.01f)
				return false;

			if ((VertexElements & EVertexElements::UV2) && (VertexA.UV2 - VertexB.UV2).LengthSquare() > 0.01f)
				return false;

			if ((VertexElements & EVertexElements::Normal) && (VertexA.Normal - VertexB.Normal).LengthSquare() > 0.01f)
				return false;

			if ((VertexElements & EVertexElements::BiNormal) && (VertexA.BiNormal - VertexB.BiNormal).LengthSquare() > 0.01f)
				return false;

			if ((VertexElements & EVertexElements::Color) && VertexA.Color != VertexB.Color)
				return false;

			return true;
		};

		TList<FMeshVertex> FinalVertices;

		TMultiMap<int32, uint32> PointVertexMapping;
		auto TryAddMeshVertex = [&](uint32 PolygonVertexIndex)-> uint32
		{
			FMeshVertex MeshVertex = {};
			int32 ControlPointIndex = PolygonVertexIndices[PolygonVertexIndex];
			MeshVertex.Position = ControlPoints[ControlPointIndex];

			if (VertexElements & EVertexElements::UV)
				MeshVertex.UV = PolygonUVs[PolygonVertexIndex];

			if (VertexElements & EVertexElements::UV2)
				MeshVertex.UV2 = PolygonUV2s[PolygonVertexIndex];

			if (VertexElements & EVertexElements::Normal)
				MeshVertex.Normal = PolygonNormals[PolygonVertexIndex];

			if (VertexElements & EVertexElements::BiNormal)
				MeshVertex.BiNormal = PolygonBinormals[PolygonVertexIndex];

			if (VertexElements & EVertexElements::Color)
				MeshVertex.Color = FVector4(PolygonVertexColors[PolygonVertexIndex]);

			auto PointVerticesIterRange = PointVertexMapping.EqualRange(ControlPointIndex);
			for (auto Iter = PointVerticesIterRange.First; Iter != PointVerticesIterRange.Second; ++Iter)
			{
				if (IsVertexEqual(MeshVertex, FinalVertices[Iter->Second], VertexElements))
				{
					// Found exists vetex with same attributes;
					return Iter->Second;
				}
			}

			uint32 VertexIndex = uint32(FinalVertices.GetSize());
			PointVertexMapping.Insert({ ControlPointIndex, VertexIndex });
			FinalVertices.Add(MeshVertex);
			return VertexIndex;
		};

		for (int32 MaterialIndex = 0; MaterialIndex < NumMaterials; ++MaterialIndex)
		{
			FFbxMaterial & MeshMaterial = Materials.AddDefault();
			FbxSurfaceMaterial * SurfaceMaterial = MeshNode->GetMaterial(MaterialIndex);
			if (SurfaceMaterial)
			{
				const char * MaterialName = SurfaceMaterial->GetName();
				MeshMaterial.MaterialName = FStringV((const char8 *)MaterialName);

				for (int LayerIndex = FbxLayerElement::eTextureDiffuse; LayerIndex <= FbxLayerElement::eTextureDisplacementVector; ++LayerIndex)
				{
					FbxProperty DiffuseTextureProperty = SurfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[LayerIndex - FbxLayerElement::eTextureDiffuse]);
					if (DiffuseTextureProperty.IsValid())
					{
						int TextureCount = DiffuseTextureProperty.GetSrcObjectCount<FbxTexture>();
						for (int TextureIndex = 0; TextureIndex < TextureCount; ++TextureIndex)
						{
							FbxTexture * Texture = DiffuseTextureProperty.GetSrcObject<FbxTexture>(TextureIndex);
							if (Texture)
							{
								switch (LayerIndex)
								{
								case FbxLayerElement::eTextureDiffuse:
									MeshMaterial.DiffuseTexture = FStringV((const char8 *)Texture->GetName());
									break;
								default:
									break;
								}
							}
						}
					}
				}
			}
			else
			{
				MeshMaterial.MaterialName = Format(u8"{}_{}_Mat"V, FStringV((const char8 *)NodeName), MaterialIndex);
			}
		}

		uint32 NumElements = Max(1, NumMaterials);
		TList<FFbxMeshElement> MeshElements { NumElements };
		TList<TList<uint32>> MeshIndices { NumElements, EInitializeMode::Default };

		for (int32 PolygonIndex = 0; PolygonIndex < NumPolygons; ++PolygonIndex)
		{
			int32 StartIndex = Mesh->GetPolygonVertexIndex(PolygonIndex);
			AssertExpr(StartIndex >= 0);

			if (StartIndex == -1)
				continue;
			int32 PolygonSize = Mesh->GetPolygonSize(PolygonIndex);
			AssertExpr(PolygonSize == 3);

			int32 ElementIndex = PolygonMaterials.Size ? PolygonMaterials[PolygonIndex] : 0;
			AssertExpr(ElementIndex >= 0);
			if (ElementIndex < 0)
				continue;

			if (ReverseCW)
			{
				for (int32 VertexIndex = 0; VertexIndex < PolygonSize; ++VertexIndex)
				{
					MeshIndices[ElementIndex].Add(TryAddMeshVertex(StartIndex + (PolygonSize - 1 - VertexIndex)));
				}
			}
			else
			{
				for (int32 VertexIndex = 0; VertexIndex < PolygonSize; ++VertexIndex)
				{
					MeshIndices[ElementIndex].Add(TryAddMeshVertex(StartIndex + VertexIndex));
				}
			}
		}

		FFbxIndices FinalIndices { FinalVertices.Size > 0xFFFF };
		for (int32 ElementIndex = 0; ElementIndex < NumElements; ++ElementIndex)
		{
			FFbxMeshElement & MeshElement = MeshElements[ElementIndex];
			FbxSurfaceMaterial * SurfaceMaterial = MeshNode->GetMaterial(ElementIndex);
			if (SurfaceMaterial)
			{
				const char * MaterialName = SurfaceMaterial->GetName();
				MeshElement.NodeName = FStringV((const char8 *)MaterialName);
			}
			else
			{
				MeshElement.NodeName = Format(u8"{}_{}"V, FStringV((const char8 *)NodeName), ElementIndex);
			}
			MeshElement.MaterialIndex = ElementIndex;
			MeshElement.IndexOffset = uint32(FinalIndices.GetNumIndices());
			MeshElement.VertexOffset = 0;
			MeshElement.IndexCount = uint32(MeshIndices[ElementIndex].Size);

			FinalIndices.AddIndices(MeshIndices[ElementIndex]);
		}

		return { FinalVertices, FinalIndices, MeshElements };
	}
}
