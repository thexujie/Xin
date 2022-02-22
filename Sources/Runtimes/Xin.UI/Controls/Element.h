#pragma once

#include "Xin.UI.Types.h"
#include "Visual.h"
#include "Core/Command.h"
#include "Core/ControlStyle.h"
#include "Core/Resource.h"
#include "Core/LayoutManager.h"
#include "Core/ControlState.h"

namespace Xin::UI
{
	class UWindow;
	using UWindowRef = TReferPtr<UWindow>;
	using UWindowObj = TEntryPtr<UWindow>;

	class UView;
	using UViewRef = TReferPtr<UView>;
	using UViewObj = TEntryPtr<UView>;

	class UMenu;
	using UMenuRef = TReferPtr<UMenu>;
	using UMenuObj = TEntryPtr<UMenu>;

	using FRoutedEvent = TEvent<void(const FRoutedEventArgs &)>;
	using FRoutedEventHandler = FRoutedEvent::FunctionT;

	class FSizeChangedEventArgs : public FRoutedEventArgs
	{
	public:
		FSizeChangedEventArgs(IObject & Sender, FSize Size) : FRoutedEventArgs(Sender), Size(Size) {}

	public:
		FSize Size;
	};
	using FSizeChangedEvent = TEvent<void(const FSizeChangedEventArgs &)>;

	class UI_API FMouseEventArgs : public FInputEventArgs
	{
	public:
		FMouseEventArgs(IObject & Sender) : FInputEventArgs(Sender) {}

		FPoint Position = FPoint::Zero;
		UElement * Element = nullptr;
	};

	using FMouseEventDelegate = TDelegate<void(const FMouseEventArgs &)>;

	class UI_API FMouseButtonEventArgs : public FMouseEventArgs
	{
	public:
		FMouseButtonEventArgs(IObject & Sender, EMouseButton Button) : FMouseEventArgs(Sender), Button(Button) {}

		EMouseButton Button;
	};

	using FMouseButtonEventDelegate = TDelegate<void(const FMouseButtonEventArgs &)>;

	class UI_API FMouseWhellEventArgs : public FMouseEventArgs
	{
	public:
		FMouseWhellEventArgs(IObject & Sender, Vec2I Whell) : FMouseEventArgs(Sender), Whell(Whell) {}

		Vec2I Whell;
	};

	using FMouseWhellEventArgsDelegate = TDelegate<void(const FMouseWhellEventArgs &)>;

	class UI_API FKeyboardEventArgs : public FRoutedEventArgs
	{
	public:
		FKeyboardEventArgs(IObject & Sender, EKeyCode Key) : FRoutedEventArgs(Sender), Key(Key) { }

	public:
		EKeyCode Key;
	};

	using FKeyboardEventDelegate = TDelegate<void(const FKeyboardEventArgs &)>;


	class UI_API UElement : public UVisual
	{
	public:
		class UI_API FPropertyMetadata : public FMetadata
		{
		public:
			bool AffectsMeasure = false;
			bool AffectsArrange = false;
			bool AffectsRender = false;
			bool AffectsState = false;

		public:
			static FPropertyMetadata None;
		};

		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UElement();
		UElement(FName Name);
		~UElement();

	public:
		void OnConstruct() override;

		void OnInitialize() override;
		void Finalize() override;
		void OnFinalize() override;

		void OnPropertyChanged(const FProperty & Property) override;
		void OnChanged(const FProperty & Property) override;
		IObject * GetParentObject() const override;

	public:
		virtual TPointerPtr<FLayoutManager> GetLayoutManager()
		{
			if (UElementRef ParentElement = Parent.Lock())
				return ParentElement->GetLayoutManager();
			else
				return nullptr;
		}

		virtual UWindow * GetWindow() const
		{
			if (UElementRef ParentElement = Parent.Lock())
				return ParentElement->GetWindow();
			else
				return nullptr;
		}

		virtual IGraphics & GetGraphics() const;
		PropertyR(IGraphics &, GetGraphics) Graphics;

		UWindow & GetWindowUnsafe() const { return *GetWindow(); }
		PropertyR(UWindow &, GetWindowUnsafe) Window;

		virtual UView * GetView() const
		{
			if (UElementRef ParentElement = Parent.Lock())
				return ParentElement->GetView();
			else
				return nullptr;
		}

		UView & GetViewUnsafe() const { return *GetView(); }
		PropertyR(UView &, GetViewUnsafe) View;

		bool IsPlaced() const { return GetWindow() != nullptr; }
		PropertyR(bool, IsPlaced) Placed;

		virtual IDesktop & GetDesktop() const;
		PropertyR(IDesktop &, GetDesktop) Desktop;

		virtual void ShowContextMenu(UElement * Element, const FVariant & Menu, EPopupPosition PlacementMode, const TFunction<void()> & OnClosed = nullptr);
		virtual void ShowToolTip(UElement * Element, const FVariant & ToolTip);

	public: // --- Appearance
		//void SetSize(const FDimen2 & SizeNew) { Size = SizeNew; }

		//FDimen GetWidth() const { return Size.Width; }
		//void SetWidth(FDimen NewWidth) { SetSize({ NewWidth, Size.Height }); }
		//FDimen GetHeight() const { return Size.Height; }
		//void SetHeight(FDimen NewHeight) { SetSize({ Size.Width, NewHeight }); }

		dimenx GetMinWidth() const { return MinSize.Width; }
		void SetMinWidth(dimenx NewWidth) { MinSize.Width = NewWidth; }
		dimenx GetMinHeight() const { return MinSize.Height; }
		void SetMinHeight(dimenx NewHeight) { MinSize.Height = NewHeight; }

		dimenx GetMaxWidth() const { return MaxSize.Width; }
		void SetMaxWidth(dimenx NewWidth) { MaxSize.Width = NewWidth; }
		dimenx GetMaxHeight() const { return MaxSize.Height; }
		void SetMaxHeight(dimenx NewHeight) { MaxSize.Height = NewHeight; }

	public:
		UElementRef FindChild(FName Name) const;
		template<typename T>
		TReferPtr<T> FindChildT(FName Name) const
		{
			UElementRef Child = FindChild(Name);
			AssertExpr(!Child || Child->Type().IsA<T>());
			return StaticCast<T>(Child);
		}
		TReferPtr<UElement> FindChild(const FType & Type) const;

	public:
		bool IsAncestorOf(const UElement * Child) const
		{
			if (!Child)
				return false;
			const UElement * Element = Child->Parent.Lock();
			while (Element)
			{
				if (Element == this)
					return true;
				Element = Child->Parent.Lock();
			}
			return false;
		}

		void SetParent(UElement * Parent_);
		void SortChildrenZ();
		void OnAddChild(UElement * Child);
		void OnRemoveChild(UElement * Child);
		virtual void OnAdded(UElement * Child);
		virtual void OnRemoved(UElement * Child);
		//TView<UElement *> GetChildren() const;

		virtual void OnChildMeasured(UElement * Element);
		virtual void OnChildDesiredSizeChanged(UElement * Element, FSize DesiredSizeBefore);

		TList<UElementRef> Children;

	public:
		FPoint LocalToParent(FPoint Point) const
		{
			return Point + FinalRect.Position;
		}

		FPoint ParentToLocal(FPoint Point) const
		{
			return Point - FinalRect.Position;
		}

		FPoint LocalToWindow(FPoint Point) const
		{
			if (UElementRef ParentElement = Parent.Lock())
				return ParentElement->LocalToWindow(LocalToParent(Point));
			return LocalToParent(Point);
		}

		FPoint WindowToLocal(FPoint Point) const
		{
			if (UElementRef ParentElement = Parent.Lock())
				return ParentToLocal(ParentElement->WindowToLocal(Point));
			return ParentToLocal(Point);
		}

		virtual FPoint LocalToGlobal(FPoint Point) const
		{
			if (UElementRef ParentElement = Parent.Lock())
				return ParentElement->LocalToGlobal(LocalToParent(Point));
			return LocalToParent(Point);
		}

		virtual FPoint GlobalToLocal(FPoint Point) const
		{
			if (UElementRef ParentElement = Parent.Lock())
				return ParentToLocal(ParentElement->GlobalToLocal(Point));
			return ParentToLocal(Point);
		}

	public:
		UElementObj Parent = nullptr;

	public:
		virtual void InvalidateVisual();
		virtual void InvalidateRect(FRect Rect);
		virtual UElement * HitElement(FPoint Point) const;
		static void DumpVisualTree(UElement & Element, uintx Depth = 0);

	public:
		virtual void OnResourcesChanged();
		FResourceTable ResourceTable;

		virtual IResourceRef FindResource(FName Name, const FType & ResourceType);
		virtual IResourceRef FindResource(const FType & TargetType, const FType & ResourceType);

		template<DerivedOfC<IResource> T>
		TReferPtr<T> FindResourceT(FName Name)
		{
			return StaticCast<T>(FindResource(Name, Typeof<T>()));
		}

		template<DerivedOfC<IResource> T>
		TReferPtr<T> FindResourceT(const FType & TargetType)
		{
			return StaticCast<T>(FindResource(TargetType, Typeof<T>()));
		}

	public: // --- Layout
		void AdjustConstrainedValue(dimenx AvailableValue, dimenx & ConstrainedValue, dimenx MinValue, FDimen DesignValue, dimenx MaxValue);
		void AdjustMeasuredValue(dimenx AvailableValue, dimenx & MeasuredValue, dimenx MinValue, FDimen DesignValue, dimenx MaxValue);

		void SuspendLayout();
		void ResumeLayout();

		virtual void InvalidateMeasure();
		virtual FSize Measure(FSize RestrictSize);
		virtual FSize OnMeasure(FSize ConstrainedSize);
		virtual void OnMeasured();
		TEvent<void(FEventArgs &)> Measured;

		virtual void InvalidateArrange();
		virtual FSize Arrange(FRect ArrangeRect);
		virtual void OnArrange();
		virtual void OnArranged();
		TEvent<void(FEventArgs &)> Arranged;

	protected:
		FSize MeasureChild(UElement * Child, FSize ConstrainedSize)
		{
			if (Child)
				return Child->Measure(ConstrainedSize);
			return FSize::Zero;
		}

		void ArrangeChild(UElement & Child, FRect AviliableRect);

	public:
		virtual void Paint(IPainter & Painter);
		virtual void OnPaint(IPainter & Painter);

	public:
		bool RenderDone = false;
		bool MeasureDoing = false;
		bool MeasureDone = false;
		bool MeasureFinishing = false;
		bool ArrangeDoing = false;
		bool ArrangeDone = false;
		bool ArrangeFinishing = false;

		bool LayoutSuspended = false;
		bool MeasurePendding = false;
		bool ArrangePendding = false;

	public:
		void LoadStyle();

	public:
		void SetState(FVisualStateName VisualStateName);
		virtual void UpdateState();


	public: // --- Mouse interaction
		TEvent<void(FMouseEventArgs &)> MouseEnter;
		TEvent<void(FMouseEventArgs &)> MouseLeave;
		TEvent<void(FMouseEventArgs &)> MouseMove;
		TEvent<void(FMouseButtonEventArgs &)> MouseDown;
		TEvent<void(FMouseButtonEventArgs &)> MouseUp;
		TEvent<void(FMouseButtonEventArgs &)> MouseClick;
		TEvent<void(FMouseWhellEventArgs  &)> MouseWhell;
		TEvent<void(FKeyboardEventArgs &)> KeyDown;
		TEvent<void(FKeyboardEventArgs &)> KeyUp;

		virtual void PreMouseEnter(FMouseEventArgs & Args);
		virtual void PreMouseLeave(FMouseEventArgs & Args);
		virtual void PreMouseMove(FMouseEventArgs & Args);

		virtual void OnMouseEnter(FMouseEventArgs & Args) { }
		virtual void OnMouseLeave(FMouseEventArgs & Args) { }
		virtual void OnMouseHover(FMouseEventArgs & Args) { }
		virtual void OnMouseMove(FMouseEventArgs & Args) { }

		virtual void PreMouseDown(FMouseButtonEventArgs & Args);
		virtual void PreMouseUp(FMouseButtonEventArgs & Args);

		virtual void OnMouseDown(FMouseButtonEventArgs & Args) { }
		virtual void OnMouseUp(FMouseButtonEventArgs & Args) { }
		virtual void OnMouseLeftButtonDown(FMouseButtonEventArgs & Args) { }
		virtual void OnMouseLeftButtonUp(FMouseButtonEventArgs & Args) { }
		virtual void OnMouseMiddleButtonDown(FMouseButtonEventArgs & Args) { }
		virtual void OnMouseMiddleButtonUp(FMouseButtonEventArgs & Args) { }
		virtual void OnMouseRightButtonDown(FMouseButtonEventArgs & Args) { }
		virtual void OnMouseRightButtonUp(FMouseButtonEventArgs & Args) { }

		virtual void PreMouseWhell(FMouseWhellEventArgs & Args);
		virtual void OnMouseWhell(FMouseWhellEventArgs & Args) { }

		virtual void PreKeyDown(FKeyboardEventArgs & Args);
		virtual void OnKeyDown(FKeyboardEventArgs & Args);
		virtual void PreKeyUp(FKeyboardEventArgs & Args);
		virtual void OnKeyUp(FKeyboardEventArgs & Args);

		virtual void AcquireMouseCapture();
		virtual void ReleaseMouseCapture();

	public:
		ProxyPropertyRW(intx, ZIndex) = 0;

		ProxyPropertyRW(intx, InnerZIndex) = 0;

		virtual void OnSizeChanged(const FDimen2 & SizeBefore) { }
		[[Meta(AffectsMeasure)]]
		DependencyPropertyRW(FDimen2, Size) = FDimen2::NaN;

		void __SetWidth(FDimen WidthAfter) { Size = FDimen2{WidthAfter, Size.Height}; }
		FDimen __GetWidth() const { return Size.Width; }
		ProxyPropertyRW4(FDimen, __GetWidth, __SetWidth, Width);

		void __SetHeight(FDimen HeightAfter) { Size = FDimen2 { Size.Width, HeightAfter }; }
		FDimen __GetHeight() const { return Size.Height; }
		ProxyPropertyRW4(FDimen, __GetHeight, __SetHeight, Height);

		FSize MinSize = FSize::Zero;
		FSize MaxSize = FSize::NaN;

		DependencyPropertyRW(FThickness, Margin) = 0;

		[[Meta(Hierarchical)]]
		DependencyPropertyRW(bool, LayoutRounding) = false;

	public: // Flags
		bool ClipToBounds = false;
		bool Focusable = false;

	public:
		virtual void OnVisibilityChanged(EVisibility VisibilityBefore);
		[[Meta(AffectsMeasure)]]
		DependencyPropertyRW(EVisibility, Visibility) = EVisibility::Visible;

	public:
		virtual void OnShown();
		virtual void OnHidden();
		virtual void OnShownChanged(bool ShownBefore);
		ProxyPropertyRW(bool, Shown) = false;

	public:
		virtual void OnAppeared();
		virtual void OnDisappeared();
		virtual void OnAppearedChanged(bool AppearedBefore);
		ProxyPropertyRW(bool, Appeared) = false;

	public:
		DependencyPropertyRW(bool, Enabled) = true;

		virtual ECursor QueryCursor(FPoint Point) const;
		DependencyPropertyRW(ECursor, Cursor) = ECursor::Arrow;

		DependencyPropertyRW(FVariant, ContextMenu) = noval;
		DependencyPropertyRW(FVariant, ToolTip) = noval;

	public:
		virtual EHitResult HitTest(FPoint Point) const
		{
			if (FinalSize.Contains(Point))
				return HitInside;
			return EHitResult::None;
		}
		DependencyPropertyRW(EHitResult, HitInside) = EHitResult::Inside;

	public:
		DependencyPropertyRW(bool, MouseCaptured) = false;
		virtual void OnMouseCapturedChanged(bool ValueBefore) {}

	public:
		ProxyPropertyRW(FSize, AvailableSize) = FSize::NaN;

		virtual void OnDesiredSizeChanged(FSize DesiredSizeBefore);
		ProxyPropertyRW(FSize, DesiredSize) = FSize::NaN;

		// Rect with margins in parent coordinate
		FRect ArrangeRect = FRect::NaN;

		// Rect without margins in parent coordinate
		ProxyPropertyRW(FRect, FinalRect) = FRect::NaN;

		// View rect in Layout space.
		FRect VisibleRect = FRect::NaN;

		// Rect without margins in local coordinate
		FSize GetFinalSize() const { return FinalRect.Size; }
		PropertyR(FSize, GetFinalSize) FinalSize;

	public:
		PropertyRW(dimenx, GetMinWidth, SetMinWidth) MinWidth;
		PropertyRW(dimenx, GetMinHeight, SetMinHeight) MinHeight;
		PropertyRW(dimenx, GetMaxWidth, SetMaxWidth) MaxWidth;
		PropertyRW(dimenx, GetMaxHeight, SetMaxHeight) MaxHeight;

	public:
		[[Meta(AffectsState)]]
		ProxyPropertyRW(bool, IsMouseOver) = false;

		[[Meta(AffectsState)]]
		ProxyPropertyRW(bool, IsMouseLeftButtonDown) = false;

	public:
		PropertyMetadata(Inherits, Default(EElementAlignment::Fill));
		DependencyPropertyRW(EElementAlignment, HorizontalAlignment) = EElementAlignment::Stretch;

		PropertyMetadata(Inherits);
		DependencyPropertyRW(EElementAlignment, VerticalAlignment) = EElementAlignment::Stretch;

	public:
		[[Meta(Hierarchical)]]
		DependencyPropertyRW(float32, ScaleRatio) = noval;

		[[Meta(Hierarchical)]]
		DependencyPropertyRW(FString, FontFamily) = noval;

		[[Meta(Hierarchical)]]
		DependencyPropertyRW(float32, FontSize) = noval;

		[[Meta(Hierarchical)]]
		DependencyPropertyRW(EFontWeight, FontWeight) = noval;

		[[Meta(Hierarchical)]]
		DependencyPropertyRW(EFontStyle, FontStyle) = noval;

	public:
		virtual void OnCommand(ICommand & Command, FRoutedEventArgs & RoutedEventArgs);
		TList<TPair<ICommandRef, TFunction<void(FRoutedEventArgs &)>>> CommandBindings;

	public:
		DependencyPropertyRW(IBrushRef, GroundingFill) = noval;
		DependencyPropertyRW(IBrushRef, GroundingStroke) = noval;
		DependencyPropertyRW(dimenx, GroundingStrokeThickness) = 1.0f;

	public:
		FControlStateManager StateManager;
		IControlStyleRef Style;

	public:
		static inline FSize DefaultSize { 32, 32 };
	};
}

extern template UI_API Xin::FType & Xin::Typeof<Xin::UI::EElementAlignment>();
