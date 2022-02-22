// 43ECAA9AA72DE2CA2477A3F4268D2380 2025-03-24 18:06:18
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/GridPanel.h"

namespace Xin
{
	class UGridPanel_Type : public TClass<UI::UGridPanel>
	{
	public:
		using UGridPanel = UI::UGridPanel;

		// --- 0 Constructors
		static inline const TConstructor<UGridPanel> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UGridPanel, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UGridPanel, FType &()> Method { u8"Type"N, &UGridPanel::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UGridPanel, void()> Method { u8"OnInitialize"N, &UGridPanel::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UGridPanel, FSize(FSize)> Method { u8"OnMeasure"N, &UGridPanel::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UGridPanel, void()> Method { u8"OnArrange"N, &UGridPanel::OnArrange, FMetadata::None };
		};
		struct Method___void___OnSizeChanged__TVec2__FDimen___const_lref
		{
			static inline const TMethod<UGridPanel, void(UI::FDimen2 const &)> Method { u8"OnSizeChanged"N, &UGridPanel::OnSizeChanged, FMetadata::None };
		};
		struct Method___FGridDimen___GetRow__uintx____const
		{
			static inline const TMethod<UGridPanel, UI::FGridDimen(uintx)> Method { u8"GetRow"N, &UGridPanel::GetRow, FMetadata::None };
		};
		struct Method___FGridDimen___GetCol__uintx____const
		{
			static inline const TMethod<UGridPanel, UI::FGridDimen(uintx)> Method { u8"GetCol"N, &UGridPanel::GetCol, FMetadata::None };
		};
		struct Method___TVec2__uint32_____GetGridSize____const
		{
			static inline const TMethod<UGridPanel, SizeU()> Method { u8"GetGridSize"N, &UGridPanel::GetGridSize, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnSizeChanged__TVec2__FDimen___const_lref::Method,
			Method___FGridDimen___GetRow__uintx____const::Method,
			Method___FGridDimen___GetCol__uintx____const::Method,
			Method___TVec2__uint32_____GetGridSize____const::Method,
		};

		// --- 2 Fields
		struct Field___Rows
		{
			static inline const TField Field { u8"Rows"N, &UGridPanel::Rows, FMetadata::None };
		};
		struct Field___Cols
		{
			static inline const TField Field { u8"Cols"N, &UGridPanel::Cols, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Rows::Field,
			Field___Cols::Field,
		};

		// --- 3 Properties
		struct Property___GridSize
		{
			static inline const TFunctorProperty Property { u8"GridSize"N, &UGridPanel::GetGridSize, nullptr, UGridPanel::FPropertyMetadata::None };
		};
		struct Property___GridLocation
		{
			static inline const TAttachedProperty<UGridPanel, Vec2U> Property { u8"GridLocation"N, UGridPanel::__GridLocation__, UGridPanel::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___GridSize::Property,
			Property___GridLocation::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPanel>(),
		};

		// --- 5 Constructor
		UGridPanel_Type(FName Name = u8"UGridPanel"N) : TClass(Name, Classof<UI::UPanel>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<Vec2U> & UI::UGridPanel::GridLocationProperty() { return UGridPanel_Type::Property___GridLocation::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UGridPanel>
	{
		static inline UGridPanel_Type UGridPanelType { u8"UGridPanel"N };

		static FType & Type()
		{
			return UGridPanelType;
		}
	};

	FType & UI::UGridPanel::StaticType()
	{
		return TStaticType<UI::UGridPanel>().Type();
	}

	static TTypeRegister<UI::UGridPanel> UGridPanelTypeRegister { TStaticType<UI::UGridPanel>().Type() };
}

// Generated code end.
