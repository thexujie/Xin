// FAD2EDCAAE56C95B652FAAE9F7C2C493 2025-03-26 16:24:47
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ScrollViewer.h"

namespace Xin
{
	class UScrollViewer_Type : public TClass<UI::UScrollViewer>
	{
	public:
		using UScrollViewer = UI::UScrollViewer;

		// --- 0 Constructors
		static inline const TConstructor<UScrollViewer> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UScrollViewer, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UScrollViewer, FType &()> Method { u8"Type"N, &UScrollViewer::Type, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UScrollViewer, FSize(FSize)> Method { u8"OnMeasure"N, &UScrollViewer::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UScrollViewer, void()> Method { u8"OnArrange"N, &UScrollViewer::OnArrange, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPresenterControl>(),
		};

		// --- 5 Constructor
		UScrollViewer_Type(FName Name = u8"UScrollViewer"N) : TClass(Name, Classof<UI::UPresenterControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UScrollViewer>
	{
		static inline UScrollViewer_Type UScrollViewerType { u8"UScrollViewer"N };

		static FType & Type()
		{
			return UScrollViewerType;
		}
	};

	FType & UI::UScrollViewer::StaticType()
	{
		return TStaticType<UI::UScrollViewer>().Type();
	}

	static TTypeRegister<UI::UScrollViewer> UScrollViewerTypeRegister { TStaticType<UI::UScrollViewer>().Type() };
}

// Generated code end.
