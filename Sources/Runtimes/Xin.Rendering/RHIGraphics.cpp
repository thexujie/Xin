#include "PCH.h"
#include "RHIGraphics.h"

#include "CommandExecutor.h"
#include "BaseEngine.h"
#include "RHIPath.h"

namespace Xin::Rendering
{
	FRHIGraphics::FRHIGraphics(FCommandExecutor & Executor) : IGraphics(EGraphicsRHI::RHI), Executor(Executor) {}

	FRHIGraphics::~FRHIGraphics() {}

	void FRHIGraphics::Initialize(IGraphics * OfflineGraphic)
	{
		this->OfflineGraphics = OfflineGraphic;
		OfflinePainter = OfflineGraphic->CreatePainter();
	}

	void FRHIGraphics::Finalize()
	{
		OfflinePainter = nullptr;
		FontTextures.Clear();
		OfflineGraphics = nullptr;
	}

	IPainterRef FRHIGraphics::CreatePainter()
	{
		return new FRHIPainter(*this);
	}

	IPainterRef FRHIGraphics::CreateWindowPainter(voidp WindowHandle)
	{
		return new FRHIViewportPainter(*this, WindowHandle);
	}

	ITextBlobRef FRHIGraphics::CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags)
	{
		if (!OfflineGraphics)
			return nullptr;
		return OfflineGraphics->CreateTextBlob(String, FontFace, FontSize, LayoutSize, LayoutFlags);
	}

	IPathRef FRHIGraphics::CreatePath()
	{
		return new FRHIPath(*this);
	}

	IPathRef FRHIGraphics::CreateTextPath(ITextBlob & TextLayout, const Point2F & Position)
	{
		return nullptr;
		//IPathRef OfflinePath = OfflineDevice->CreateTextPath(TextLayout, Position);
		//FRHIPathRef Path = new FRHIPath(*this);

		//class FPathTessellateSink : public IPathTessellateSink
		//{
		//public:
		//	FPathTessellateSink(TList<FRHIGrphicsVertex> & Vertices) : Vertices(Vertices) {}

		//	void AddTriangle(const PointF & PointA, const PointF & PointB, const PointF & PointC) override
		//	{
		//		Vertices.AddRange({ { PointA }, { PointB }, { PointC } });
		//	}

		//	void AddTriangles(TView<PointF> Points) override
		//	{
		//		for (sizet TriangleIndex = 0; TriangleIndex < Points.Size / 3; ++TriangleIndex)
		//		{
		//			Vertices.Add({ Points[TriangleIndex * 3 + 0], {} });
		//			Vertices.Add({ Points[TriangleIndex * 3 + 1], {} });
		//			Vertices.Add({ Points[TriangleIndex * 3 + 2], {} });
		//		}
		//	}

		//public:
		//	TList<FRHIGrphicsVertex> & Vertices;
		//};

		//FPathTessellateSink PathTessellateSink { Path->Vertices };
		//OfflinePath->Tessellate(PathTessellateSink);

		//return Path;
	}

	IBitmapRef FRHIGraphics::CreateBitmap(FStringV FilePath)
	{
		IAssetStreamRef AssetStream = Executor.Engine.LoadStream(FilePath);
		if (!AssetStream)
			return nullptr;

		IImageRef Image = IImage::Load(AssetStream.Ref());
		if (!Image)
			return nullptr;

		FRHIBitmapRef Bitmap =  RHICreateBitmap(Image->ImageData);
		Bitmap->Texture->SetDebugName(AssetStream->AssetPath);
		return Bitmap;
	}

	IBitmapRef FRHIGraphics::CreateBitmap(EFormat Format, SizeU Size, EBitmapUsage BitmapUsage)
	{
		return new FRHIBitmap(*this, Size, Format, BitmapUsage);
	}

	IBitmapRef FRHIGraphics::CreateBitmap(const IPixmap & Pixmap)
	{
		return new FRHIBitmap(*this, Pixmap);
	}

	IBitmapRef FRHIGraphics::CreateSharedBitmap(void * SharedHandle, EBitmapUsage BitmapUsage)
	{
		return new FRHIBitmap(*this, InterpretCast<IRHIResource>(SharedHandle), BitmapUsage);
	}

	IBitmapRef FRHIGraphics::CreateBitmap(IRHIResource * Resource, EBitmapUsage BitmapUsage)
	{
		return new FRHIBitmap(*this, Resource, BitmapUsage);
	}

	FRHIBitmapRef FRHIGraphics::RHICreateBitmap(const FImageData & ImageData)
	{
		IRHIResourceRef Resource = Executor.Engine.Device.CreatePlacedResource(
			FTextureDesc(ImageData.Format, { ImageData.Width, ImageData.Height, ImageData.MipLevels, ImageData.NumLayers }, 
				EResourceUsage::SampledTexture | EResourceUsage::CopyWrite), Executor.SharedAllocator);
		Executor.UpdateTexture(Resource, FTextureImageSource(ImageData));
		return new FRHIBitmap(*this, Resource, EBitmapUsage::ShaderResource);
	}

	void FRHIGraphics::MapGlyphs(IFontFace & Font, float32 FontSize, TView<uint16> GlyphIndices)
	{
		if (GlyphIndices.Size == 0)
			return;

		FFontTextureRef FontTexture = GetFontTexture(Font, FontSize);
		FontTexture->MapGlyphs(GlyphIndices);
	}

	forceinline FFontTextureRef FRHIGraphics::GetFontTexture(IFontFace & Font, float32 FontSize)
	{
		IFontFaceRef FontRef { &Font };
		if (auto Iter = FontTextures.Find({ FontRef, FontSize }); Iter != FontTextures.End())
			return Iter->Second;

		FFontTextureRef FontTexture = new FFontTexture(*this, Font, FontSize);
		FontTextures[{ FontRef, FontSize }] = FontTexture;

		return FontTexture;
	}
}
