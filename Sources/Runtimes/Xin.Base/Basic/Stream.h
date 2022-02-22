#pragma once

#include "Basic.Types.h"
#include "Strings.h"
#include "List.h"
#include "Formatter.h"
#include "Refer.h"

namespace Xin
{
	enum class EStreamSeek
	{
		Begin = 0,
		End,
		Current,
	};

	class BASE_API IStreamable : public ISharedRefer
	{
	public:
		IStreamable() = default;
		virtual ~IStreamable() = default;

	public:
		virtual uintx Read(byte * Data, uintx Size) = 0;
		virtual uintx Write(const byte * Data, uintx Size) = 0;
		virtual uintx Peek(byte * Data, uintx Size) { return 0; }

		virtual uintx Seek(EStreamSeek StreamSeek, intx Distance) { return 0; }
		virtual uintx Position() const { return 0; }
		virtual uintx Size() const { return 0; }

	public:
		uintx WriteBytes(TView<byte> Bytes) { return Write(Bytes.Data, Bytes.Size); }

		uintx WriteData(const void * Data, uintx Size) { return Write((const byte *)Data, Size); }
		uintx ReadData(void * Data, uintx Size) { return Read((byte *)Data, Size); }
		uintx PeekData(void * Data, uintx Size) { return Peek((byte *)Data, Size); }

		template<TriviallyCopyableC T>
		uintx WriteElement(const T & Data) { return Write((const byte *)&Data, sizeof(T)); }
		template<TriviallyCopyableC T>
		uintx ReadElement(T & Data) { return Read((byte *)&Data, sizeof(T)); }
		template<TriviallyCopyableC T>
		uintx PeekElement(T & Data) { return Peek((byte *)&Data, sizeof(T)); }

		template<TriviallyCopyableC T>
		uintx WriteElements(const T * Data, uintx Size) { return Write((const byte *)Data, sizeof(T) * Size); }
		template<TriviallyCopyableC T>
		uintx ReadElements(const T * Data, uintx Size) { return Read((byte *)Data, sizeof(T) * Size); }
		template<TriviallyCopyableC T>
		uintx PeekElements(const T * Data, uintx Size) { return Peek((byte *)Data, sizeof(T) * Size); }
	};

	class BASE_API IDataStream : public IStreamable
	{
	public:
		template<TriviallyCopyableC T>
		T ReadValue()
		{
			T Value;
			ReadData(&Value, sizeof(T));
			return Value;
		}

		template<NoneTriviallyCopyableC T>
		T ReadValue()
		{
			T Value;
			*this >> Value;
			return Value;
		}

	public:
		FStringV ReadLine();
		bool ReadLine(FString & Buffer);
		void WriteLine(FStringV String);

	public:
		int8 ReadInt8() { return ReadValue<int8>(); }
		uint8 ReadUInt8() { return ReadValue<uint8>(); }
		int16 ReadInt16() { return ReadValue<int16>(); }
		uint16 ReadUInt16() { return ReadValue<uint16>(); }
		int32 ReadInt32() { return ReadValue<int32>(); }
		uint32 ReadUInt32() { return ReadValue<uint32>(); }
		int64_t ReadInt64() { return ReadValue<int64_t>(); }
		uint64 ReadUInt64() { return ReadValue<uint64>(); }
		float32 ReadFloat32() { return ReadValue<float32>(); }
		float64 ReadFloat64() { return ReadValue<float64>(); }

		template<TriviallyCopyableC T> requires (!IsClassV<T>)
		void WriteValue(T Value)
		{
			WriteData(&Value, sizeof(T));
		}

		template<TriviallyCopyableC T> requires IsClassV<T>
		void WriteValue(const T & Value)
		{
			WriteData(&Value, sizeof(T));
		}

		template<NoneTriviallyCopyableC T>
		void WriteValue(const T & Value)
		{
			*this << Value;
		}

		void WriteInt8(int8 Value) { return WriteValue<int8>(Value); }
		void WriteUInt8(uint8 Value) { return WriteValue<uint8>(Value); }
		void WriteInt16(int16 Value) { return WriteValue<int16>(Value); }
		void WriteUInt16(uint16 Value) { return WriteValue<uint16>(Value); }
		void WriteInt32(int32 Value) { return WriteValue<int32>(Value); }
		void WriteUInt32(uint32 Value) { return WriteValue<uint32>(Value); }
		void WriteInt64(int64_t Value) { return WriteValue<int64_t>(Value); }
		void WriteUInt64(uint64 Value) { return WriteValue<uint64>(Value); }
		void WriteFloat32(float32 Value) { return WriteValue<float32>(Value); }
		void WriteFloat64(float64 Value) { return WriteValue<float64>(Value); }

		template<ScalarC T>
		IDataStream & operator >>(T & Value)
		{
			Read((byte *)&Value, sizeof(T));
			return *this;
		}

		template<ClassC T> requires IsTriviallyCopyableV<T>
		IDataStream & operator >>(T & Value)
		{
			Read((byte *)&Value, sizeof(T));
			return *this;
		}

		template<ScalarC T>
		IDataStream & operator <<(T Value)
		{
			Write((const byte *)&Value, sizeof(T));
			return *this;
		}

		template<ClassC T> requires IsTriviallyCopyableV<T>
		IDataStream & operator <<(const T & Value)
		{
			Write((const byte *)&Value, sizeof(T));
			return *this;
		}

		//------------------------------------------------ TView TSlice byte
		IDataStream & operator <<(TView<byte> Bytes)
		{
			WriteUInt32((uint32)Bytes.Size);
			Write(Bytes.Data, Bytes.Size);
			return *this;
		}

		IDataStream & operator >>(TSlice<byte> Bytes)
		{
			Read(Bytes.Data, Bytes.Size);
			return *this;
		}

		//------------------------------------------------ TList
		template<TriviallyCopyableC T>
		IDataStream & operator >>(TList<T> & List)
		{
			uint32 Size = ReadUInt32();
			List.Resize(Size, EInitializeMode::None);
			ReadData(List.Data, sizeof(T) * List.Size);
			return *this;
		}

		template<TriviallyCopyableC T>
		IDataStream & operator <<(const TList<T> & List)
		{
			WriteUInt32((uint32)List.Size);
			WriteData(List.Data, sizeof(T) * List.Size);
			return *this;
		}

		template<NoneTriviallyCopyableC T>
		IDataStream & operator >>(TList<T> & List)
		{
			uint32 Size = ReadUInt32();
			List.Resize(Size, EInitializeMode::Default);
			for (uint32 Index = 0; Index < Size; ++Index)
				*this >> List[Index];
			return *this;
		}

		template<NoneTriviallyCopyableC T>
		IDataStream & operator <<(const TList<T> & List)
		{
			WriteUInt32((uint32)List.Size);
			for (uint32 Index = 0; Index < List.Size; ++Index)
				*this << List[Index];
			return *this;
		}

		//--------------------------------------------------- FString FStringView
		template<uintx Length>
		IDataStream & operator >>(const char8_t(&Value)[Length]) { if constexpr (Length > 1) ReadData(Value, (Length - 1) * sizeof(char8_t)); return *this; }
		IDataStream & operator >>(FString & Value)
		{
			uint32 Length = ReadUInt32();
			Value.Resize(Length, Length + 1);
			ReadData(Value.Data, Value.Length * sizeof(char8));
			return *this;
		}

		IDataStream & operator >>(FAString & Value)
		{
			uint32 Length = ReadUInt32();
			Value.Resize(Length, Length);
			ReadData(Value.Data, Value.Length * sizeof(chara));
			return *this;
		}

		template<uintx Length>
		IDataStream & operator<<(const char8_t(&Value)[Length]) { if constexpr (Length > 1) WriteData(Value, (Length - 1) * sizeof(char8_t)); return *this; }
		IDataStream & operator<<(const FStringV & Value) { WriteUInt32((uint32)Value.Size); WriteData(Value.Data, Value.Size * sizeof(char8)); return *this; }
		IDataStream & operator<<(const FString & Value) { WriteUInt32((uint32)Value.Length); WriteData(Value.Data, Value.Length * sizeof(char8)); return *this; }

		template<uintx Length>
		IDataStream & operator<<(const char(&Value)[Length]) { if constexpr (Length > 1) WriteData(Value, (Length - 1) * sizeof(char)); return *this; }
		IDataStream & operator<<(const FAStringV & Value) { WriteUInt32((uint32)Value.Size); WriteData(Value.Data, Value.Size * sizeof(chara)); return *this; }
		IDataStream & operator<<(const FAString & Value) { WriteUInt32((uint32)Value.Length); WriteData(Value.Data, Value.Length * sizeof(chara)); return *this; }
	};
	using IDataStreamRef = TReferPtr<IDataStream>;

	class BASE_API FStringStream : public IStreamable
	{
	public:
		IStreamable & Stream;

	public:
		FStringStream(IStreamable & Stream) : Stream(Stream) { }

	public:
		uintx Read(byte * Data, uintx Size) override { return Stream.Read(Data, Size); }
		uintx Write(const byte * Data, uintx Size) override { return Stream.Write(Data, Size); }

	public:
		FStringStream & operator<<(char Ch) { Stream.WriteData(&Ch, 1); return *this; }
		FStringStream & operator<<(char8_t Ch) { Stream.WriteData(&Ch, 1); return *this; }

		template<uintx Length>
		FStringStream & operator<<(const char8_t(&Value)[Length]) { if constexpr (Length > 1) Stream.WriteData(Value, (Length - 1) * sizeof(char8_t)); return *this; }
		FStringStream & operator<<(const FStringV & Value) { Stream.WriteData(Value.Data, Value.Size * sizeof(char8)); return *this; }
		FStringStream & operator<<(const FString & Value) { Stream.WriteData(Value.Data, Value.Length * sizeof(char8)); return *this; }

		template<uintx Length>
		FStringStream & operator<<(const char(&Value)[Length]) { if constexpr (Length > 1) Stream.WriteData(Value, (Length - 1) * sizeof(char)); return *this; }
		FStringStream & operator<<(const FAStringV & Value) { Stream.WriteData(Value.Data, Value.Size * sizeof(chara)); return *this; }
		FStringStream & operator<<(const FAString & Value) { Stream.WriteData(Value.Data, Value.Length * sizeof(chara)); return *this; }

		template<typename T>
		FStringStream & operator << (const T & Value) requires (!IsSameV<T, FStringV> && IsFormatAbleV<T, char8>)
		{
			FStringV String = Format(u8"{}"V, Value);
			WriteData(String.Data, String.Size);
			return *this;
		}
	};


	class FNullDataStream : public IDataStream
	{
	public:
		uintx Read(byte * Data, uintx Size) override { return 0; }
		uintx Write(const byte * Data, uintx Size) override { return 0; }
	};
}
