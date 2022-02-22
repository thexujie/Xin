#pragma once

#include "RHITypes.h"
#include "RHIHeap.h"

namespace Xin::RHI
{
	class IRHIResourceView;
	class IRHICommandList;

	enum class EPageable
	{
		Resource = 0,
		ResourceView,
	};

	enum class ERHILockMode
	{
		None = 0,
		Read,
		Write,
		WriteDiscard,
	};

	struct RHI_API FTextureSlice
	{
		FTextureSlice() = default;

		FTextureSlice(EFormat Format, uint32 Width, uint32 Height, uint32 RowPitch, uint32 SlicePitch)
			: Format(Format),Width(Width),Height(Height)
			, RowPitch(RowPitch),SlicePitch(SlicePitch)
		{
			
		}

		FTextureSlice(const FImageSlice & ImageSlice)
			: Format(ImageSlice.Format),Width(ImageSlice.Width),Height(ImageSlice.Height)
			, RowPitch(ImageSlice.RowPitch),SlicePitch(ImageSlice.SlicePitch)
		{

		}

		EFormat Format = EFormat::None;
		uint32 Width = 0;
		uint32 Height = 0;
		uint32 RowPitch = 0;
		uint32 SlicePitch = 0;

		RHI_API friend IDataStream & operator <<(IDataStream & Stream, const FTextureSlice & TextureSlice);
		RHI_API friend IDataStream & operator >>(IDataStream & Stream, FTextureSlice & TextureSlice);
	};

	struct RHI_API FTextureData
	{
		FTextureData() = default;
		FTextureData(const FTextureData &) = default;
		FTextureData(FTextureData &&) noexcept = default;
		FTextureData & operator =(const FTextureData &) = default;
		FTextureData & operator =(FTextureData &&) noexcept = default;

		FTextureData(const FImageData & ImageData);

		EFormat Format = EFormat::None;
		// Width Height Miplevels Depth/NumLayers
		Size4U Dimension;

		TList<byte> Bytes;
		TList<FTextureSlice> Slices;

		RHI_API friend IDataStream & operator <<(IDataStream & Stream, const FTextureData & TextureData);
		RHI_API friend IDataStream & operator >>(IDataStream & Stream, FTextureData & TextureData);
	};

	struct RHI_API FTextureSource
	{
		FTextureSource() = default;

		FTextureSource(const FTextureData & ImageData)
			: Format(ImageData.Format)
			, Dimension(ImageData.Dimension)
			, Bytes(ImageData.Bytes)
			, Slices(ImageData.Slices)
		{ }

		EFormat Format = EFormat::None;
		// Width Height Miplevels Depth/NumLayers
		Size4U Dimension;

		TView<byte> Bytes;
		TView<FTextureSlice> Slices;
	};

	struct RHI_API FTextureImageSource : FTextureSource
	{
		FTextureImageSource() = default;

		FTextureImageSource(const FImageData & ImageData)
		{
			ImageSlices = ImageData.Slices;
			Format = ImageData.Format;
			Dimension = { ImageData.Width, ImageData.Height, 1, 1 };
			Bytes = ImageData.Bytes;
			Slices = ImageSlices;
		}

		TList<FTextureSlice> ImageSlices;
	};

	struct RHI_API FTextureBitmapSource : FTextureSource
	{
		FTextureBitmapSource() = default;

		FTextureBitmapSource(const IPixmap & Pixmap)
		{
			Slice = { Pixmap.Format, Pixmap.Size.Width, Pixmap.Size.Height, Pixmap.Pitch, uint32(Pixmap.Bytes.Size) };
			Format = Pixmap.Format;
			Dimension = { Pixmap.Size, 1, 1 };
			Bytes = Pixmap.Bytes;
			Slices = {&Slice, 1 };
		}

		FTextureBitmapSource(EFormat Format, uint32 Width, uint32 Height, uint32 RowPitch, TView<byte> Bytes)
		{
			Slice = { Format, Width, Height, RowPitch, uint32(Bytes.Size) };
			this->Format = Format;
			this->Dimension = { Width, Height, 1, 1 };
			this->Bytes = Bytes;
			this->Slices = {&Slice, 1 };
		}

		FTextureSlice Slice;
	};

	class RHI_API IRHIPageable : public IRHIObject
	{
	public:
		IRHIPageable(EPageable Pageable) : Pageable(Pageable) {}

		virtual bool IsResidential() const { return true; }
		virtual void Resident() const { }
		virtual void Evict() const { }

	public:
		EPageable Pageable = EPageable::Resource;
	};
	using IRHIPageableRef = TReferPtr<IRHIPageable>;

	class RHI_API IRHIResource : public IRHIPageable
	{
	public:
		IRHIResource(const FResourceDesc & ResourceDesc)
			: IRHIPageable(EPageable::Resource), ResourceDesc(ResourceDesc) {}

		virtual IRHIResourceView * GetResourceView(uint32 ArrayIndex = NullIndex) const = 0;

		virtual void * GetSharedHandle() const = 0;

	public:
		FRHIAddress GetBaseAddress() const { return Memory.PlaceHeap ? Memory.PlaceHeap->BaseAddress : FRHIAddress{}; }

	public:
		FResourceDesc ResourceDesc;
		FRHIMemory Memory;
		FRHIAddress BaseAddress;
		FRHILocation Location;
	};
	using IRHIResourceRef = TReferPtr<IRHIResource>;


	class RHI_API IRHIResourceView : public IRHIPageable
	{
	public:
		IRHIResourceView(IRHIResource & Resource, const FResourceViewDesc & ViewDesc)
			: IRHIPageable(EPageable::ResourceView), Resource(Resource), ViewDesc(ViewDesc) {}
		~IRHIResourceView() = default;

		virtual IRHIResource * GetResource() const = 0;

	public:
		IRHIResource & Resource;
		FResourceViewDesc ViewDesc;
	};
	using IRHIResourceViewRef = TReferPtr<IRHIResourceView>;

	struct FRHITextureCreation
	{
		Size4U Size;
		EFormat Format;
		EResourceDimension Dimension;
		EResourceUsage Usage;
		EResourceAccess InitialAccess;
		FClearValue ClearValue;
	};
}
