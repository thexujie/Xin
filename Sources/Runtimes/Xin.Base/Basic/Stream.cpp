#include "PCH.h"
#include "Stream.h"

namespace Xin
{
	FStringV IDataStream::ReadLine()
	{
		FString & Buffer = GetThreadLocalStringBuffer<char8>();
		ReadLine(Buffer);
		return Buffer;
	}

	bool IDataStream::ReadLine(FString & Buffer)
	{
		Buffer.Clear();
		uint8 Ch = 0;
		while (true)
		{
			uintx ReadResult = ReadData(&Ch, 1);
			if (!ReadResult || ReadResult == NullIndex)
				break;

			if (Ch == 0 || Ch == '\n')
			{
				if (Buffer.Empty())
					continue;

				break;
			}

			if (Ch == '\r')
				continue;

			// UTF-8
			if (Ch == 0xEF)
			{
				uint8 BOM_BBBF[2];
				ReadResult = ReadData(BOM_BBBF, 2);
				if (!ReadResult || ReadResult == NullIndex)
					break;

				if (BOM_BBBF[0] == 0xBB && BOM_BBBF[1] == 0xBF)
					continue;
				else
					break;
			}

			Buffer.Append(char8(Ch));
		}
		return !Buffer.Empty();
	}

	void IDataStream::WriteLine(FStringV String)
	{
		WriteData(String.Data, String.Size);
		WriteBytes({ '\r', '\n' });
	}
}
