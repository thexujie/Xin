#include "PCH.h"
#include "RHIREsource.h"

namespace Xin::RHI
{
	IDataStream & operator <<(IDataStream & Stream, const FTextureSlice & TextureSlice)
	{
		Stream << TextureSlice.Format;
		Stream << TextureSlice.Width;
		Stream << TextureSlice.Height;
		Stream << TextureSlice.RowPitch;
		Stream << TextureSlice.SlicePitch;
		return Stream;
	}

	IDataStream & operator >>(IDataStream & Stream, FTextureSlice & TextureSlice)
	{
		Stream >> TextureSlice.Format;
		Stream >> TextureSlice.Width;
		Stream >> TextureSlice.Height;
		Stream >> TextureSlice.RowPitch;
		Stream >> TextureSlice.SlicePitch;
		return Stream;
	}

	FTextureData::FTextureData(const FImageData & ImageData)
		: Format(ImageData.Format), Dimension(ImageData.Width, ImageData.Height, ImageData.MipLevels, ImageData.NumLayers)
	{
		Bytes = ImageData.Bytes;
		Slices = ImageData.Slices;
	}

	IDataStream & operator <<(IDataStream & Stream, const FTextureData & TextureData)
	{
		Stream << TextureData.Format;
		Stream << TextureData.Dimension;
		Stream << TextureData.Bytes;
		Stream << TextureData.Slices;
		return Stream;
	}

	IDataStream & operator >>(IDataStream & Stream, FTextureData & TextureData)
	{
		Stream >> TextureData.Format;
		Stream >> TextureData.Dimension;
		Stream >> TextureData.Bytes;
		Stream >> TextureData.Slices;
		return Stream;
	}
}


