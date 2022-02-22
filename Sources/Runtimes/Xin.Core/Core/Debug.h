#pragma once

#include "Core.Types.h"

namespace Xin
{
	class FDebugValue;

	class CORE_API FDebug
	{
	public:
		FDebug();

		void OnKeyDown(FKey Key);

	public:
		TList<FDebugValue *> DebugValues;

	public:
		static FDebug & Instance();

		static void Break();
		static void WriteLine(FAStringV String);
		static void WriteLine(FWStringV String);
		static void WriteLine(FUStringV String);

		template<typename ...ArgsT>
		static void WriteLineF(FAStringV Formal, ArgsT &&...Args)
		{
#ifdef XIN_DEBUG
			WriteLine(Xin::Format(Formal, Forward<ArgsT>(Args)...));
#endif
		}

		template<typename ...ArgsT>
		static void WriteLineF(FWStringV Formal, ArgsT &&...Args)
		{
#ifdef XIN_DEBUG
			WriteLine(Xin::Format(Formal, Forward<ArgsT>(Args)...));
#endif
		}
		template<typename ...ArgsT>
		static void WriteLineF(FUStringV Formal, ArgsT &&...Args)
		{
#ifdef XIN_DEBUG
			WriteLine(Xin::Format(Formal, Forward<ArgsT>(Args)...));
#endif
		}
	};

	class CORE_API FDebugValue
	{
	public:
		FDebugValue();
		virtual ~FDebugValue();
		virtual void OnKeyDown(FKey Key) {}
	};

	template<typename T>
	concept KeybordInteractiveLambdaTC = requires(T Lambda, FKey Key)
	{
		Lambda(Key);
	};

	template<KeybordInteractiveLambdaTC LambdaT>
	class TkeybordInteractiveDebug : public FDebugValue
	{
	public:
		TkeybordInteractiveDebug(LambdaT Lambda) : Lambda(Move(Lambda)) {}

		void OnKeyDown(FKey Key) override { Lambda(Key); }

	private:
		LambdaT Lambda;
	};

	template<typename T, typename ValueT>
	concept KeybordInteractiveValueLambdaTC = requires(T Lambda, FKey Key, ValueT & Value)
	{
		Lambda(Value, Key);
	};

	template<typename ValueT, KeybordInteractiveValueLambdaTC<ValueT> LambdaT>
	class TkeybordInteractiveDebugValue : public FDebugValue
	{
	public:
		TkeybordInteractiveDebugValue(const ValueT & InitValue, LambdaT Lambda) : Value(InitValue), Lambda(Move(Lambda)) {}

		void OnKeyDown(FKey Key) override { Lambda(Value, Key); }

		operator ValueT & () { return Value; }
		operator const ValueT & () const { return Value; }

	public:
		ValueT Value = {};

	private:
		LambdaT Lambda;
	};
}

