#pragma once

#include "IO.Types.h"

namespace Xin
{
	class BASE_API FMemoryStream : public IDataStream
	{
	public:
		FMemoryStream() = default;
		~FMemoryStream() = default;

		uintx Read(byte * Data, uintx Size) override
		{
			MemoryCopy(Data, Buffer.GetData() + Position, Size);
			Position += Size;
			return Size;
		}

		uintx Write(const byte * Data, uintx Size) override
		{
			Buffer.AddRange({ Data, Size });
			Position += Size;
			return Size;
		}

		TList<byte> & GetBuffer() { return Buffer; }
		const TList<byte> & GetBuffer() const { return Buffer; }
		
		void Reset()
		{
			Position = 0;
		}

	private:
		TList<byte> Buffer;
		uintx Position = 0;
	};

	class BASE_API FBufferredStream : public IDataStream
	{
	public:
		FBufferredStream(IDataStream & Stream, uintx BufferSize = 1024) : Stream(Stream), Buffer(BufferSize, EInitializeMode::None) {}
		~FBufferredStream()
		{
			if (NeedFlushWriteOnClose)
				FlushWrite();
		}

		uintx Peek(byte * Data, uintx Size) override
		{
			if (Current + Size < Capacity)
			{
				MemoryCopy(Data, Buffer.Data + Current, Size);
				return Size;
			}
			else
			{
				uintx SavedPosition = Position();

				uintx Readed = 0;
				while(Readed < Size)
				{
					uintx Need = Size - Readed;
					if (Current + Need <= Capacity)
					{
						MemoryCopy(Data + Readed, Buffer.Data + Current, Need);
						Readed += Need;
						break;
					}
					else
					{
						MemoryCopy(Data + Readed, Buffer.Data + Current, Capacity - Current);
						Readed += Capacity - Current;
					}

					if (!FlushRead())
						break;
				}

				uintx StreamPosition = Stream.Position();
				if (StreamPosition - Capacity <= SavedPosition && SavedPosition < StreamPosition)
				{
					Current = SavedPosition - (StreamPosition - Capacity);
				}
				else
				{
					Stream.Seek(EStreamSeek::Begin, SavedPosition);
					Current = 0;
					Capacity = 0;
				}
				return Readed;
			}
		}

		uintx Read(byte * Data, uintx Size) override
		{
			uintx Readed = 0;
			while (Readed < Size)
			{
				uintx NeedRead = Size - Readed;
				if (Current + NeedRead <= Capacity)
				{
					MemoryCopy(Data + Readed, Buffer.Data + Current, NeedRead);
					Current += NeedRead;
					Readed += NeedRead;
					break;
				}
				else
				{
					MemoryCopy(Data + Readed, Buffer.Data + Current, Capacity - Current);
					Readed += Capacity - Current;
					Current = 0; // ReadPrepare will do this too.
				}

				if (!FlushRead())
					return Readed;
			}

			return Readed;
		}

		uintx Write(const byte * Data, uintx Size) override
		{
			uintx Writen = 0;
			while(Writen < Size)
			{
				uintx NeedWrite = Size - Writen;
				if (Current + NeedWrite < Buffer.Size)
				{
					MemoryCopy(Buffer.Data + Current, Data + Writen, NeedWrite);
					Current += NeedWrite;
					Writen += NeedWrite;
					break;
				}
				else
				{
					Stream.Write(Buffer.Data, Current);
					if (Stream.Write(Data + Writen, Buffer.Size - Current))
						Writen += Buffer.Size - Current;
					else
						break;
					Current = 0;
				}
			}

			NeedFlushWriteOnClose = true;
			return Writen;
		}

		uintx Position() const override { return Stream.Position() - Capacity + Current; }
		uintx Size() const override { return Stream.Size(); }

	public:
		uintx FlushRead()
		{
			Capacity = Stream.Read(Buffer.Data, Buffer.Size);
			Current = 0;
			NeedFlushWriteOnClose = false;
			return Capacity;
		}

		uintx FlushWrite()
		{
			if (Current)
			{
				uintx Writen = Stream.Read(Buffer.Data, Current);
				return Writen;
			}
			return 0;
		}

	public:
		IDataStream & Stream;
		TList<byte> Buffer;

		uintx Capacity = 0;
		uintx Current = 0;

		bool NeedFlushWriteOnClose = false;
	};
}