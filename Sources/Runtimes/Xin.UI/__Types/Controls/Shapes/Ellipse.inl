// 937C120667DF9A2D5608F9837BD02CC7 2025-03-06 15:49:18
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Shapes/Ellipse.h"

namespace Xin
{
	class UEllipse_Type : public TClass<UI::UEllipse>
	{
	public:
		using UEllipse = UI::UEllipse;

		// --- 0 Constructors
		static inline const TConstructor<UEllipse> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UEllipse, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UEllipse, FType &()> Method { u8"Type"N, &UEllipse::Type, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UEllipse, void(IPainter &)> Method { u8"OnPaint"N, &UEllipse::OnPaint, FMetadata::None };
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
			struct FPropertyMetadata : UEllipse::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"Radius"N, &UEllipse::__Radius, &UEllipse::__GetRadius, &UEllipse::__SetRadius, Metadata };
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
		UEllipse_Type(FName Name = u8"UEllipse"N) : TClass(Name, Classof<UI::UShape>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FSize> & UI::UEllipse::RadiusProperty() { return UEllipse_Type::Property___Radius::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UEllipse>
	{
		static inline UEllipse_Type UEllipseType { u8"UEllipse"N };

		static FType & Type()
		{
			return UEllipseType;
		}
	};

	FType & UI::UEllipse::StaticType()
	{
		return TStaticType<UI::UEllipse>().Type();
	}

	static TTypeRegister<UI::UEllipse> UEllipseTypeRegister { TStaticType<UI::UEllipse>().Type() };
}

// Generated code end.
