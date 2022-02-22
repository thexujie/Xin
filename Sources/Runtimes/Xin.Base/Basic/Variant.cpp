#include "PCH.h"
#include "Variant.h"

#include "Formatter.h"
#include "../Meta/Type.h"

namespace Xin
{
	FVariant FVariant::None;

	const FType & FVariant::Type() const
	{
		switch(Kind)
		{
		case EVariantKind::None:			return FType::None;

#define ITERATE_VARIANT_KIND_LOOP_BODY(VariantType, VariantKind) case EVariantKind::VariantKind: return Typeof<VariantType>();
			ITERATE_VARIANT_KIND
#undef ITERATE_VARIANT_KIND_LOOP_BODY

		case EVariantKind::Name:			return Typeof<FName>();
		case EVariantKind::Text:			return Typeof<FText>();
		case EVariantKind::String:			return Typeof<FString>();

		case EVariantKind::ReferPointer:	return ReferPointerValue ? ReferPointerValue->Type() : FType::None;
		case EVariantKind::ObjectPointer:
		{
			if (!ObjectEntryValue)
				return FType::None;
			if (IObject * Object = ObjectEntryValue->Lock())
				return Object->Type();
			else
				return FType::None;
		}
		case EVariantKind::Adapter:
			return AdapterValue ? AdapterValue->Type() : FType::None;
		default:
			return FType::None;
		}
	}

	FStringV FVariant::Format(FStringV Formal) const
	{
		switch (Kind)
		{
		case EVariantKind::None : return u8"'none'"V;

		case EVariantKind::Integer: return Xin::Format(Formal, IntegerValue);
		case EVariantKind::Float32: return Xin::Format(Formal, Float32Value);
		case EVariantKind::Float64: return Xin::Format(Formal, Float64Value);

		case EVariantKind::Vec2SB: return Xin::Format(Formal, Vec2SBValue);
		case EVariantKind::Vec3SB: return Xin::Format(Formal, Vec3SBValue);
		case EVariantKind::Vec4SB: return Xin::Format(Formal, Vec4SBValue);
		case EVariantKind::Vec2UB: return Xin::Format(Formal, Vec2UBValue);
		case EVariantKind::Vec3UB: return Xin::Format(Formal, Vec3UBValue);
		case EVariantKind::Vec4UB: return Xin::Format(Formal, Vec4UBValue);

		case EVariantKind::Vec2SS: return Xin::Format(Formal, Vec2SSValue);
		case EVariantKind::Vec3SS: return Xin::Format(Formal, Vec3SSValue);
		case EVariantKind::Vec4SS: return Xin::Format(Formal, Vec4SSValue);
		case EVariantKind::Vec2US: return Xin::Format(Formal, Vec2USValue);
		case EVariantKind::Vec3US: return Xin::Format(Formal, Vec3USValue);
		case EVariantKind::Vec4US: return Xin::Format(Formal, Vec4USValue);

		case EVariantKind::Vec2I: return Xin::Format(Formal, Vec2IValue);
		case EVariantKind::Vec3I: return Xin::Format(Formal, Vec3IValue);
		case EVariantKind::Vec4I: return Xin::Format(Formal, Vec4IValue);
		case EVariantKind::Vec2U: return Xin::Format(Formal, Vec2UValue);
		case EVariantKind::Vec3U: return Xin::Format(Formal, Vec3UValue);
		case EVariantKind::Vec4U: return Xin::Format(Formal, Vec4UValue);

		case EVariantKind::Vec2SL: return Xin::Format(Formal, Vec2SLValue);
		case EVariantKind::Vec3SL: return Xin::Format(Formal, Vec3SLValue);
		case EVariantKind::Vec4SL: return Xin::Format(Formal, Vec4SLValue);
		case EVariantKind::Vec2UL: return Xin::Format(Formal, Vec2ULValue);
		case EVariantKind::Vec3UL: return Xin::Format(Formal, Vec3ULValue);
		case EVariantKind::Vec4UL: return Xin::Format(Formal, Vec4ULValue);

		case EVariantKind::Name: return NameValue.Resolve();
		case EVariantKind::Text: return TextValue.Resolve();
		case EVariantKind::String: return StringValue;

		case EVariantKind::ReferPointer: return Xin::Format(u8"'refer'{:p}"V, static_cast<void *>(ReferPointerValue));
		case EVariantKind::ObjectPointer: return Xin::Format(u8"'object'{:p}"V, static_cast<void *>(ObjectEntryValue));

		case EVariantKind::Adapter: return Xin::Format(u8"'adapter'{:p}"V, static_cast<void *>(AdapterValue));

		default:
			return Xin::Format(u8"'{} Variant'"V, Type().Name);
		}
	}

	bool FVariant::IsA(const FType & Type_) const
	{
		return Type().IsA(Type_);
	}
}
