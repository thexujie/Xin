#pragma once

#include "Xin.Core/Xin.Core.h"

#if _Xin_UI_Module_
#if XIN_LIB_DYNAMIC
#define UI_API XIN_EXPORT
#define UI_VAR XIN_EXPORT
#else
#define UI_API
#define UI_VAR
#endif
#else
#define UI_API XIN_IMPORT
#define UI_VAR XIN_IMPORT
#endif

namespace Xin::UI
{
	inline FSize operator + (const FSize & Size, const FThickness & Margin)
	{
		return { Size.Width + Margin.Width, Size.Height + Margin.Height };
	}

	inline FSize & operator += (FSize & Size, const FThickness & Margin)
	{
		Size.Width += Margin.Width;
		Size.Height += Margin.Height;
		return Size;
	}

	constexpr dimenx operator""dim(long double Value)
	{
		return dimenx(Value);
	}

	constexpr dimenx operator""dim(uint64 Value) noexcept
	{
		return dimenx(Value);
	}

	inline FSize operator - (const FSize & Size, const FThickness & Margin)
	{
		return { Size.Width - Margin.Width, Size.Height - Margin.Height };
	}

	inline FSize & operator -= (FSize & Size, const FThickness & Margin)
	{
		Size.Width -= Margin.Width;
		Size.Height -= Margin.Height;
		return Size;
	}

	using IUObject = IObject;

	class UControl;
	using UControlRef = TReferPtr<UControl>;
	using UControlObj = TEntryPtr<UControl>;

	class UElement;
	using UElementRef = TReferPtr<UElement>;
	using UElementObj = TEntryPtr<UElement>;

	enum class [[Meta]] EStretch
	{
		None = 0,
		Fill,
		Uniform,
		UniformFill,
	};

	enum class [[Meta]] EOrientation
	{
		Horizontal = 0,
		Vertical,
	};

	enum class [[Meta]] ESortOrder
	{
		None = 0,
		Ascend,
		Descend,
	};

	struct [[Meta]] FSortKey
	{
		FName Name;
		ESortOrder Order = ESortOrder::Ascend;

		bool operator==(const FSortKey &) const noexcept = default;
		auto operator<=>(const FSortKey &) const noexcept = default;
		explicit operator bool() const noexcept { return !!Name; }
	};

	enum class ERoutingStrategy
	{
		Bubble,
		Tunnel,
		Direct,
	};

	enum class [[Meta]] EElementAlignment
	{
		Near = 0,
		Center,
		Far,
		Stretch,
	};

	enum class [[Meta]] EVisibility
	{
		Visible = 0,
		Hidden,
		Collapsed
	};

	enum class [[Meta]] ECoordinateMode
	{
		Absolute,
		Relative,
	};


	template<typename T>
	class TIItemAddable
	{
	public:
		virtual ~TIItemAddable() = default;

		virtual void OnAddItem(T & Item) {}
		virtual void OnRemoveItem(T & Item) {}
		virtual void OnItemsChanged(TList<T> & List) {}
	};

	template<typename T>
	class TItemCollection : public TList<T>
	{
	public:
		TItemCollection(TIItemAddable<T> & Addable) : Addable(Addable)
		{

		}

		void ClearItems()
		{
			if (!TList<T>::Size)
				return;

			TList<T> ItemsRemoved { Move(*this) };
			for (T & ItemRemoved : ItemsRemoved)
				Addable.OnRemoveItem(ItemRemoved);

			Addable.OnItemsChanged(*this);
		}

		T & AddItem(const T & Item)
		{
			T & AddedItem = TList<T>::Add(Item);
			Addable.OnAddItem(AddedItem);
			Addable.OnItemsChanged(*this);
			return AddedItem;
		}

		void AddItems(TView<T> Items)
		{
			if (!Items.Size)
				return;

			TList<T>::AddRange(Items);
			Addable.OnItemsChanged(*this);
			//Addable.OnAddItem(Item);
		}

		void RemoveItem(const T & Item)
		{
			if (TList<T>::Remove(Item))
			{
				Addable.OnRemoveItem(ConstCast(Item));
				Addable.OnItemsChanged(*this);
			}
		}

		TItemCollection & operator =(TView<T> Items)
		{
			bool ItemsChanged = (TList<T>::Size > 0) || (Items.Size > 0);

			for (T & Item : *this)
				Addable.OnRemoveItem(Item);

			TList<T>::operator=(Items);

			for (T & Item : *this)
				Addable.OnAddItem(Item);

			if (ItemsChanged)
				Addable.OnItemsChanged(*this);

			return *this;
		}

		template<ConvertibleToC<T> Ty>
		TItemCollection & operator =(TView<Ty> Items)
		{
			bool ItemsChanged = (TList<T>::Size > 0) || (Items.Size > 0);

			for (T & Item : *this)
				Addable.OnRemoveItem(Item);

			TList<T>::operator=(Items);

			for (T & Item : *this)
				Addable.OnAddItem(Item);

			if (ItemsChanged)
				Addable.OnItemsChanged(*this);

			return *this;
		}

		template<ConvertibleToC<T> Ty>
		TItemCollection & operator =(const TList<Ty> & Items)
		{
			return operator=(TView<Ty> { Items });
		}


	public:
		TIItemAddable<T> & Addable;
	};

	class UI_API FRoutedEventArgs : public FEventArgs
	{
	public:
		FRoutedEventArgs(IObject & Sender) : Sender(Sender) {}

	public:
		IObject & Sender;
		ERoutingStrategy Strategy = ERoutingStrategy::Bubble;
	};
	class UI_API FInputEventArgs : public FRoutedEventArgs
	{
	public:
		FInputEventArgs(IObject & Sender) : FRoutedEventArgs(Sender) {}
	};

	class UI_API [[Meta]] FToolTip : public IDependency
	{
	public:
		FToolTip() = delete;

		AttachedPropertyRW(uint32, Delay);
		AttachedPropertyRW(uint32, Duration);
		AttachedPropertyRW(EPopupPosition, Placement) = EPopupPosition::Bottom;
	};
}
