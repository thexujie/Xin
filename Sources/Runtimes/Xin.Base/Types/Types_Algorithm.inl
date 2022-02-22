#pragma once

namespace Xin
{
	template<typename T, uintx Length>
	constexpr T * Begin(T(&Array)[Length]) noexcept { return Array; }

	template<typename T, uintx Length>
	constexpr T * End(T(&Array)[Length]) noexcept { return Array + Length; }

	template<typename IterT, typename T>
	IterT Find(IterT First, const IterT Last, const T & Value)
	{
		while (First != Last)
		{
			if (*First == Value)
				return First;
			++First;
		}
		return Last;
	}

	template<Iteraterable ContainerT, typename T>
	auto Find(const ContainerT & Container, const T & Value)
	{
		for (auto Iter = Container.Begin(); Iter != Container.End(); ++Iter)
		{
			if (*Iter == Value)
				return Iter;
		}
		return Container.End();
	}

	template<typename IterT, typename CompareT>
	forceinline IterT FindIf(IterT First, const IterT Last, CompareT && Compare)
	{
		return std::find_if(First, Last, Forward<CompareT>(Compare));
	}

	template<IterableC ContainerT, typename CompareT>
	forceinline auto FindIf(const ContainerT & Container, CompareT && Compare)
	{
		return FindIf(Container.Begin(), Container.End(), Forward<CompareT>(Compare));
	}

	template<typename IterT, typename CompareT>
	forceinline IterT FindIfNot(IterT First, const IterT Last, CompareT Compare)
	{
		return std::find_if_not(First, Last, Forward<CompareT>(Compare));
	}

	template<typename IterT, typename T>
	bool Contains(IterT First, const IterT Last, const T & Value)
	{
		while (First != Last)
		{
			if (*First == Value)
				return true;
			++First;
		}
		return false;
	}

	template<Iteraterable ContainerT, typename T>
	bool Contains(const ContainerT & Container, const T & Value)
	{
		for (auto Iter = Container.Begin(); Iter != Container.End(); ++Iter)
		{
			if (*Iter == Value)
				return true;
		}
		return false;
	}

	template<Iteraterable T, typename ConditionT>
	uintx FindIndex(const T & Container, ConditionT && Condition)
	{
		for (auto Iter = Container.Begin(); Iter != Container.End(); ++Iter)
		{
			if (Condition(*Iter))
				return Iter - Container.Begin();
		}
		return NullIndex;
	}

	template<typename IterT, typename Ty>
	Ty Accumulate(IterT First, IterT Last, Ty Value)
	{
		return std::accumulate(First, Last, Value);
	}
}
