#pragma once

#include "Xin.Rendering.Types.h"

namespace Xin::Rendering
{
	constexpr FStringV CompiledShaderExtention(EShaderStage ShaderStage)
	{
		switch(ShaderStage)
		{
		case EShaderStage::Vertex: return u8".xvs"V;
		case EShaderStage::Pixel: return u8".xps"V;
		case EShaderStage::Geometry: return u8".xgs"V;
		case EShaderStage::Compute: return u8".xcs"V;
		case EShaderStage::Hull: return u8".xhs"V;
		case EShaderStage::Domain: return u8".xds"V;
		default: return u8".xin"V;
		}
	}

	constexpr FStringV ShaderFriendlyName(EShaderStage ShaderStage)
	{
		switch (ShaderStage)
		{
		case EShaderStage::Vertex: return u8"VS"V;
		case EShaderStage::Pixel: return u8"PS"V;
		case EShaderStage::Geometry: return u8"GS"V;
		case EShaderStage::Compute: return u8"CS"V;
		case EShaderStage::Hull: return u8"HS"V;
		case EShaderStage::Domain: return u8"DS"V;
		default: return u8"NA"V;
		}
	}

	struct RENDERING_API FShaderPermutation
	{
		struct FDefine
		{
			FString Name;
			FString Value;
			forceinline uint64 Hash() const { return HashCombine(Name, Value); }
		};

		FShaderPermutation() = default;
		FShaderPermutation(FStringV ShaderEntry, TView<FDefine> Defines = { }) : ShaderEntry(ShaderEntry), Defines(Defines) { }

		FString ShaderEntry;
		TList<FDefine> Defines;

		forceinline friend uint64 Hash(const FShaderPermutation & ShaderPermutation) { return HashCombine(ShaderPermutation.ShaderEntry, ShaderPermutation.Defines); }
	};
	using FIndexedShaderPermutation = TIndexedStructure<uint64, FShaderPermutation>;

	class FShaderCompilerEnvironment
	{
	public:
		void SetDefine(FName Name, FStringV Value)
		{
			Definitions.Add({ Name.Resolve(), Value });
		}

		struct FDefine
		{
			FString Name;
			FString Value;
			forceinline uint64 Hash() const { return HashCombine(Name, Value); }
		};

		TMap<FString, FString> IncludeFileMapping;
		TList<FDefine> Definitions;
	};

	struct IShaderPermutation
	{
		constexpr IShaderPermutation(FName Name) : Name(Name) {}
		constexpr virtual ~IShaderPermutation() = default;

		virtual void AffectEnvironment(FShaderCompilerEnvironment & Environment) const {}

		FName Name;
	};

	template<TFixedString PermutationName, typename PermutationValueT>
	struct TShaderPermutation : IShaderPermutation
	{
		using ValueT = PermutationValueT;

		constexpr TShaderPermutation() : IShaderPermutation((FStringV)PermutationName) {}
	};


	template<TFixedString PermutationName>
	struct TShaderPermutationBool : TShaderPermutation<PermutationName, bool>
	{
		static constexpr uintx PermutationCount = 2;

		template<bool PermutationValue>
		static constexpr uintx PermutationIndex = PermutationValue ? 1 : 0;

		static uintx ComputePermutationIndex(bool PermutationValue)
		{
			return PermutationValue ? 1 : 0;
		}

		static bool ValidatePermutationValue(bool PermutationValue)
		{
			return true;
		}

		void AffectEnvironment(FShaderCompilerEnvironment & Environment) const override
		{
			Environment.SetDefine(IShaderPermutation::Name, Value ? u8"1"V : u8"0"V);
		}

		bool Value = false;
	};

	template<TFixedString PermutationName>
	struct TShaderPermutationInt : TShaderPermutation<PermutationName, int32>
	{
		void AffectEnvironment(FShaderCompilerEnvironment & Environment) const override
		{
			Environment.SetDefine(IShaderPermutation::Name, Format(u8"{}"V, Value));
		}

		int32 Value = 0;
	};

	template<TFixedString PermutationName, int32 ...Values>
	struct TShaderPermutationIntSequence : TShaderPermutationInt<PermutationName>
	{
		static constexpr uintx PermutationCount = sizeof...(Values);

		template<int32 PermutationValue>
		static constexpr uintx PermutationIndex = PackValueIndex<int32, PermutationValue, Values...>;

		static uintx ComputePermutationIndex(int32 PermutationValue)
		{
			intx Index = PackFindValue<int32, 0, Values...>(PermutationValue);
			AssertReturn(Index != NullIndex, 0);
			return Index;
		}

		static bool ValidatePermutationValue(int32 PermutationValue)
		{
			AssertExpr(((PermutationValue == Values) || ...));
			return ((PermutationValue == Values) || ...);
		}
	};

	/** [ValueMin, ValueMax] */
	template<TFixedString PermutationName, int32 ValueMin, int32 ValueMax>
	struct TShaderPermutationIntRange : TShaderPermutationInt<PermutationName>
	{
		static_assert(ValueMin <= ValueMax);

		template<int32 PermutationValue>
		static constexpr uintx PermutationIndex = Clamp(PermutationValue, ValueMin, ValueMax) - ValueMin;

		static constexpr uintx PermutationCount = uintx(ValueMax - ValueMin + 1);

		static uintx ComputePermutationIndex(int32 PermutationValue)
		{
			return Clamp(PermutationValue, ValueMin, ValueMax) - ValueMin;
		}

		static bool ValidatePermutationValue(int32 PermutationValue)
		{
			AssertExpr(ValueMin <= PermutationValue && PermutationValue <= ValueMax);
			return ValueMin <= PermutationValue && PermutationValue <= ValueMax;
		}
	};

	template<TFixedString PermutationName, EnumC EnumT>
	struct TShaderPermutationEnum : TShaderPermutation<PermutationName, EnumT>
	{
		static constexpr uintx PermutationCount = uintx(UnderlyingTypeT<EnumT>(EnumT::Count));

		template<EnumT PermutationValue>
		static constexpr uintx PermutationIndex = uintx(UnderlyingTypeT<EnumT>(PermutationValue));

		static uintx ComputePermutationIndex(EnumT PermutationValue)
		{
			return uintx(UnderlyingTypeT<EnumT>(PermutationValue));
		}

		static bool ValidatePermutationValue(EnumT PermutationValue)
		{
			UnderlyingTypeT<EnumT> UnderlyingValue = UnderlyingTypeT<EnumT>(PermutationValue);
			AssertExpr(0 <= UnderlyingValue && UnderlyingValue < UnderlyingTypeT<EnumT>(EnumT::Count));
			return 0 <= UnderlyingValue && UnderlyingValue < UnderlyingTypeT<EnumT>(EnumT::Count);
		}

		void AffectEnvironment(FShaderCompilerEnvironment & Environment) const override
		{
			Environment.SetDefine(IShaderPermutation::Name, Format(u8"{}"V, UnderlyingTypeT<EnumT>(Value)));
		}

		EnumT Value = EnumT(0);
	};

	template<uintx Index, typename ...PermutationTs>
	uintx AccumulateShaderPermutationMultiple()
	{
		if constexpr (Index >= sizeof...(PermutationTs))
			return 1;
		else
			return PackIndexingT<Index, PermutationTs...>::PermutationCount * AccumulateShaderPermutationMultiple<Index + 1, PermutationTs...>();
	}

	template<uintx Index, typename ...PermutationTs>
	uintx SingleShaderPermutationIndex(const TTuple<PermutationTs...> & Permutations)
	{
		uintx PermutationIndex = PackIndexingT<Index, PermutationTs...>::ComputePermutationIndex(Get<Index>(Permutations).Value);
		return PermutationIndex * AccumulateShaderPermutationMultiple<Index + 1, PermutationTs...>();
	}

	template<uintx ...Indices, typename ...PermutationTs>
	uintx AccumulateShaderPermutationIndex(IndexSequence<Indices...>, const TTuple<PermutationTs...> & Permutations)
	{
		return (SingleShaderPermutationIndex<Indices, PermutationTs...>(Permutations) + ...);

		// same as SingleShaderPermutationIndex
		//return ((PackIndexingT<Indices, PermutationTs...>::ComputePermutationIndex(Get<Indices>(Permutations).Value) * AccumulateShaderPermutationMultiple<Indices + 1, PermutationTs...>()) + ...);
	}

	template<typename ...PermutationTs>
	uintx ComputeShaderPermutationIndex(const TTuple<PermutationTs...> & Permutations)
	{
		return AccumulateShaderPermutationIndex(MakeIndexSequence<sizeof...(PermutationTs)>(), Permutations);
	}

	class IShaderVariance
	{
	public:
		virtual ~IShaderVariance() = default;

		virtual uintx ComputeShaderVarianceIndex() const = 0;
		virtual void AffectEnvironment(FShaderCompilerEnvironment & Environment) const = 0;
	};

	class FEmptyShaderVariance : public IShaderVariance
	{
	public:
		static constexpr uintx VarianceCount = 1;

		uintx ComputeShaderVarianceIndex() const override { return 0; }
		void AffectEnvironment(FShaderCompilerEnvironment & Environment) const override {}
	};

	template<typename ...PermutationTs>
	class TShaderVariance : public IShaderVariance
	{
	public:
		static constexpr uintx VarianceCount = (PermutationTs::PermutationCount * ...);

		template<typename PermutationT> requires (PackTypeIndex<PermutationT, PermutationTs...> != -1)
		void SetValue(const typename PermutationT::ValueT & Value)
		{
			PermutationT::ValidatePermutationValue(Value);

			Get<PackTypeIndex<PermutationT, PermutationTs...>>(Permutations).Value = Value;
		}

		template<typename PermutationT>
		typename PermutationT::ValueT & GetValue()
		{
			return Get<PackTypeIndex<PermutationT, PermutationTs...>>(Permutations).Value;
		}

		template<typename PermutationT>
		const typename PermutationT::ValueT & GetValue() const
		{
			return Get<PackTypeIndex<PermutationT, PermutationTs...>>(Permutations).Value;
		}

		template<typename PermutationT>
		void ApplyPermutation(FShaderCompilerEnvironment & Environment) const
		{
			(Get<PackTypeIndex<PermutationT, PermutationTs...>>(Permutations)).AffectEnvironment(Environment);
		}

	public:
		void AffectEnvironment(FShaderCompilerEnvironment & Environment) const override
		{
			(ApplyPermutation<PermutationTs>(Environment), ...);
		}

		uintx ComputeShaderVarianceIndex() const override
		{
			return ComputeShaderPermutationIndex(Permutations);
		}

		TTuple<PermutationTs...> Permutations;
	};

	template<>
	class TShaderVariance<> : public IShaderVariance
	{
	public:
		static constexpr uintx VarianceCount = 1;

		uintx ComputeShaderVarianceIndex() const override { return 0; }
		void AffectEnvironment(FShaderCompilerEnvironment & Environment) const override {}
	};

	using FIndexedShaderPermutation = TIndexedStructure<uint64, FShaderPermutation>;


	class FBaseShaderInstance: public IShaderInstance
	{
	public:
		FBaseShaderInstance(FStringV FilePath, EShaderStage ShaderStage, FStringV EntryName)
			: ShaderStage(ShaderStage), FilePath(FilePath), EntryName(EntryName)
		{
		}

		EShaderStage GetShaderStage() const override{ return ShaderStage; }
		FStringV GetFilePath() const override { return FilePath; }
		FStringV GetEntryName() const override{ return EntryName; }

	public:
		EShaderStage ShaderStage;
		FString FilePath;
		FString EntryName;
	};

	class RENDERING_API IShaderSource : public IRenderingObject
	{
	public:
		using FShaderVariance = FEmptyShaderVariance;

		IShaderSource(EShaderStage ShaderStage, FStringV FilePath, FStringV EntryName = FStringV::None);

		virtual uintx GetVarianceCount() const { return 0; }

	public:
		EShaderStage ShaderStage;
		FString FilePath;
		FString EntryName;
	};


#define DEFINE_SIMPLE_SHADER(ClassName, FilePath, ShaderStage, EntryName) \
	class ClassName : public IShaderSource\
	{ \
	public: \
		using FShaderVariance = FEmptyShaderVariance; \
 \
		ClassName() : IShaderSource(ShaderStage, FilePath, EntryName) \
		{ \
		} \
\
		static ClassName & Instance()\
		{ \
			static ClassName ClassInstance; \
			return ClassInstance; \
		} \
\
		uintx GetVarianceCount() const override { return FShaderVariance::VarianceCount; }\
	};

#define DEFINE_SIMPLE_DEFAULT_SHADER(ClassName, FilePath, ShaderStage) \
	class ClassName : public IShaderSource\
	{ \
	public: \
		using FShaderVariance = FEmptyShaderVariance; \
 \
		ClassName() : IShaderSource(ShaderStage, FilePath) \
		{ \
		} \
\
		static ClassName & Instance()\
		{ \
			static ClassName ClassInstance; \
			return ClassInstance; \
		} \
\
		uintx GetVarianceCount() const override { return FShaderVariance::VarianceCount; }\
	};

#define DEFINE_SHADER_INSTANCE(ClassName) \
		static ClassName & Instance()\
		{ \
			static ClassName ClassInstance; \
			return ClassInstance; \
		} \
	\
		uintx GetVarianceCount() const override { return FShaderVariance::VarianceCount; }
}
