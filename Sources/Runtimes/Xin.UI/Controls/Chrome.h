#pragma once

#include "ContentControl.h"
#include "ButtonControl.h"
#include "Border.h"

namespace Xin::UI
{
	class UChrome;
	class UChromePanel;

	enum class EChromeCommand
	{
		None = 0,
		Close,
	};

	class UI_API UChromeButton: public UButtonControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UChromeButton(UChrome & Chrome) : Chrome(Chrome) {}
		UChromeButton(UChrome & Chrome, FName Name) : UButtonControl(Name), Chrome(Chrome) {}
		~UChromeButton() = default;

	public:
		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnClick(FRoutedEventArgs & Args) override;

	public:
		UChrome & Chrome;

		DependencyPropertyRW(EChromeCommand, Command);

		DependencyPropertyRW(FString, PathCommand);

		DependencyPropertyRW(IBrushRef, PathFill);

		DependencyPropertyRW(IBrushRef, PathStroke);

		DependencyPropertyRW(float32, PathStrokeThickness);
	};
	using UChromeButtonRef = TReferPtr<UChromeButton>;


	class UI_API UChromeBorder: public UBorder
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UChromeBorder(UChrome & Chrome) : Chrome(Chrome) {}
		UChromeBorder(UChrome & Chrome, FName Name) : UBorder(Name), Chrome(Chrome) {}
		~UChromeBorder() = default;

		void OnConstruct() override;
		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		EHitResult HitTest(FPoint Point) const override;
		ECursor QueryCursor(FPoint Point) const override;

	public:
		UChrome & Chrome;

	public:
		[[Meta(AffectsMeasure, FriendlyName("Border Thickness"), AffectsRender)]]
		DependencyPropertyRW(FThickness, ChromeThickness) = 2;

	};
	using UChromeBorderRef = TReferPtr<UChromeBorder>;


	class UI_API UChromeTitle : public UControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UChromeTitle(UChrome & Chrome) : Chrome(Chrome) {}
		UChromeTitle(UChrome & Chrome, FName Name) : UControl(Name), Chrome(Chrome) {}
		~UChromeTitle() = default;

		void OnConstruct() override;
		void OnMouseDown(FMouseButtonEventArgs & Args) override;

	public:
		UChrome & Chrome;

		DependencyPropertyRW(FText, Title);
	};

	using UChromeTitleRef = TReferPtr<UChromeTitle>;


	class UI_API UChrome : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UChrome(UChromePanel & ChromePanel) : ChromePanel(ChromePanel) {}
		UChrome(UChromePanel & ChromePanel, FName Name) : UContentControl(Name), ChromePanel(ChromePanel) {}

		~UChrome() = default;

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		void OnMouseDown(FMouseButtonEventArgs & Args) override;

	public:
		UChromePanel & ChromePanel;
		intx ChromeDepth = 0;

	public:
		DependencyPropertyRW(FText, Title) = FText::None;

		void OnLocationChanged(FPoint LocationBefore);
		DependencyPropertyRW(FPoint, Location) = FPoint::Zero;

	public:
		FDelegate Closed;
	};

	using UChromeRef = TReferPtr<UChrome>;
	using UChromeObj = TEntryPtr<UChrome>;
}
