#pragma once

#include "X2D.Types.h"

namespace Xin
{
	struct FBitmapDesc
	{
		SizeU Size = {};
		EFormat Format = EFormat::None;
		EBitmapUsage Usage = EBitmapUsage::None;
	};

	enum class EBitmapLock
	{
		None = 0,
		Read,
		Write,
		Discard,
	};

	/**
	 * Surpport actual pixels and rowbytes.
	 */
	class IPixmap  : public IX2DObject
	{
	public:
		virtual ~IPixmap() = default;

	public:
		EFormat Format = EFormat::None;
		SizeU Size = {};
		TView<byte> Bytes;
		uint32 Pitch = 0;
	};
	using IPixmapRef = TReferPtr<IPixmap>;

	struct FLockedBitmap
	{
		~FLockedBitmap();

		class IBitmap * Bitmap = nullptr;
		byte * Bytes = nullptr;
		uint32 Pitch = 0;

		explicit operator bool() const { return !!Bitmap; }
	};

	class CORE_API IBitmap : public IX2DObject
	{
	public:
		IBitmap() = default;
		IBitmap(EFormat Format, SizeU Size, EBitmapUsage Usage = EBitmapUsage::ShaderResource) : Size(Size), Format(Format), Usage(Usage) {}
		IBitmap(const FBitmapDesc & BitmapDesc) : Size(BitmapDesc.Size), Format(BitmapDesc.Format), Usage(BitmapDesc.Usage) {}
		~IBitmap() = default;
		virtual IGraphics & GetGraphics() const = 0;

	public:
		virtual void AcquireSurface() {}
		virtual void ReleaseSurface() {}
		virtual bool Save(FStringV FilePath, EImageFormat ImageFormat = EImageFormat::BMP) const { return false; }
	
		virtual FLockedBitmap Lock(EBitmapLock BitmapLock) = 0;

		/**
		 * FLockedBitmap will unlock automaticly, so you shouldn't call Unkock by yourself.
		 */
		virtual void Unlock() = 0;

	public:
		SizeU Size;
		EFormat Format = EFormat::None;
		EBitmapUsage Usage = EBitmapUsage::None;
	};
	using IBitmapRef = TReferPtr<IBitmap>;

	inline FLockedBitmap::~FLockedBitmap()
	{
		if (Bitmap)
		{
			Bitmap->Unlock();
			Bitmap = nullptr;
		}

		Bytes = nullptr;
		Pitch = 0;
	}

}
