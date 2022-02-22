// 99E6AEB23AC8E3CE2C1543F90C2DF4C0 2025-03-31 14:39:37
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Xin.UI.Types.h"

namespace Xin
{
	class EStretch_Type : public TEnum<UI::EStretch>
	{
	public:
		// --- 1 Items
		struct Item___None
		{
			static inline const TEnumItem Item { u8"None"N, UI::EStretch::None, FMetadata::None };
		};
		struct Item___Fill
		{
			static inline const TEnumItem Item { u8"Fill"N, UI::EStretch::Fill, FMetadata::None };
		};
		struct Item___Uniform
		{
			static inline const TEnumItem Item { u8"Uniform"N, UI::EStretch::Uniform, FMetadata::None };
		};
		struct Item___UniformFill
		{
			static inline const TEnumItem Item { u8"UniformFill"N, UI::EStretch::UniformFill, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___None::Item,
			Item___Fill::Item,
			Item___Uniform::Item,
			Item___UniformFill::Item,
		};

		// --- 2 Constructor
		EStretch_Type(FName Name = u8"EStretch"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::EStretch>
	{
		static inline EStretch_Type EStretchType { u8"EStretch"N };

		static FType & Type()
		{
			return EStretchType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::EStretch>()
	{
		return TStaticType<UI::EStretch>().Type();
	}

	static TTypeRegister<UI::EStretch> EStretchTypeRegister { TStaticType<UI::EStretch>().Type() };

	class EOrientation_Type : public TEnum<UI::EOrientation>
	{
	public:
		// --- 1 Items
		struct Item___Horizontal
		{
			static inline const TEnumItem Item { u8"Horizontal"N, UI::EOrientation::Horizontal, FMetadata::None };
		};
		struct Item___Vertical
		{
			static inline const TEnumItem Item { u8"Vertical"N, UI::EOrientation::Vertical, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___Horizontal::Item,
			Item___Vertical::Item,
		};

		// --- 2 Constructor
		EOrientation_Type(FName Name = u8"EOrientation"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::EOrientation>
	{
		static inline EOrientation_Type EOrientationType { u8"EOrientation"N };

		static FType & Type()
		{
			return EOrientationType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::EOrientation>()
	{
		return TStaticType<UI::EOrientation>().Type();
	}

	static TTypeRegister<UI::EOrientation> EOrientationTypeRegister { TStaticType<UI::EOrientation>().Type() };

	class ESortOrder_Type : public TEnum<UI::ESortOrder>
	{
	public:
		// --- 1 Items
		struct Item___None
		{
			static inline const TEnumItem Item { u8"None"N, UI::ESortOrder::None, FMetadata::None };
		};
		struct Item___Ascend
		{
			static inline const TEnumItem Item { u8"Ascend"N, UI::ESortOrder::Ascend, FMetadata::None };
		};
		struct Item___Descend
		{
			static inline const TEnumItem Item { u8"Descend"N, UI::ESortOrder::Descend, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___None::Item,
			Item___Ascend::Item,
			Item___Descend::Item,
		};

		// --- 2 Constructor
		ESortOrder_Type(FName Name = u8"ESortOrder"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::ESortOrder>
	{
		static inline ESortOrder_Type ESortOrderType { u8"ESortOrder"N };

		static FType & Type()
		{
			return ESortOrderType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::ESortOrder>()
	{
		return TStaticType<UI::ESortOrder>().Type();
	}

	static TTypeRegister<UI::ESortOrder> ESortOrderTypeRegister { TStaticType<UI::ESortOrder>().Type() };

	class EElementAlignment_Type : public TEnum<UI::EElementAlignment>
	{
	public:
		// --- 1 Items
		struct Item___Near
		{
			static inline const TEnumItem Item { u8"Near"N, UI::EElementAlignment::Near, FMetadata::None };
		};
		struct Item___Center
		{
			static inline const TEnumItem Item { u8"Center"N, UI::EElementAlignment::Center, FMetadata::None };
		};
		struct Item___Far
		{
			static inline const TEnumItem Item { u8"Far"N, UI::EElementAlignment::Far, FMetadata::None };
		};
		struct Item___Stretch
		{
			static inline const TEnumItem Item { u8"Stretch"N, UI::EElementAlignment::Stretch, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___Near::Item,
			Item___Center::Item,
			Item___Far::Item,
			Item___Stretch::Item,
		};

		// --- 2 Constructor
		EElementAlignment_Type(FName Name = u8"EElementAlignment"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::EElementAlignment>
	{
		static inline EElementAlignment_Type EElementAlignmentType { u8"EElementAlignment"N };

		static FType & Type()
		{
			return EElementAlignmentType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::EElementAlignment>()
	{
		return TStaticType<UI::EElementAlignment>().Type();
	}

	static TTypeRegister<UI::EElementAlignment> EElementAlignmentTypeRegister { TStaticType<UI::EElementAlignment>().Type() };

	class EVisibility_Type : public TEnum<UI::EVisibility>
	{
	public:
		// --- 1 Items
		struct Item___Visible
		{
			static inline const TEnumItem Item { u8"Visible"N, UI::EVisibility::Visible, FMetadata::None };
		};
		struct Item___Hidden
		{
			static inline const TEnumItem Item { u8"Hidden"N, UI::EVisibility::Hidden, FMetadata::None };
		};
		struct Item___Collapsed
		{
			static inline const TEnumItem Item { u8"Collapsed"N, UI::EVisibility::Collapsed, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___Visible::Item,
			Item___Hidden::Item,
			Item___Collapsed::Item,
		};

		// --- 2 Constructor
		EVisibility_Type(FName Name = u8"EVisibility"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::EVisibility>
	{
		static inline EVisibility_Type EVisibilityType { u8"EVisibility"N };

		static FType & Type()
		{
			return EVisibilityType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::EVisibility>()
	{
		return TStaticType<UI::EVisibility>().Type();
	}

	static TTypeRegister<UI::EVisibility> EVisibilityTypeRegister { TStaticType<UI::EVisibility>().Type() };

	class ECoordinateMode_Type : public TEnum<UI::ECoordinateMode>
	{
	public:
		// --- 1 Items
		struct Item___Absolute
		{
			static inline const TEnumItem Item { u8"Absolute"N, UI::ECoordinateMode::Absolute, FMetadata::None };
		};
		struct Item___Relative
		{
			static inline const TEnumItem Item { u8"Relative"N, UI::ECoordinateMode::Relative, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___Absolute::Item,
			Item___Relative::Item,
		};

		// --- 2 Constructor
		ECoordinateMode_Type(FName Name = u8"ECoordinateMode"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::ECoordinateMode>
	{
		static inline ECoordinateMode_Type ECoordinateModeType { u8"ECoordinateMode"N };

		static FType & Type()
		{
			return ECoordinateModeType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::ECoordinateMode>()
	{
		return TStaticType<UI::ECoordinateMode>().Type();
	}

	static TTypeRegister<UI::ECoordinateMode> ECoordinateModeTypeRegister { TStaticType<UI::ECoordinateMode>().Type() };

	class FSortKey_Type : public TClass<UI::FSortKey>
	{
	public:
		using FSortKey = UI::FSortKey;

		// --- 0 Constructors
		static inline const TConstructor<FSortKey, UI::FSortKey const &> Constructor___FSortKey_const_lref { u8"!Constructor___FSortKey_const_lref"N, FMetadata::None };
		static inline const TConstructor<FSortKey, UI::FSortKey &&> Constructor___FSortKey_ref { u8"!Constructor___FSortKey_ref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___FSortKey_const_lref,
			Constructor___FSortKey_ref,
		};

		// --- 1 Methods

		// --- 2 Fields
		struct Field___Name
		{
			static inline const TField Field { u8"Name"N, &FSortKey::Name, FMetadata::None };
		};
		struct Field___Order
		{
			static inline const TField Field { u8"Order"N, &FSortKey::Order, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Name::Field,
			Field___Order::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		FSortKey_Type(FName Name = u8"FSortKey"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::FSortKey>
	{
		static inline FSortKey_Type FSortKeyType { u8"FSortKey"N };

		static FType & Type()
		{
			return FSortKeyType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::FSortKey>()
	{
		return TStaticType<UI::FSortKey>().Type();
	}

	static TTypeRegister<UI::FSortKey> FSortKeyTypeRegister { TStaticType<UI::FSortKey>().Type() };

	class FToolTip_Type : public TClass<UI::FToolTip>
	{
	public:
		using FToolTip = UI::FToolTip;

		// --- 0 Constructors

		// --- 1 Methods

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Delay
		{
			static inline const TAttachedProperty<FToolTip, uint32> Property { u8"Delay"N, FToolTip::__Delay__, FMetadata::None };
		};
		struct Property___Duration
		{
			static inline const TAttachedProperty<FToolTip, uint32> Property { u8"Duration"N, FToolTip::__Duration__, FMetadata::None };
		};
		struct Property___Placement
		{
			static inline const TAttachedProperty<FToolTip, EPopupPosition> Property { u8"Placement"N, FToolTip::__Placement__, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Delay::Property,
			Property___Duration::Property,
			Property___Placement::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IDependency>(),
		};

		// --- 5 Constructor
		FToolTip_Type(FName Name = u8"FToolTip"N) : TClass(Name, Classof<IDependency>())
		{
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<uint32> & UI::FToolTip::DelayProperty() { return FToolTip_Type::Property___Delay::Property; }

	const TProperty<uint32> & UI::FToolTip::DurationProperty() { return FToolTip_Type::Property___Duration::Property; }

	const TProperty<EPopupPosition> & UI::FToolTip::PlacementProperty() { return FToolTip_Type::Property___Placement::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::FToolTip>
	{
		static inline FToolTip_Type FToolTipType { u8"FToolTip"N };

		static FType & Type()
		{
			return FToolTipType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::FToolTip>()
	{
		return TStaticType<UI::FToolTip>().Type();
	}

	static TTypeRegister<UI::FToolTip> FToolTipTypeRegister { TStaticType<UI::FToolTip>().Type() };
}

// Generated code end.
