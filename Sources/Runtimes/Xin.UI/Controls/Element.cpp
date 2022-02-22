#include "PCH.h"
#include "Element.h"
#include "Window.h"

#define PRAGMA_DISABLE_OPTIMIZATION __pragma(optimize("",off))
#define PRAGMA_ENABLE_OPTIMIZATION __pragma(optimize("",on))

PRAGMA_DISABLE_OPTIMIZATION

namespace Xin::UI
{
	UElement::FPropertyMetadata UElement::FPropertyMetadata::None;

	UElement::UElement()
	{
	}

	UElement::UElement(FName Name) : UVisual(Name)
	{

	}

	UElement::~UElement()
	{
		//TList<UElementRef> TempLogicalChildren = LogicalChildren;
		//for (UElementRef Child : TempLogicalChildren)
		//	RemoveLogicalChild(Child);

		//TList<UElementRef> TempChildren = Children;
		//for (UElementRef Child : TempChildren)
		//	RemoveVisualChild(Child);
	}

	void UElement::OnConstruct()
	{
		UVisual::OnConstruct();
	}

	void UElement::OnInitialize()
	{
		UObject::OnInitialize();

		for (const UElementRef & Child : Children)
			Child->Initialize();

		LoadStyle();
		UpdateState();
	}

	void UElement::Finalize()
	{
		Style = nullptr;
		StateManager.ClearStateGroups();

		UObject::Finalize();
	}

	void UElement::OnFinalize()
	{
		MeasureDone = false;
		ArrangeDone = false;
		LayoutSuspended = false;
		MeasurePendding = false;
		ArrangePendding = false;

		ResetValue(AvailableSizeProperty());
		ResetValue(DesiredSizeProperty());
		ResetValue(FinalRectProperty());

		for (UElementRef & Child : Children)
			Child->Finalize();
			//OnRemoveChild(Child);

		UObject::OnFinalize();
	}

	void UElement::OnPropertyChanged(const FProperty & Property)
	{
		for (FTriggerRef & Trigger : Triggers)
		{
			if (Trigger->Property != Property)
				continue;

			Trigger->Trigger(*this);
		}
		UVisual::OnPropertyChanged(Property);
	}

	void UElement::OnChanged(const FProperty & Property)
	{
		if (FetchMetadataT<FPropertyMetadata>(Property).AffectsMeasure)
			InvalidateMeasure();
		if (FetchMetadataT<FPropertyMetadata>(Property).AffectsRender)
			InvalidateVisual();
		if (FetchMetadataT<FPropertyMetadata>(Property).AffectsArrange)
			InvalidateArrange();
		if (FetchMetadataT<FPropertyMetadata>(Property).AffectsState)
			UpdateState();

		UVisual::OnChanged(Property);
	}

	IObject * UElement::GetParentObject() const
	{
		return Parent.Lock().Get();
	}

	IGraphics & UElement::GetGraphics() const
	{
		if (UElementRef ParentElement = Parent.Lock())
			return ParentElement->GetGraphics();
		else
			return *(IGraphics *)nullptr;
	}

	IDesktop & UElement::GetDesktop() const
	{
		if (UElementRef ParentElement = Parent.Lock())
			return ParentElement->GetDesktop();
		return *(IDesktop *)nullptr;
	}

	void UElement::ShowContextMenu(UElement * Element, const FVariant & Menu, EPopupPosition PlacementMode, const TFunction<void()> & OnClosed)
	{
		if (UElementRef ParentElement = Parent.Lock())
			return ParentElement->ShowContextMenu(Element, Menu, PlacementMode, OnClosed);
	}

	void UElement::ShowToolTip(UElement * Element, const FVariant & ToolTip)
	{
		if (UElementRef ParentElement = Parent.Lock())
			return ParentElement->ShowToolTip(Element, ToolTip);
	}

	TReferPtr<UElement> UElement::FindChild(FName Name) const
	{
		if (IObject::Name == Name)
			return ConstCast(this);

		for (const UElementRef & Child : Children)
		{
			if (Child->Name == Name)
				return StaticCast<UElement>(Child);

			if (UElementRef ChildChild = StaticCast<UElement>(Child)->FindChild(Name))
				return ChildChild;
		}

		return nullptr;
	}

	TReferPtr<UElement> UElement::FindChild(const FType & Type) const
	{
		for (const UElementRef & Child : Children)
		{
			if (UElementRef ChildChild = Child->FindChild(Type))
				return ChildChild;
		}

		return nullptr;
	}

	void UElement::SetParent(UElement * Parent_)
	{
		if (Parent_ == Parent)
			return;

		if (auto ParentElement = Parent.Lock())
			ParentElement->OnRemoveChild(this);

		if (Parent_)
			Parent_->OnAddChild(this);
	}

	void UElement::SortChildrenZ()
	{
		auto SortComparer = [](const UElementRef & ElementA, const UElementRef & ElementB)
			{
				if (ElementA->ZIndex < ElementB->ZIndex)
					return true;
				else if (ElementA->ZIndex > ElementB->ZIndex)
					return false;
				else
					return ElementA->InnerZIndex < ElementB->InnerZIndex;
			};
		std::sort(Children.Begin(), Children.End(), SortComparer);

		InvalidateVisual();
	}

	void UElement::OnAddChild(UElement * Child)
	{
		AssertExpr(!MeasureDoing && !ArrangeDoing);

		if (!Child)
			return;

		if (Child->Parent)
		{
			if (auto ParentBerofe = Child->Parent.Lock())
				ParentBerofe->OnRemoveChild(Child);
			Child->Parent = nullptr;
		}

		AssertExpr(Child->Parent == nullptr);
		AssertExpr(!Children.Contains(Child));

		Children.Add(Child);
		Child->Parent = this;

		if ((Initializing || Initialized) && !Child->Initialized)
			Child->Initialize();

		Child->Shown = Shown;
		Child->Appeared = Appeared;

		InvalidateMeasure();
		Child->OnAdded(this);
	}

	void UElement::OnAdded(UElement * Child)
	{
		if (MeasureDone)
			InvalidateMeasure();
		else
		{
			if (UElementRef ParentElement = Parent.Lock())
				ParentElement->InvalidateMeasure();
		}
	}

	void UElement::OnRemoveChild(UElement * Child)
	{
		AssertExpr(!MeasureDoing && !ArrangeDoing);

		if (!Child)
			return;

		AssertExpr(Child->Parent == this);
		AssertExpr(Children.Contains(Child));

		if (Child->Initialized)
			Child->Finalize();

		Child->Appeared = false;
		Child->Shown = false;
		Child->Parent = nullptr;
		Children.Remove(Child);

		InvalidateMeasure();

		Child->OnRemoved(this);
	}

	void UElement::OnRemoved(UElement * Child)
	{
		
	}

	void UElement::OnChildMeasured(UElement * Element)
	{
		
	}

	void UElement::OnChildDesiredSizeChanged(UElement * Element, FSize DesiredSizeBefore)
	{
		//if (Size.Width.IsAuto() || Size.Height.IsAuto())
		//{
		//	MeasureDone = false;
		//	ArrangeDone = false;

		//	if (UElementRef ParentElement = Parent.Lock())
		//		ParentElement->OnChildDesiredSizeChanged(this, DesiredSizeBefore);
		//	else
		//	{
		//		if (MeasureDone)
		//			InvalidateMeasure();
		//		else if (ArrangeDone)
		//			InvalidateArrange();
		//		else { }
		//	}
		//}
		//else
		//{
		//	//if (MeasureDone)
		//	//	InvalidateMeasure();
		//	//else if (ArrangeDone)
		//	//		InvalidateArrange();
		//	//else {}
		//	if (ArrangeDone)
		//		InvalidateArrange();
		//}
	}

	void UElement::InvalidateVisual()
	{
		if (!RenderDone)
			return;

		RenderDone = false;

		if (Appeared)
		{
			if (UElementRef ParentElement = Parent.Lock())
				ParentElement->InvalidateVisual();
			//InvalidateRect({ FPoint::Zero, ActualSize });
		}
	}

	void UElement::InvalidateRect(FRect Rect)
	{
		if (UElementRef ParentElement = Parent.Lock())
			ParentElement->InvalidateRect({ FinalRect.Position + Rect.Position, Rect.Size });
	}

	UElement * UElement::HitElement(FPoint Point) const
	{
		EHitResult HitResult = HitTest(Point);
		if (HitResult == EHitResult::None)
			return nullptr;

		for (uintx ChildIndex = 0; ChildIndex < Children.Size; ++ChildIndex)
		{
			const UElementRef & Child = Children[Children.Size - ChildIndex - 1];
			if (Child->Visibility == EVisibility::Visible)
			{
				if (UElement * Element = Child->HitElement(Point - Child->FinalRect.Position))
				{
					if (!Element->Enabled)
						break;

					return Element;
				}
			}
		}

		return HitResult >= EHitResult::Inside ? ConstCast(this) : nullptr;
	}

	void UElement::DumpVisualTree(UElement & Element, uintx Depth)
	{
		FString States = EnumNameof(Element.Visibility).Resolve();
		if (Element.IsMouseOver)
			States.Append(u8",MouseOver"V);
		if (Element.Initialized)
			States.Append(u8",Initialized"V);
		if (Element.Shown)
			States.Append(u8",Shown"V);
		if (Element.Appeared)
			States.Append(u8",Appeared"V);
		if (Element.MeasureDone)
			States.Append(u8",Measured"V);
		if (Element.ArrangeDone)
			States.Append(u8",Arranged"V);
		if (Element.RenderDone)
			States.Append(u8",Rendered"V);

		static FStringV Tabs = u8"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"V;
		static FStringV Spaces = u8"                                                                 "V;
		FConsole::WriteLine(u8"{}[{}] {:p}    DesiredSize{:({:.0f},{:.0f})},FinalRect{:({:.0f},{:.0f},{:.0f},{:.0f})}, {}, {}, <{}>,    {}"V, Spaces.SubView(0, Depth * 2), 
			Element.Type().Name, 
			static_cast<void *>(&Element),
			Element.DesiredSize,
			Element.FinalRect, 
			Element.Name, 
			Element.Describe(), 
			States, 
			Element.ToString());

		for (UElementRef Child : Element.Children)
			DumpVisualTree(Child.Ref(), Depth + 1);
	}

	IResourceRef UElement::FindResource(FName Name, const FType & ResourceType)
	{
		if (auto Resource = ResourceTable.FindResource(Name, ResourceType))
			return Resource;

		if (UElementRef ParentElement = Parent.Lock())
		{
			if (IResourceRef Resource = ParentElement->FindResource(Name, ResourceType))
				return Resource;
		}
		else {}

		return nullptr;
	}

	void UElement::OnResourcesChanged()
	{
		
	}

	IResourceRef UElement::FindResource(const FType & TargetType, const FType & ResourceType)
	{
		if (auto Resource = ResourceTable.FindResource(TargetType.Name, ResourceType))
			return Resource;

		if (UElementRef ParentElement = Parent.Lock())
		{
			if (auto Resource = ParentElement->FindResource(TargetType.Name, ResourceType))
				return Resource;
		}

		return nullptr;
	}

	void UElement::AdjustConstrainedValue(dimenx AvailableValue, dimenx & ConstrainedValue, dimenx MinValue, FDimen DesignValue, dimenx MaxValue)
	{
		AssertExpr(not IsNaN(ConstrainedValue));
		if (DesignValue.IsAuto())
			ConstrainedValue = IsNaN(AvailableValue) ? Infinity<dimenx> : AvailableValue;
		else if (DesignValue.IsAbsolute())
			ConstrainedValue = DesignValue.Value;
		else if (DesignValue.IsPercent())
			ConstrainedValue = AvailableValue * DesignValue.Value;
		else { }

		if (not IsNaN(MinValue))
			ConstrainedValue = Max(ConstrainedValue, MinValue);

		if (not IsNaN(MaxValue))
			ConstrainedValue = Min(ConstrainedValue, MaxValue);
	}

	void UElement::AdjustMeasuredValue(dimenx AvailableValue, dimenx & MeasuredValue, dimenx MinValue, FDimen DesignValue, dimenx MaxValue)
	{
		if (IsNaN(MeasuredValue))
			MeasuredValue = 0dim;
		else if (DesignValue.IsAbsolute())
			MeasuredValue = DesignValue.Value;
		else if (DesignValue.IsPercent())
			MeasuredValue = AvailableValue * DesignValue.Value;
		else { }

		if (not IsNaN(MinValue))
			MeasuredValue = Max(MeasuredValue, MinValue);

		if (not IsNaN(MaxValue))
			MeasuredValue = Min(MeasuredValue, MaxValue);
	}

	void UElement::SuspendLayout()
	{
		LayoutSuspended = true;
	}

	void UElement::ResumeLayout()
	{
		LayoutSuspended = false;

		if (MeasurePendding)
		{
			MeasurePendding = false;
			InvalidateMeasure();
		}

		if (ArrangePendding)
		{
			ArrangePendding = false;
			InvalidateArrange();
		}
	}

	void UElement::InvalidateMeasure()
	{
		EnsureExpr(!MeasureDoing);
		EnsureExpr(!MeasureFinishing);

		if (LayoutSuspended)
		{
			MeasurePendding = true;
			return;
		}

		if (!MeasureDone)
			return;

		MeasureDone = false;

		if (Finalizing)
			return;

		if (UElementRef ParentElement = Parent.Lock())
			ParentElement->InvalidateMeasure();
	}

	FSize UElement::Measure(FSize RestrictSize)
	{
		AssertExpr(Initialized);
		AssertExpr(not RestrictSize.AnyNaN());

		if (MeasureDone && RestrictSize == AvailableSize)
			return DesiredSize;

		AvailableSize = RestrictSize;
		if (Visibility == EVisibility::Collapsed)
		{
			MeasureDone = true;
			DesiredSize = FSize::Zero;
			return FSize::Zero;
		}

		FDimen2 BaseSize = Size;
		FSize ConstrainedSize = AvailableSize - Margin;
		AdjustConstrainedValue(AvailableSize.Width, ConstrainedSize.Width, MinSize.Width, BaseSize.Width, MaxSize.Width);
		AdjustConstrainedValue(AvailableSize.Height, ConstrainedSize.Height, MinSize.Height, BaseSize.Height, MaxSize.Height);
		//FSize MeasuredSize = (Width.IsAuto() || Height.IsAuto()) ? Measure(ConstrainedSize) : ConstrainedSize;
		MeasureDoing = true;
		FSize MeasuredSize = OnMeasure(ConstrainedSize);
#if XIN_DEBUG
		for (const UElementRef & Child : Children)
		{
			if (Child->Visibility != EVisibility::Collapsed)
				AssertExpr(Child->MeasureDone);
		}
#endif
		MeasureDoing = false;
		AdjustMeasuredValue(AvailableSize.Width, MeasuredSize.Width, MinSize.Width, BaseSize.Width, MaxSize.Width);
		AdjustMeasuredValue(AvailableSize.Height, MeasuredSize.Height, MinSize.Height, BaseSize.Height, MaxSize.Height);

		MeasuredSize = MeasuredSize + Margin;
		if (LayoutRounding)
			MeasuredSize = Floor(MeasuredSize);

		MeasureDone = true;
		MeasureFinishing = true;
		if (MeasuredSize != DesiredSize)
		{
			AssertExpr(MeasuredSize.Width < 0xFFFFF);
			AssertExpr(MeasuredSize.Height < 0xFFFFF);

			FSize DesiredSizeBefore = DesiredSize;
			DesiredSize = MeasuredSize;
			OnDesiredSizeChanged(DesiredSizeBefore);
			if (UElementRef ParentElement = Parent.Lock())
				ParentElement->OnChildDesiredSizeChanged(this, DesiredSizeBefore);
		}

		OnMeasured();
		if (UElementRef ParentElement = Parent.Lock())
			ParentElement->OnChildMeasured(this);
		MeasureFinishing = false;

		AssertExpr(not DesiredSize.AnyNaN());
		return DesiredSize;
	}

	FSize UElement::OnMeasure(FSize ConstrainedSize)
	{
		return FSize::Zero;
	}

	void UElement::OnMeasured()
	{
		FEventArgs EventArgs;
		Measured(EventArgs);
	}

	void UElement::InvalidateArrange()
	{
		EnsureExpr(!ArrangeDoing);
		EnsureExpr(!ArrangeFinishing);

		if (LayoutSuspended)
		{
			ArrangePendding = true;
			return;
		}

		if (!ArrangeDone)
			return;

		ArrangeDone = false;

		if (Finalizing || Visibility == EVisibility::Collapsed)
			return;

		if (UElementRef ParentElement = Parent.Lock())
			ParentElement->InvalidateArrange();
	}

	FSize UElement::Arrange(FRect ArrangeRect)
	{
		AssertExpr(not ArrangeRect.AnyNaN());
		AssertExpr(ArrangeRect.Width < 0xFFFFF);
		AssertExpr(ArrangeRect.Height < 0xFFFFF);
		AssertExpr(Initialized && /*MeasureDone*/!DesiredSize.AnyNaN());

		if (Visibility == EVisibility::Collapsed)
			return FSize::Zero;

		if (!ArrangeDone || this->ArrangeRect != ArrangeRect)
		{
			this->ArrangeRect = ArrangeRect;
			FinalRect = { ArrangeRect.Position + Margin.LeftTop, Max(ArrangeRect.Size - Margin.Size, FSize::Zero) };

			if (UElementRef ParentElement = Parent.Lock())
				VisibleRect = FRect(FPoint::Zero, FinalRect.Size).Intersect(ParentElement->VisibleRect.Offset(-FinalRect.Position));
			else
				VisibleRect = { FPoint::Zero, ArrangeRect.Size };

			Appeared = !VisibleRect.Empty();
		}
		else
		{
			FRect VisibleRectModified;
			if (UElementRef ParentElement = Parent.Lock())
				VisibleRectModified = FRect(FPoint::Zero, ArrangeRect.Size).Intersect(ParentElement->VisibleRect.Offset(-ArrangeRect.Position));
			else
				VisibleRectModified = { FPoint::Zero, ArrangeRect.Size };

			if (VisibleRectModified != VisibleRect)
			{
				VisibleRect = VisibleRectModified;
				Appeared = !VisibleRect.Empty();
			}
		}

		{
			ArrangeDoing = true;
			OnArrange();
#if XIN_DEBUG
			for (const UElementRef & Child : Children)
			{
				if (Child->Visibility != EVisibility::Collapsed)
					AssertExpr(Child->ArrangeDone);
			}
#endif
			ArrangeDoing = false;

			ArrangeDone = true;
			ArrangeFinishing = true;
			OnArranged();
			ArrangeFinishing = false;
		}

		return ArrangeRect.Size;
	}

	void UElement::OnArrange()
	{
	}

	void UElement::OnArranged()
	{
		InvalidateVisual();

		FEventArgs EventArgs;
		Arranged(EventArgs);
	}

	void UElement::ArrangeChild(UElement & Child, FRect AviliableRect)
	{
		FRect Rect;
		if (Child.Width.IsAuto() && Child.HorizontalAlignment == EElementAlignment::Stretch)
		{
			Rect.X = AviliableRect.Left;
			Rect.Width = AviliableRect.Width;
		}
		else
		{
			//Rect.Width = Min(AviliableRect.Width, Child.DesiredSize.Width);
			switch (Child.HorizontalAlignment)
			{
			case EElementAlignment::Near:
				Rect.X = AviliableRect.Left;
				Rect.Width = Child.DesiredSize.Width;
				break;
			case EElementAlignment::Stretch:
				Rect.X = AviliableRect.Left;
				Rect.Width = AviliableRect.Width;
				break;
			case EElementAlignment::Far:
				Rect.X = AviliableRect.Right - Child.DesiredSize.Width;
				Rect.Width = Child.DesiredSize.Width;
				break;
			default:
			case EElementAlignment::Center:
				Rect.X = AviliableRect.X + (AviliableRect.Width - Child.DesiredSize.Width) * 0.5dim;
				Rect.Width = Child.DesiredSize.Width;
				break;
			}
		}

		if (Child.Height.IsAuto() && Child.VerticalAlignment == EElementAlignment::Stretch)
		{
			Rect.Y = AviliableRect.Top;
			Rect.Height = AviliableRect.Height;
		}
		else
		{
			//Rect.Height = Min(AviliableRect.Height, Child.DesiredSize.Height);
			switch (Child.VerticalAlignment)
			{
			case EElementAlignment::Near:
				Rect.Y = AviliableRect.Top;
				Rect.Height = Child.DesiredSize.Height;
				break;
			case EElementAlignment::Stretch:
				Rect.Y = AviliableRect.Top;
				Rect.Height = AviliableRect.Height;
				break;
			case EElementAlignment::Far:
				Rect.Y = AviliableRect.Bottom - Child.DesiredSize.Height;
				Rect.Height = Child.DesiredSize.Height;
				break;
			default:
			case EElementAlignment::Center:
				Rect.Y = AviliableRect.Y + (AviliableRect.Height - Child.DesiredSize.Height) * 0.5dim;
				Rect.Height = Child.DesiredSize.Height;
				break;
			}
		}

		Child.Arrange(Rect);
	}

	void UElement::Paint(IPainter & Painter)
	{
		AssertExpr(!FinalSize.AnyNaN());
		if (VisibleRect.Empty() || FinalSize.Width <= 0 || FinalSize.Height <= 0 || Visibility != EVisibility::Visible)
			return;

		if (GroundingFill || GroundingStroke)
			Painter.DrawRect({ { }, FinalSize }, GroundingFill ? GroundingFill.Ref() : IBrush::None, { GroundingStroke ? GroundingStroke.Ref() : IBrush::None, (float32)GroundingStrokeThickness });

		OnPaint(Painter);
		{
			for (const UElementRef & Child : Children)
			{
				if (Child->Visibility == EVisibility::Visible && !Child->FinalSize.Empty()
					&& VisibleRect.IntersectsWith(Child->FinalRect))
				{
					Painter.PushViewport(RectF(Child->FinalRect));
					Child->Paint(Painter);
					Painter.PopViewport();
				}
			}
		}
		RenderDone = true;
	}

	void UElement::OnPaint(IPainter & Painter)
	{
	}

	void UElement::LoadStyle()
	{
		if (!Style)
			Style = FindResourceT<IControlStyle>(Type());

		if (Style)
			Style->LoadStyle(*this);
	}

	void UElement::SetState(FVisualStateName VisualStateName)
	{
		for (FVisualStateGroupRef & VisualStateGroup : StateManager.StateGroups)
		{
			if (VisualStateGroup->Name == VisualStateName.GroupName)
				VisualStateGroup->SetCurrentState(VisualStateName.StateName);
		}
	}

	void UElement::UpdateState()
	{
		if (IsMouseOver)
			SetState(u8"MouseOver"N);
		else
			SetState(u8"Normal"N);
	}

	void UElement::PreMouseEnter(FMouseEventArgs & Args)
	{
		//FConsole::WriteLine(u8"{} PreMouseEnter {} {:p} {}"V, FDateTime::Now(), Type().Name, (void *)this, Describe());

		AssertExpr(!IsMouseOver);
		IsMouseOver = true;
		if (!Args.Handled)
			OnMouseEnter(Args);
		if (!Args.Handled)
			MouseEnter(Args);
	}

	void UElement::PreMouseLeave(FMouseEventArgs & Args)
	{
		//FConsole::WriteLine(u8"{} PreMouseLeave {} {:p} {}"V, FDateTime::Now(), Type().Name, (void *)this, Describe());

		AssertExpr(IsMouseOver);

		if (MouseCaptured)
			ReleaseMouseCapture();

		IsMouseLeftButtonDown = false;
		IsMouseOver = false;
		if (!Args.Handled)
			OnMouseLeave(Args);
		if (!Args.Handled)
			MouseLeave(Args);
	}

	void UElement::PreMouseMove(FMouseEventArgs & Args)
	{
		if (!Args.Handled)
			OnMouseMove(Args);

		if (!Args.Handled)
			MouseMove.Excute(Args);

		if (!Args.Handled)
		{
			if (UElementRef ParentElement = Parent.Lock())
			{
				Args.Position = LocalToParent(Args.Position);
				ParentElement->PreMouseMove(Args);
			}
		}
	}

	void UElement::PreMouseWhell(FMouseWhellEventArgs & Args)
	{
		if (!Args.Handled)
			OnMouseWhell(Args);
		if (!Args.Handled)
			MouseWhell.Excute(Args);

		if (!Args.Handled)
		{
			if (UElementRef ParentElement = Parent.Lock())
			{
				Args.Position = LocalToParent(Args.Position);
				ParentElement->PreMouseWhell(Args);
			}
		}
	}

	void UElement::PreKeyDown(FKeyboardEventArgs & Args)
	{
		if (!Args.Handled)
			OnKeyDown(Args);

		if (!Args.Handled)
			KeyDown.Excute(Args);
	}

	void UElement::OnKeyDown(FKeyboardEventArgs & Args)
	{
	}

	void UElement::PreKeyUp(FKeyboardEventArgs & Args)
	{
		if (!Args.Handled)
			OnKeyUp(Args);

		if (!Args.Handled)
			KeyUp.Excute(Args);
	}

	void UElement::OnKeyUp(FKeyboardEventArgs & Args)
	{
	}

	void UElement::AcquireMouseCapture()
	{
		if (Enabled)
		{
			Window.SetCapturedElement(this);
			MouseCaptured = true;
		}
	}

	void UElement::ReleaseMouseCapture()
	{
		if (MouseCaptured)
		{
			MouseCaptured = false;
			Window.SetCapturedElement(nullptr);
		}
	}

	void UElement::OnVisibilityChanged(EVisibility VisibilityBefore)
	{
		UElementRef ParentElement = Parent.Lock();
		if (Visibility == EVisibility::Visible)
		{
			if (ParentElement)
			{
				Shown = ParentElement->Shown;
				InvalidateMeasure();
			}
			else
			{
				Shown = false;
			}
		}
		else
		{
			if (ParentElement)
				InvalidateMeasure();
			Shown = true;
		}


		//{
		//	if (Parent->Size.Width.IsAuto() || Parent->Size.Height.IsAuto())
		//		Parent->InvalidateMeasure();
		//	else
		//		Parent->InvalidateArrange();
		//}
		//if (Parent)
		//	Parent->OnChildDesiredSizeChanged(this, DesiredSize);
	}

	void UElement::OnShown()
	{
	}

	void UElement::OnHidden()
	{
	}

	void UElement::OnShownChanged(bool ShownBefore)
	{
		if (Shown)
		{
			OnShown();
			for (const UElementRef & Element : Children)
				Element->Shown = Element->Visibility == EVisibility::Visible;
		}
		else
		{
			ResetValues(EValueLayer::Animated);
			for (FTriggerRef & Trigger : Triggers)
				Trigger->Interrupt(*this);

			for (UElementRef & Element : Children)
				Element->Shown = false;
			OnHidden();
		}
	}

	void UElement::OnAppeared()
	{
		//if (!MeasureDone)
		//{
		//	TPointerPtr<FLayoutManager> LayoutManager = GetLayoutManager();
		//	LayoutManager->InvalidateMeasure(this);
		//}

		//if (!ArrangeDone)
		//{
		//	if (UElementRef ParentElement = Parent.Lock())
		//		ParentElement->InvalidateArrange();
		//}

		if (!RenderDone)
		{
			if (UElementRef ParentElement = Parent.Lock())
				ParentElement->InvalidateVisual();
			//InvalidateRect({ FPoint::Zero, ActualSize });
		}
	}

	void UElement::OnDisappeared()
	{
		
	}

	void UElement::OnAppearedChanged(bool AppearedBefore)
	{
		if (Appeared)
		{
			OnAppeared();
			for (UElementRef & Element : Children)
				Element->Appeared = true;
		}
		else
		{
			for (UElementRef & Element : Children)
				Element->Appeared = false;
			OnDisappeared();
		}
	}
	ECursor UElement::QueryCursor(FPoint Point) const
	{
		if (Cursor != ECursor::None)
			return Cursor;

		if (UElementRef ParentElement = Parent.Lock())
			return ParentElement->QueryCursor(LocalToParent(Point));

		return ECursor::None;
	}

	void UElement::PreMouseDown(FMouseButtonEventArgs & Args)
	{
		if (!Args.Handled)
			OnMouseDown(Args);

		if (!Args.Handled)
		{
			switch (Args.Button)
			{
			case EMouseButton::Left:
				IsMouseLeftButtonDown = true;
				OnMouseLeftButtonDown(Args);
				break;
			case EMouseButton::Middle:
				OnMouseMiddleButtonDown(Args);
				break;
			case EMouseButton::Right:
				OnMouseRightButtonDown(Args);
				break;
			default:
				break;
			}
		}

		if (!Args.Handled)
			MouseDown(Args);

		if (!Args.Handled)
		{
			if (UElementRef ParentElement = Parent.Lock())
			{
				Args.Position = LocalToParent(Args.Position);
				ParentElement->PreMouseDown(Args);
			}
		}
	}

	void UElement::PreMouseUp(FMouseButtonEventArgs & Args)
	{
		if (!Args.Handled)
			OnMouseUp(Args);

		if (!Args.Handled)
		{
			switch (Args.Button)
			{
			case EMouseButton::Left:
				IsMouseLeftButtonDown = false;
				OnMouseLeftButtonUp(Args);
				break;
			case EMouseButton::Middle:
				OnMouseMiddleButtonUp(Args);
				break;
			case EMouseButton::Right:
				OnMouseRightButtonUp(Args);
				break;
			default:
				break;
			}
		}

		if (!Args.Handled)
			MouseUp(Args);

		if (!Args.Handled)
		{
			if (UElementRef ParentElement = Parent.Lock())
			{
				Args.Position = LocalToParent(Args.Position);
				ParentElement->PreMouseUp(Args);
			}
		}

		if (!Args.Handled && Args.Button == EMouseButton::Right && ContextMenu)
		{
			ShowContextMenu(this, ContextMenu, EPopupPosition::Mouse, nullptr);
		}
	}

	void UElement::OnDesiredSizeChanged(FSize DesiredSizeBefore)
	{
		if (UElementRef ParentElement = Parent.Lock())
			ParentElement->InvalidateArrange();
	}

	void UElement::OnCommand(ICommand & Command, FRoutedEventArgs & RoutedEventArgs)
	{
		auto Iter = FindIf(CommandBindings, [&](const auto & Pair) noexcept { return Command.Name == Pair.First->Name; });
		if (Iter != CommandBindings.End())
			Iter->Second(RoutedEventArgs);

		if (!RoutedEventArgs.Handled)
		{
			if (UElementRef ParentElement = Parent.Lock())
				ParentElement->OnCommand(Command, RoutedEventArgs);
		}
	}
}

PRAGMA_ENABLE_OPTIMIZATION
