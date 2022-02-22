#pragma once

#include "../Core/Dependency.h"
#include "../X2D/Bitmap.h"

namespace Xin
{
	using FFileInfoRef = TReferPtr<struct FFileInfo>;

	struct CORE_API FFileInfo : IDependency
	{
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		FDateTime CreationTime;

		ProxyPropertyRW(IPixmapRef, FileIcon);
		ProxyPropertyRW(FString, FileName);
		ProxyPropertyRW(uint64, FileSize) = 0;
		ProxyPropertyRW(FString, FilePath);

		IValueConverterRef Converter = nullptr;

		static TList<FFileInfoRef> EnumPath(FStringV DirectoryPath);
		static IPixmapRef GetFileIcon(FStringV FilePath, SizeU IconSize = { 24, 24 });
	};
}
