// F3E09267DB0A240FD16C74F0B0F93CC1 2025-03-06 15:49:24
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Shapes/Rectangle.h"

namespace Xin
{
	class URectangle_Type : public TClass<UI::URectangle>
	{
	public:
		using URectangle = UI::URectangle;

		// --- 0 Constructors
		static inline const TConstructor<URectangle> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<URectangle, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<URectangle, FType &()> Method { u8"Type"N, &URectangle::Type, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<URectangle, void(IPainter &)> Method { u8"OnPaint"N, &URectangle::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Radius
		{
			struct FPropertyMetadata : URectangle::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"Radius"N, &URectangle::__Radius, &URectangle::__GetRadius, &URectangle::__SetRadius, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Radius::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UShape>(),
		};

		// --- 5 Constructor
		URectangle_Type(FName Name = u8"URectangle"N) : TClass(Name, Classof<UI::UShape>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FSize> & UI::URectangle::RadiusProperty() { return URectangle_Type::Property___Radius::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::URectangle>
	{
		static inline URectangle_Type URectangleType { u8"URectangle"N };

		static FType & Type()
		{
			return URectangleType;
		}
	};

	FType & UI::URectangle::StaticType()
	{
		return TStaticType<UI::URectangle>().Type();
	}

	static TTypeRegister<UI::URectangle> URectangleTypeRegister { TStaticType<UI::URectangle>().Type() };
}

// Generated code end.
