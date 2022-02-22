// B8E268509B5FAE536891ADD020B69CCE 2025-03-07 17:13:43
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Platform/Desktop.h"

namespace Xin
{
	class ECursor_Type : public TEnum<ECursor>
	{
	public:
		// --- 1 Items
		struct Item___None
		{
			static inline const TEnumItem Item { u8"None"N, ECursor::None, FMetadata::None };
		};
		struct Item___Arrow
		{
			static inline const TEnumItem Item { u8"Arrow"N, ECursor::Arrow, FMetadata::None };
		};
		struct Item___Cross
		{
			static inline const TEnumItem Item { u8"Cross"N, ECursor::Cross, FMetadata::None };
		};
		struct Item___Hand
		{
			static inline const TEnumItem Item { u8"Hand"N, ECursor::Hand, FMetadata::None };
		};
		struct Item___Help
		{
			static inline const TEnumItem Item { u8"Help"N, ECursor::Help, FMetadata::None };
		};
		struct Item___Hiden
		{
			static inline const TEnumItem Item { u8"Hiden"N, ECursor::Hiden, FMetadata::None };
		};
		struct Item___IBeam
		{
			static inline const TEnumItem Item { u8"IBeam"N, ECursor::IBeam, FMetadata::None };
		};
		struct Item___Pen
		{
			static inline const TEnumItem Item { u8"Pen"N, ECursor::Pen, FMetadata::None };
		};
		struct Item___SizeAll
		{
			static inline const TEnumItem Item { u8"SizeAll"N, ECursor::SizeAll, FMetadata::None };
		};
		struct Item___SizeNESW
		{
			static inline const TEnumItem Item { u8"SizeNESW"N, ECursor::SizeNESW, FMetadata::None };
		};
		struct Item___SizeVertical
		{
			static inline const TEnumItem Item { u8"SizeVertical"N, ECursor::SizeVertical, FMetadata::None };
		};
		struct Item___SizeNWSE
		{
			static inline const TEnumItem Item { u8"SizeNWSE"N, ECursor::SizeNWSE, FMetadata::None };
		};
		struct Item___SizeHorizontal
		{
			static inline const TEnumItem Item { u8"SizeHorizontal"N, ECursor::SizeHorizontal, FMetadata::None };
		};
		struct Item___UpArrow
		{
			static inline const TEnumItem Item { u8"UpArrow"N, ECursor::UpArrow, FMetadata::None };
		};
		struct Item___Wait
		{
			static inline const TEnumItem Item { u8"Wait"N, ECursor::Wait, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___None::Item,
			Item___Arrow::Item,
			Item___Cross::Item,
			Item___Hand::Item,
			Item___Help::Item,
			Item___Hiden::Item,
			Item___IBeam::Item,
			Item___Pen::Item,
			Item___SizeAll::Item,
			Item___SizeNESW::Item,
			Item___SizeVertical::Item,
			Item___SizeNWSE::Item,
			Item___SizeHorizontal::Item,
			Item___UpArrow::Item,
			Item___Wait::Item,
		};

		// --- 2 Constructor
		ECursor_Type(FName Name = u8"ECursor"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<ECursor>
	{
		static inline ECursor_Type ECursorType { u8"ECursor"N };

		static FType & Type()
		{
			return ECursorType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<ECursor>()
	{
		return TStaticType<ECursor>().Type();
	}

	static TTypeRegister<ECursor> ECursorTypeRegister { TStaticType<ECursor>().Type() };

	class EHitResult_Type : public TEnum<EHitResult>
	{
	public:
		// --- 1 Items
		struct Item___None
		{
			static inline const TEnumItem Item { u8"None"N, EHitResult::None, FMetadata::None };
		};
		struct Item___Ignore
		{
			static inline const TEnumItem Item { u8"Ignore"N, EHitResult::Ignore, FMetadata::None };
		};
		struct Item___Inside
		{
			static inline const TEnumItem Item { u8"Inside"N, EHitResult::Inside, FMetadata::None };
		};
		struct Item___Minimun
		{
			static inline const TEnumItem Item { u8"Minimun"N, EHitResult::Minimun, FMetadata::None };
		};
		struct Item___Maximum
		{
			static inline const TEnumItem Item { u8"Maximum"N, EHitResult::Maximum, FMetadata::None };
		};
		struct Item___Close
		{
			static inline const TEnumItem Item { u8"Close"N, EHitResult::Close, FMetadata::None };
		};
		struct Item___Title
		{
			static inline const TEnumItem Item { u8"Title"N, EHitResult::Title, FMetadata::None };
		};
		struct Item___Caption
		{
			static inline const TEnumItem Item { u8"Caption"N, EHitResult::Caption, FMetadata::None };
		};
		struct Item___SystemMenu
		{
			static inline const TEnumItem Item { u8"SystemMenu"N, EHitResult::SystemMenu, FMetadata::None };
		};
		struct Item___Left
		{
			static inline const TEnumItem Item { u8"Left"N, EHitResult::Left, FMetadata::None };
		};
		struct Item___Right
		{
			static inline const TEnumItem Item { u8"Right"N, EHitResult::Right, FMetadata::None };
		};
		struct Item___Top
		{
			static inline const TEnumItem Item { u8"Top"N, EHitResult::Top, FMetadata::None };
		};
		struct Item___Bottom
		{
			static inline const TEnumItem Item { u8"Bottom"N, EHitResult::Bottom, FMetadata::None };
		};
		struct Item___TopLeft
		{
			static inline const TEnumItem Item { u8"TopLeft"N, EHitResult::TopLeft, FMetadata::None };
		};
		struct Item___TopRight
		{
			static inline const TEnumItem Item { u8"TopRight"N, EHitResult::TopRight, FMetadata::None };
		};
		struct Item___BottomLeft
		{
			static inline const TEnumItem Item { u8"BottomLeft"N, EHitResult::BottomLeft, FMetadata::None };
		};
		struct Item___BottomRight
		{
			static inline const TEnumItem Item { u8"BottomRight"N, EHitResult::BottomRight, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___None::Item,
			Item___Ignore::Item,
			Item___Inside::Item,
			Item___Minimun::Item,
			Item___Maximum::Item,
			Item___Close::Item,
			Item___Title::Item,
			Item___Caption::Item,
			Item___SystemMenu::Item,
			Item___Left::Item,
			Item___Right::Item,
			Item___Top::Item,
			Item___Bottom::Item,
			Item___TopLeft::Item,
			Item___TopRight::Item,
			Item___BottomLeft::Item,
			Item___BottomRight::Item,
		};

		// --- 2 Constructor
		EHitResult_Type(FName Name = u8"EHitResult"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<EHitResult>
	{
		static inline EHitResult_Type EHitResultType { u8"EHitResult"N };

		static FType & Type()
		{
			return EHitResultType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<EHitResult>()
	{
		return TStaticType<EHitResult>().Type();
	}

	static TTypeRegister<EHitResult> EHitResultTypeRegister { TStaticType<EHitResult>().Type() };

	class EPlacementMode_Type : public TEnum<EPopupPosition>
	{
	public:
		// --- 1 Items
		struct Item___Absolute
		{
			static inline const TEnumItem Item { u8"Absolute"N, EPopupPosition::Absolute, FMetadata::None };
		};
		struct Item___Relative
		{
			static inline const TEnumItem Item { u8"Relative"N, EPopupPosition::Relative, FMetadata::None };
		};
		struct Item___Left
		{
			static inline const TEnumItem Item { u8"Left"N, EPopupPosition::Left, FMetadata::None };
		};
		struct Item___Top
		{
			static inline const TEnumItem Item { u8"Top"N, EPopupPosition::Top, FMetadata::None };
		};
		struct Item___Right
		{
			static inline const TEnumItem Item { u8"Right"N, EPopupPosition::Right, FMetadata::None };
		};
		struct Item___Bottom
		{
			static inline const TEnumItem Item { u8"Bottom"N, EPopupPosition::Bottom, FMetadata::None };
		};
		struct Item___Center
		{
			static inline const TEnumItem Item { u8"Center"N, EPopupPosition::Center, FMetadata::None };
		};
		struct Item___Mouse
		{
			static inline const TEnumItem Item { u8"Mouse"N, EPopupPosition::Mouse, FMetadata::None };
		};
		struct Item___Cursor
		{
			static inline const TEnumItem Item { u8"Cursor"N, EPopupPosition::Cursor, FMetadata::None };
		};
		struct Item___Custom
		{
			static inline const TEnumItem Item { u8"Custom"N, EPopupPosition::Custom, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___Absolute::Item,
			Item___Relative::Item,
			Item___Left::Item,
			Item___Top::Item,
			Item___Right::Item,
			Item___Bottom::Item,
			Item___Center::Item,
			Item___Mouse::Item,
			Item___Cursor::Item,
			Item___Custom::Item,
		};

		// --- 2 Constructor
		EPlacementMode_Type(FName Name = u8"EPlacementMode"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<EPopupPosition>
	{
		static inline EPlacementMode_Type EPlacementModeType { u8"EPlacementMode"N };

		static FType & Type()
		{
			return EPlacementModeType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<EPopupPosition>()
	{
		return TStaticType<EPopupPosition>().Type();
	}

	static TTypeRegister<EPopupPosition> EPlacementModeTypeRegister { TStaticType<EPopupPosition>().Type() };
}

// Generated code end.
