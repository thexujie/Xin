#pragma once

#include "../Basic/Basic.h"

namespace Xin
{
	class BASE_API FConsole
	{
	public:
		FConsole() = delete;

	public:
		static FEncoding GetEncoding();
		static FEncoding SetEncoding(FEncoding Encoding);

		static int32 ReadKey(FStringV TipText = FStringV::None);
		static void Clear();

	public:
		static void Write(FAStringV String);
		static void Write(FWStringV String);
		static void Write(FUStringV String);

		static void WriteLine();
		static void WriteLine(FAStringV String);
		static void WriteLine(FWStringV String);
		static void WriteLine(FUStringV String);

		template<typename ...ArgsT>
		static void Write(FAStringV Formal, ArgsT &&...Args)
		{
			Write(Xin::Format(Formal, Forward<ArgsT>(Args)...));
		}

		template<typename ...ArgsT>
		static void Write(FWStringV Formal, ArgsT &&...Args)
		{
			Write(Xin::Format(Formal, Forward<ArgsT>(Args)...));
		}

		template<typename ...ArgsT>
		static void Write(FUStringV Formal, ArgsT &&...Args)
		{
			Write(Xin::Format(Formal, Forward<ArgsT>(Args)...));
		}

		template<typename ...ArgsT>
		static void WriteLine(FAStringV Formal, ArgsT &&...Args)
		{
			WriteLine(Xin::Format(Formal, Forward<ArgsT>(Args)...));
		}

		template<typename ...ArgsT>
		static void WriteLine(FWStringV Formal, ArgsT &&...Args)
		{
			WriteLine(Xin::Format(Formal, Forward<ArgsT>(Args)...));
		}

		template<typename ...ArgsT>
		static void WriteLine(FUStringV Formal, ArgsT &&...Args)
		{
			WriteLine(Xin::Format(Formal, Forward<ArgsT>(Args)...));
		}
	};
}

