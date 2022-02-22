#include "PCH.h"
#include "MD5.h"

namespace Xin
{
	class FMD5Encrypter
	{
	public:
		FMD5Encrypter();
		~FMD5Encrypter();

	public:
		void Initialize();
		void UpdateBytes(TView<byte> Bytes);
		void Finalize();

		void HexDigests(TSlice<char8> Result, bool UpperCase = true);

	private:
		static constexpr uint32 BlockSize = 64;

		void Transform(const byte Block[BlockSize]);
		static void Decode(uint32 output[], const byte input[], sizet len);
		static void Encode(uint8 Outputs[], const uint32 Inputs[], sizet len);

		bool Finalized = false;
		byte EncodeBuffer[BlockSize]; // bytes that didn't fit in last 64 byte chunk
		uint32 EncodeCount[2];   // 64bit counter for number of bits (lo, hi)
		uint32 EncodeState[4];   // digest so far
		byte EncodeDigests[16]; // the result

		// low level logic operations
		static inline uint32 F(uint32 x, uint32 y, uint32 z);
		static inline uint32 G(uint32 x, uint32 y, uint32 z);
		static inline uint32 H(uint32 x, uint32 y, uint32 z);
		static inline uint32 I(uint32 x, uint32 y, uint32 z);
		static inline uint32 rotate_left(uint32 x, int n);
		static inline void FF(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);
		static inline void GG(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);
		static inline void HH(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);
		static inline void II(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);
	};

	std::string md5(const std::string str);



	// Constants for MD5Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


///////////////////////////////////////////////

// F, G, H and I are basic MD5 functions.
	inline uint32 FMD5Encrypter::F(uint32 x, uint32 y, uint32 z) {
		return x & y | ~x & z;
	}

	inline uint32 FMD5Encrypter::G(uint32 x, uint32 y, uint32 z) {
		return x & z | y & ~z;
	}

	inline uint32 FMD5Encrypter::H(uint32 x, uint32 y, uint32 z) {
		return x ^ y ^ z;
	}

	inline uint32 FMD5Encrypter::I(uint32 x, uint32 y, uint32 z) {
		return y ^ (x | ~z);
	}

	// rotate_left rotates x left n bits.
	inline uint32 FMD5Encrypter::rotate_left(uint32 x, int n) {
		return (x << n) | (x >> (32 - n));
	}

	// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
	// Rotation is separate from addition to prevent recomputation.
	inline void FMD5Encrypter::FF(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac) {
		a = rotate_left(a + F(b, c, d) + x + ac, s) + b;
	}

	inline void FMD5Encrypter::GG(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac) {
		a = rotate_left(a + G(b, c, d) + x + ac, s) + b;
	}

	inline void FMD5Encrypter::HH(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac) {
		a = rotate_left(a + H(b, c, d) + x + ac, s) + b;
	}

	inline void FMD5Encrypter::II(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac) {
		a = rotate_left(a + I(b, c, d) + x + ac, s) + b;
	}

	//////////////////////////////////////////////

	// default ctor, just initailize
	FMD5Encrypter::FMD5Encrypter()
	{
	}

	FMD5Encrypter::~FMD5Encrypter()
	{
	}

	void FMD5Encrypter::Initialize()
	{
		Finalized = false;

		EncodeCount[0] = 0;
		EncodeCount[1] = 0;

		// load magic initialization constants.
		EncodeState[0] = 0x67452301;
		EncodeState[1] = 0xefcdab89;
		EncodeState[2] = 0x98badcfe;
		EncodeState[3] = 0x10325476;
	}

	// decodes input (unsigned char) into output (uint32). Assumes len is a multiple of 4.
	void FMD5Encrypter::Decode(uint32 output[], const byte input[], sizet len)
	{
		for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
			output[i] = ((uint32)input[j]) | (((uint32)input[j + 1]) << 8) |
			(((uint32)input[j + 2]) << 16) | (((uint32)input[j + 3]) << 24);
	}

	// encodes input (uint32) into output (unsigned char). Assumes len is
	// a multiple of 4.
	void FMD5Encrypter::Encode(uint8 Outputs[], const uint32 Inputs[], sizet len)
	{
		for (sizet i = 0, j = 0; j < len; i++, j += 4) {
			Outputs[j] = Inputs[i] & 0xff;
			Outputs[j + 1] = (Inputs[i] >> 8) & 0xff;
			Outputs[j + 2] = (Inputs[i] >> 16) & 0xff;
			Outputs[j + 3] = (Inputs[i] >> 24) & 0xff;
		}
	}

	// apply MD5 algo on a block
	void FMD5Encrypter::Transform(const byte Block[BlockSize])
	{
		uint32 a = EncodeState[0], b = EncodeState[1], c = EncodeState[2], d = EncodeState[3], x[16];
		Decode(x, Block, BlockSize);

		/* Round 1 */
		FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
		FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
		FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
		FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
		FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
		FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
		FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
		FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
		FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
		FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
		FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
		FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
		FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
		FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
		FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
		FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

		/* Round 2 */
		GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
		GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
		GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
		GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
		GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
		GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
		GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
		GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
		GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
		GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
		GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
		GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
		GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
		GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
		GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
		GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

		/* Round 3 */
		HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
		HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
		HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
		HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
		HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
		HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
		HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
		HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
		HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
		HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
		HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
		HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
		HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
		HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
		HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
		HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

		/* Round 4 */
		II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
		II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
		II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
		II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
		II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
		II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
		II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
		II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
		II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
		II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
		II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
		II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
		II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
		II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
		II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
		II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

		EncodeState[0] += a;
		EncodeState[1] += b;
		EncodeState[2] += c;
		EncodeState[3] += d;

		// Zeroize sensitive information.
		memset(x, 0, sizeof x);
	}

	// MD5 block update operation. Continues an MD5 message-digest
	// operation, processing another message block
	void FMD5Encrypter::UpdateBytes(TView<byte> Bytes)
	{
		// compute number of bytes mod 64
		sizet Index = EncodeCount[0] / 8 % BlockSize;

		// Update number of bits
		if ((EncodeCount[0] += uint32(Bytes.Size << 3)) < uint32(Bytes.Size << 3))
			EncodeCount[1]++;
		EncodeCount[1] += uint32(Bytes.Size >> 29);

		// number of bytes we need to fill in buffer
		sizet Firstpart = 64 - Index;

		sizet ByteIndex;

		// transform as many times as possible.
		if (Bytes.Size >= Firstpart)
		{
			// fill buffer first, transform
			memcpy(&EncodeBuffer[Index], Bytes.Data, Firstpart);
			Transform(EncodeBuffer);

			// transform chunks of blocksize (64 bytes)
			for (ByteIndex = Firstpart; ByteIndex + BlockSize <= Bytes.Size; ByteIndex += BlockSize)
				Transform(Bytes.Data + ByteIndex);

			Index = 0;
		}
		else
			ByteIndex = 0;

		// buffer remaining input
		memcpy(&EncodeBuffer[Index], Bytes.Data + ByteIndex, Bytes.Size - ByteIndex);
	}

	// MD5 finalization. Ends an MD5 message-digest operation, writing the
	// the message digest and zeroizing the context.
	void FMD5Encrypter::Finalize()
	{
		static byte Padding[64] = 
		{
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		if (!Finalized) {
			// Save number of bits
			uint8 Bits[8];
			Encode(Bits, EncodeCount, 8);

			// pad out to 56 mod 64.
			sizet index = EncodeCount[0] / 8 % 64;
			sizet PaddingLength = (index < 56) ? (56 - index) : (120 - index);
			UpdateBytes({ Padding, PaddingLength });

			// Append length (before padding)
			UpdateBytes({ Bits, 8 });

			// Store state in digest
			Encode(EncodeDigests, EncodeState, 16);

			// Zeroize sensitive information.
			memset(EncodeBuffer, 0, sizeof EncodeBuffer);
			memset(EncodeCount, 0, sizeof EncodeCount);

			Finalized = true;
		}
	}

	void FMD5Encrypter::HexDigests(TSlice<char8> Result, bool UpperCase)
	{
		if (!Finalized || Result.Size < 32)
			return;

		constexpr FStringV CharsLowerCase = u8"0123456789abcdef"V;
		constexpr FStringV CharsUpperCase  = u8"0123456789ABCDEF"V;

		const FStringV & Chars = UpperCase ? CharsUpperCase : CharsLowerCase;
		for (sizet DigitIndex = 0; DigitIndex < 16; ++DigitIndex)
		{
			uint8 Digit = EncodeDigests[DigitIndex];
			Result[DigitIndex * 2 + 0] = Chars[Digit >> 4];
			Result[DigitIndex * 2 + 1] = Chars[Digit & 0xF];
		}
	}

	FMD5Result MD5Encrypt(TView<byte> Bytes)
	{
		FMD5Result Result;
		FMD5Encrypter MD5Encrypter;
		MD5Encrypter.Initialize();
		MD5Encrypter.UpdateBytes(Bytes);
		MD5Encrypter.Finalize();
		MD5Encrypter.HexDigests(Result, true);
		return Result;
	}

	FStringV MD5EncryptToString(TView<byte> Bytes)
	{
		FString & Result = GetThreadLocalStringBuffer<char8>();
		Result.Resize(32);
		MD5Encrypt(Bytes, Result);
		return Result;
	}

	void MD5Encrypt(TView<byte> Bytes, TSlice<char8> Result)
	{
		FMD5Encrypter MD5Encrypter;
		MD5Encrypter.Initialize();
		MD5Encrypter.UpdateBytes(Bytes);
		MD5Encrypter.Finalize();
		MD5Encrypter.HexDigests(Result, true);
	}
}
