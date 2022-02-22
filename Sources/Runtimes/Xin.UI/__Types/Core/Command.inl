// 07D959288A1DD868C44153584B795CFF 2025-03-06 15:49:33
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/Command.h"

namespace Xin
{
	class ICommand_Type : public TClass<UI::ICommand>
	{
	public:
		using ICommand = UI::ICommand;

		// --- 0 Constructors
		static inline const TConstructor<ICommand> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<ICommand, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<ICommand, FType &()> Method { u8"Type"N, &ICommand::Type, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::IUObject>(),
		};

		// --- 5 Constructor
		ICommand_Type(FName Name = u8"ICommand"N) : TClass(Name, Classof<UI::IUObject>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::ICommand>
	{
		static inline ICommand_Type ICommandType { u8"ICommand"N };

		static FType & Type()
		{
			return ICommandType;
		}
	};

	FType & UI::ICommand::StaticType()
	{
		return TStaticType<UI::ICommand>().Type();
	}

	static TTypeRegister<UI::ICommand> ICommandTypeRegister { TStaticType<UI::ICommand>().Type() };
}

// Generated code end.
