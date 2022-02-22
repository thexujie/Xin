#include "PCH.h"
#include "H3DModel.h"

namespace H3D
{
	FH3DModel::FH3DModel()
	{

	}

	bool FH3DModel::Load(FStringView FilePath)
	{
		FFileStream FileStream(FilePath, EFileModes::Input);

		FileStream.ReadData(&Header, sizeof(FH3DHeader));

		Meshs.Resize(Header.MeshCount, EInitializeMode::Zero);
		FileStream.ReadData(Meshs.Data, sizeof(FH3DMesh) * Header.MeshCount);

		Materials.Resize(Header.MaterialCount, EInitializeMode::Zero);
		FileStream.ReadData(Materials.Data, sizeof(FH3DMaterial) * Header.MaterialCount);

        VertexStride = Meshs[0].VertexStride;
        VertexStrideDepth = Meshs[0].VertexStrideDepth;
#if _DEBUG
        for (uint32_t MeshIndex = 1; MeshIndex < Header.MeshCount; ++MeshIndex)
        {
            const FH3DMesh & Mesh = Meshs[MeshIndex];
            AssertExpresion(Mesh.VertexStride == VertexStride);
            AssertExpresion(Mesh.VertexStrideDepth == VertexStrideDepth);

            AssertExpresion(Mesh.AttribsEnabled == (attrib_mask_position | attrib_mask_texcoord0 | attrib_mask_normal | attrib_mask_tangent | attrib_mask_bitangent));
            AssertExpresion(Mesh.Attributes[0].Components == 3 && Mesh.Attributes[0].Format == H3D::attrib_format_float); // position
            AssertExpresion(Mesh.Attributes[1].Components == 2 && Mesh.Attributes[1].Format == H3D::attrib_format_float); // texcoord0
            AssertExpresion(Mesh.Attributes[2].Components == 3 && Mesh.Attributes[2].Format == H3D::attrib_format_float); // normal
            AssertExpresion(Mesh.Attributes[3].Components == 3 && Mesh.Attributes[3].Format == H3D::attrib_format_float); // tangent
            AssertExpresion(Mesh.Attributes[4].Components == 3 && Mesh.Attributes[4].Format == H3D::attrib_format_float); // bitangent

            AssertExpresion(Mesh.AttribsEnabledDepth == (attrib_mask_position));
            AssertExpresion(Mesh.Attributes[0].Components == 3 && Mesh.Attributes[0].Format == H3D::attrib_format_float); // position
        }
#endif

        uint32_t TotalBinarySize = Header.VertexDataByteSize
            + Header.IndexDataByteSize
            + Header.VertexDataByteSizeDepth
            + Header.IndexDataByteSize;

        BinaryData.Resize(TotalBinarySize, EInitializeMode::None);
		uintx_t ReadBinarySize = FileStream.Read(BinaryData.Data, TotalBinarySize);
        AssertExpresion(ReadBinarySize == TotalBinarySize);
        TView<FH3DVertex> TypedVertices = TView<FH3DVertex>((const FH3DVertex *)BinaryData.Data, Header.VertexDataByteSize / VertexStride);
        TView<uint16_t> TypedIndices = TView<uint16_t>((const uint16_t *)(BinaryData.Data + Header.VertexDataByteSize), Header.IndexDataByteSize / sizeof(uint16_t));

        TView<FVector3> TypedDepthVertices = TView<FVector3>((const FVector3 *)(BinaryData.Data + Header.VertexDataByteSize + Header.IndexDataByteSize), Header.VertexDataByteSizeDepth / sizeof(FVector3));
        TView<uint16_t> TypedDepthIndices = TView<uint16_t>((const uint16_t *)(BinaryData.Data + Header.VertexDataByteSize + Header.IndexDataByteSize + Header.VertexDataByteSizeDepth), Header.IndexDataByteSize / sizeof(uint16_t));

		Vertices = { (const FH3DVertex *)BinaryData.Data, Header.VertexDataByteSize / sizeof(FH3DVertex)};
		Indices = { (const uint16_t *)(BinaryData.Data + Header.VertexDataByteSize), Header.IndexDataByteSize / 2 };
        uint16_t MaxIndex = 0;
        for (auto DepthIndex : TypedDepthIndices)
        {
            if (DepthIndex > MaxIndex)
                MaxIndex = DepthIndex;
        }

		return true;
	}
}
