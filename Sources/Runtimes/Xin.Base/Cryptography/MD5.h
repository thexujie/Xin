#pragma once

#include "../Basic/Basic.h"

namespace Xin
{
	using FMD5Result = TArray<char8, 32>;

	BASE_API FMD5Result MD5Encrypt(TView<byte> Bytes);
	inline FMD5Result MD5Encrypt(FStringV String)
	{
		return MD5Encrypt({ (const byte *)String.Data, String.Size });
	}

	BASE_API FStringV MD5EncryptToString(TView<byte> Bytes);
	BASE_API void MD5Encrypt(TView<byte> Bytes, TSlice<char8> HexDigests);

	inline FStringV MD5EncryptToString(FStringV String)
	{
		return MD5EncryptToString({ (const byte *)String.Data, String.Size });
	}

	inline void MD5Encrypt(FStringV String, TSlice<char8> HexDigests)
	{
		MD5Encrypt({ (const byte *)String.Data, String.Size }, HexDigests);
	}
}
