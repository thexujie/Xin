#pragma once

#include "Basic.Types.h"
#include "Stream.h"

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quat.h"
#include "Matrix.h"
#include "Matrix3x3.h"
#include "Affine.h"

namespace Xin
{
	enum class EArchiveFlag
	{
		None = 0,
		Input= 0x0001,
		Output = 0x0002,
	};
	template class TEnumWrapper<EArchiveFlag>;

	class BASE_API FArchive : public IDataStream
	{
	public:
		FArchive(IDataStream & DataStream, EArchiveFlag ArchiveFlags = EArchiveFlag::None) : DataStream(DataStream), ArchiveFlags(ArchiveFlags) {}
		~FArchive() = default;

		bool IsInput() const { return HasFlags(ArchiveFlags, EArchiveFlag::Input); }
		uintx Read(byte * Data, uintx Size) override { return DataStream.Read(Data, Size); }
		uintx Write(const byte * Data, uintx Size) override { return DataStream.Write(Data, Size); }
		
		template<typename T>
		void Write(const T & Value)
		{
			if (!IsInput())
				DataStream << Value;
		}

		template<typename T>
		T Read()
		{
			if (IsInput())
			{
				T Value;
				IDataStream::Read(&Value, sizeof(T));
				return Value;
			}
			else
				throw EError::InvalidOperation;
		}
		
		//--------------------------------------------------------
		template<typename T>
		requires IsTriviallyCopyConstructibleV<T>
		FArchive & operator >>(T & Value)
		{
			if constexpr (IsTriviallyCopyConstructibleV<T>)
			{
				if (IsInput())
					DataStream.ReadData(&Value, sizeof(T));
				else
					DataStream.WriteData(&Value, sizeof(T));
			}
			else
			{
				static_assert(FalseTypeV<T>, "T is not Archivable");
			}
			return *this;
		}

		template<TFlagTypeC T>
		void operator >>(T & Value)
		{
			if (IsInput())
				DataStream >> Value.Get();
			else
				DataStream << Value.Get();
		}

	protected:
		IDataStream & DataStream;

	public:
		EArchiveFlag ArchiveFlags = EArchiveFlag::None;
	};


	// Archivable
	template<typename T>
	concept ArchivableC = requires(FArchive & Archive, T & Instance)
	{
		Archive >> Instance;
	};
	template<typename T>
	inline constexpr bool IsArchivableV = false;
	template<ArchivableC T>
	inline constexpr bool IsArchivableV<T> = true;
	template<typename T>
	struct IsArchivable : BoolConstant<IsArchivableV<T>> {};

	template<typename T>
	concept TriviallyArchivableC = requires(FArchive & Archive, T & Instance)
	{
		Archive.template operator>> <T>(Instance);
	};
	template<typename T>
	inline constexpr bool IsTriviallyArchivableV = false;
	template<TriviallyArchivableC T>
	inline constexpr bool IsTriviallyArchivableV<T> = true;
	template<typename T>
	struct IsTriviallyArchivable : BoolConstant<IsTriviallyArchivableV<T>> {};

	template<CharacterC T>
	FArchive & operator >>(FArchive & Serializer, const TString<T> & String)
	{
		if (!Serializer.IsInput())
		{
			Serializer.Write(uint32(String.GetSize()));
			Serializer.WriteElements(String.Data, String.Size);
		}
		return Serializer;
	}

	template<CharacterC T>
	FArchive & operator >>(FArchive & Serializer, const TView<T> & String)
	{
		if (Serializer.IsInput())
		{
			uint32 Length = Serializer.Read<uint32>();
			String.Resize(Length, Length + 1);
			Serializer.Read(String.Data, sizeof(T) * Length);
			String.FinishNull();
		}
		else
		{
			Serializer.Write(uint32(String.GetSize()));
			Serializer.Write(String.GetData(), sizeof(T) * String.GetSize());
		}
		return Serializer;
	}

	template<typename T>
	FArchive & operator >>(FArchive & Archive, TList<T> & Vector)
	{
		if (Archive.IsInput())
		{
			uint32 Length = Archive.Read<uint32>();
			Vector.Resize(Length);
		}
		else
		{
			Archive.Write(uint32(Vector.GetSize()));
		}

		if constexpr (IsArchivableV<T>)
		{
			if constexpr (IsTriviallyArchivableV<T>)
			{
				if (Archive.IsInput())
					Archive.ReadElements(Vector.Data, Vector.Size);
				else
					Archive.WriteElements(Vector.Data, Vector.Size);
			}
			else
			{
				for (uint32 Index = 0; Index < Vector.Size; ++Index)
					Archive >> Vector[Index];
			}
		}
		else
		{
			static_assert(FalseTypeV<T>, "T is not Archivable");
		}
		return Archive;
	}

	template<typename T>
	FArchive & operator >>(FArchive & Archive, TVec2<T> & Vec2)
	{
		Archive >> Vec2.X;
		Archive >> Vec2.Y;
		return Archive;
	}

	template<typename T>
	FArchive & operator >>(FArchive & Archive, TVec3<T> & Vec3)
	{
		Archive >> Vec3.X;
		Archive >> Vec3.Y;
		Archive >> Vec3.Z;
		return Archive;
	}

	template<typename T>
	FArchive & operator >>(FArchive & Archive, TVec4<T> & Vec4)
	{
		Archive >> Vec4.X;
		Archive >> Vec4.Y;
		Archive >> Vec4.Z;
		Archive >> Vec4.W;
		return Archive;
	}

	inline FArchive & operator >> (FArchive & Serializer, FVector2 & Vector2)
	{
		Serializer >> Vector2.X;
		Serializer >> Vector2.Y;
		return Serializer;
	}

	inline FArchive & operator >>(FArchive & Serializer, FVector3 & Vector3)
	{
		Serializer >> Vector3.X;
		Serializer >> Vector3.Y;
		Serializer >> Vector3.Z;
		return Serializer;
	}
	
	inline FArchive & operator >>(FArchive & Serializer, FVector4 & Vector4)
	{
		Serializer >> Vector4.X;
		Serializer >> Vector4.Y;
		Serializer >> Vector4.Z;
		Serializer >> Vector4.W;
		return Serializer;
	}
	
	inline FArchive & operator >>(FArchive & Serializer, FQuat & Quat)
	{
		Serializer >> Quat.X;
		Serializer >> Quat.Y;
		Serializer >> Quat.Z;
		Serializer >> Quat.W;
		return Serializer;
	}

	inline FArchive & operator >>(FArchive & Serializer, FMatrix & Matrix)
	{
		Serializer >> Matrix.Row0;
		Serializer >> Matrix.Row1;
		Serializer >> Matrix.Row2;
		Serializer >> Matrix.Row3;
		return Serializer;
	}

	inline FArchive & operator >>(FArchive & Serializer, FMatrix3x3 & Affine)
	{
		Serializer >> Affine.Row0;
		Serializer >> Affine.Row1;
		Serializer >> Affine.Row2;
		return Serializer;
	}
	
	inline FArchive & operator >>(FArchive & Serializer, FAffine & Affine)
	{
		Serializer >> Affine.Row0;
		Serializer >> Affine.Row1;
		Serializer >> Affine.Row2;
		return Serializer;
	}
}

