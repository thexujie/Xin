// 14FE825BCF0EA6318A6AB187EB75952E 2025-03-06 15:46:30
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Utils/FileInfo.h"

namespace Xin
{
	class FFileInfo_Type : public TClass<FFileInfo>
	{
	public:
		// --- 0 Constructors

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<FFileInfo, FType &()> Method { u8"Type"N, &FFileInfo::Type, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
		};

		// --- 2 Fields
		struct Field___CreationTime
		{
			static inline const TField Field { u8"CreationTime"N, &FFileInfo::CreationTime, FMetadata::None };
		};
		struct Field___Converter
		{
			static inline const TField Field { u8"Converter"N, &FFileInfo::Converter, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___CreationTime::Field,
			Field___Converter::Field,
		};

		// --- 3 Properties
		struct Property___FileIcon
		{
			static inline const TProxyProperty Property { u8"FileIcon"N, &FFileInfo::__FileIcon, &FFileInfo::__GetFileIcon, &FFileInfo::__SetFileIcon, FMetadata::None };
		};
		struct Property___FileName
		{
			static inline const TProxyProperty Property { u8"FileName"N, &FFileInfo::__FileName, &FFileInfo::__GetFileName, &FFileInfo::__SetFileName, FMetadata::None };
		};
		struct Property___FileSize
		{
			static inline const TProxyProperty Property { u8"FileSize"N, &FFileInfo::__FileSize, &FFileInfo::__GetFileSize, &FFileInfo::__SetFileSize, FMetadata::None };
		};
		struct Property___FilePath
		{
			static inline const TProxyProperty Property { u8"FilePath"N, &FFileInfo::__FilePath, &FFileInfo::__GetFilePath, &FFileInfo::__SetFilePath, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___FileIcon::Property,
			Property___FileName::Property,
			Property___FileSize::Property,
			Property___FilePath::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IDependency>(),
		};

		// --- 5 Constructor
		FFileInfo_Type(FName Name = u8"FFileInfo"N) : TClass(Name, Classof<IDependency>())
		{
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<IPixmapRef> & FFileInfo::FileIconProperty() { return FFileInfo_Type::Property___FileIcon::Property; }

	const TProperty<FString> & FFileInfo::FileNameProperty() { return FFileInfo_Type::Property___FileName::Property; }

	const TProperty<uint64> & FFileInfo::FileSizeProperty() { return FFileInfo_Type::Property___FileSize::Property; }

	const TProperty<FString> & FFileInfo::FilePathProperty() { return FFileInfo_Type::Property___FilePath::Property; }

	template<> struct XIN_EXPORT TStaticType<FFileInfo>
	{
		static inline FFileInfo_Type FFileInfoType { u8"FFileInfo"N };

		static FType & Type()
		{
			return FFileInfoType;
		}
	};

	FType & FFileInfo::StaticType()
	{
		return TStaticType<FFileInfo>().Type();
	}

	static TTypeRegister<FFileInfo> FFileInfoTypeRegister { TStaticType<FFileInfo>().Type() };
}

// Generated code end.
