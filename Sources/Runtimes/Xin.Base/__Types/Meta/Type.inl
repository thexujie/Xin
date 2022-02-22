// 14FC1CD657EC10962887DA5E5F01D0A9 2024-12-25 11:51:49
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Meta/Type.h"

namespace Xin
{
	// -------------------------------- void --------------------------------
	class void_Type : public TBuiltinType<void>
	{
	public:
		void_Type() : TBuiltinType<void>(u8"void"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<void>
	{
		static FType & Type()
		{
			static void_Type voidType;
			return voidType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<void>()
	{
		return TStaticType<void>().Type();
	}

	static TTypeRegister<void> voidTypeRegister { TStaticType<void>().Type() };

	// -------------------------------- bool --------------------------------
	class bool_Type : public TBuiltinType<bool>
	{
	public:
		bool_Type() : TBuiltinType<bool>(u8"bool"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<bool>
	{
		static FType & Type()
		{
			static bool_Type boolType;
			return boolType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<bool>()
	{
		return TStaticType<bool>().Type();
	}

	static TTypeRegister<bool> boolTypeRegister { TStaticType<bool>().Type() };

	// -------------------------------- nullp --------------------------------
	class nullp_Type : public TBuiltinType<nullp>
	{
	public:
		nullp_Type() : TBuiltinType<nullp>(u8"nullp"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<nullp>
	{
		static FType & Type()
		{
			static nullp_Type nullpType;
			return nullpType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<nullp>()
	{
		return TStaticType<nullp>().Type();
	}

	static TTypeRegister<nullp> nullpTypeRegister { TStaticType<nullp>().Type() };

	// -------------------------------- int8 --------------------------------
	class int8_Type : public TBuiltinType<int8>
	{
	public:
		int8_Type() : TBuiltinType<int8>(u8"int8"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<int8>
	{
		static FType & Type()
		{
			static int8_Type int8Type;
			return int8Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<int8>()
	{
		return TStaticType<int8>().Type();
	}

	static TTypeRegister<int8> int8TypeRegister { TStaticType<int8>().Type() };

	// -------------------------------- uint8 --------------------------------
	class uint8_Type : public TBuiltinType<uint8>
	{
	public:
		uint8_Type() : TBuiltinType<uint8>(u8"uint8"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<uint8>
	{
		static FType & Type()
		{
			static uint8_Type uint8Type;
			return uint8Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<uint8>()
	{
		return TStaticType<uint8>().Type();
	}

	static TTypeRegister<uint8> uint8TypeRegister { TStaticType<uint8>().Type() };

	// -------------------------------- int16 --------------------------------
	class int16_Type : public TBuiltinType<int16>
	{
	public:
		int16_Type() : TBuiltinType<int16>(u8"int16"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<int16>
	{
		static FType & Type()
		{
			static int16_Type int16Type;
			return int16Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<int16>()
	{
		return TStaticType<int16>().Type();
	}

	static TTypeRegister<int16> int16TypeRegister { TStaticType<int16>().Type() };

	// -------------------------------- uint16 --------------------------------
	class uint16_Type : public TBuiltinType<uint16>
	{
	public:
		uint16_Type() : TBuiltinType<uint16>(u8"uint16"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<uint16>
	{
		static FType & Type()
		{
			static uint16_Type uint16Type;
			return uint16Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<uint16>()
	{
		return TStaticType<uint16>().Type();
	}

	static TTypeRegister<uint16> uint16TypeRegister { TStaticType<uint16>().Type() };

	// -------------------------------- int32 --------------------------------
	class int32_Type : public TBuiltinType<int32>
	{
	public:
		int32_Type() : TBuiltinType<int32>(u8"int32"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<int32>
	{
		static FType & Type()
		{
			static int32_Type int32Type;
			return int32Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<int32>()
	{
		return TStaticType<int32>().Type();
	}

	static TTypeRegister<int32> int32TypeRegister { TStaticType<int32>().Type() };

	// -------------------------------- uint32 --------------------------------
	class uint32_Type : public TBuiltinType<uint32>
	{
	public:
		uint32_Type() : TBuiltinType<uint32>(u8"uint32"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<uint32>
	{
		static FType & Type()
		{
			static uint32_Type uint32Type;
			return uint32Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<uint32>()
	{
		return TStaticType<uint32>().Type();
	}

	static TTypeRegister<uint32> uint32TypeRegister { TStaticType<uint32>().Type() };

	// -------------------------------- int64 --------------------------------
	class int64_Type : public TBuiltinType<int64>
	{
	public:
		int64_Type() : TBuiltinType<int64>(u8"int64"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<int64>
	{
		static FType & Type()
		{
			static int64_Type int64Type;
			return int64Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<int64>()
	{
		return TStaticType<int64>().Type();
	}

	static TTypeRegister<int64> int64TypeRegister { TStaticType<int64>().Type() };

	// -------------------------------- uint64 --------------------------------
	class uint64_Type : public TBuiltinType<uint64>
	{
	public:
		uint64_Type() : TBuiltinType<uint64>(u8"uint64"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<uint64>
	{
		static FType & Type()
		{
			static uint64_Type uint64Type;
			return uint64Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<uint64>()
	{
		return TStaticType<uint64>().Type();
	}

	static TTypeRegister<uint64> uint64TypeRegister { TStaticType<uint64>().Type() };

	// -------------------------------- float32 --------------------------------
	class float32_Type : public TBuiltinType<float32>
	{
	public:
		float32_Type() : TBuiltinType<float32>(u8"float32"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<float32>
	{
		static FType & Type()
		{
			static float32_Type float32Type;
			return float32Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<float32>()
	{
		return TStaticType<float32>().Type();
	}

	static TTypeRegister<float32> float32TypeRegister { TStaticType<float32>().Type() };

	// -------------------------------- float64 --------------------------------
	class float64_Type : public TBuiltinType<float64>
	{
	public:
		float64_Type() : TBuiltinType<float64>(u8"float64"N) { }
	};

	template<> struct XIN_EXPORT TStaticType<float64>
	{
		static FType & Type()
		{
			static float64_Type float64Type;
			return float64Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<float64>()
	{
		return TStaticType<float64>().Type();
	}

	static TTypeRegister<float64> float64TypeRegister { TStaticType<float64>().Type() };
}

// Generated code end.
