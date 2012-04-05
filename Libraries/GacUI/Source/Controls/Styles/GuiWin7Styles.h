/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
	GuiControl::IStyleController
		Win7EmptyStyle
		Win7WindowStyle
		Win7MenuStyle
		Win7MenuBarStyle
		Win7MenuSplitterStyle
		Win7GroupBoxStyle
	GuiTab::IStyleController
		Win7TabStyle
	GuiSelectableButton::IStyleController
		Win7ButtonStyle(vertical|horizontal)
		Win7ToolstripButtonStyle
		Win7CheckBoxStyle(check|radio)
		Win7SelectableItemStyle
		Win7TabPageHeaderStyle
		Win7ListViewColumnDropDownStyle
		Win7ListViewColumnHeaderStyle
		Win7TreeViewExpandingButtonStyle
	GuiComboBoxBase::IStyleController
		Win7DropDownComboBoxStyle
	GuiMenuButton::IStyleController
		Win7MenuBarButtonStyle
		Win7MenuItemButtonStyle
	GuiScroll::IStyleController
		Win7ScrollStyle
		Win7TrackStyle
	GuiScrollView::IStyleProvider
		Win7ScrollViewProvider
		Win7MultilineTextBoxProvider
	GuiSinglelineTextBox::IStyleProvider
		Win7SinglelineTextBoxProvider
	list::TextItemStyleProvider::ITextItemStyleProvider
		Win7TextListProvider
		Win7CheckTextListProvider
		Win7RadioTextListProvider
	GuiListView::IStyleProvider
		Win7ListViewProvider
	GuiTreeView::IStyleProvider
		Win7TreeViewProvider
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES
#define VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES

#include "GuiCommonStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Button Configuration
***********************************************************************/
			
			struct Win7ButtonColors
			{
				Color										borderColor;
				Color										backgroundColor;
				Color										g1;
				Color										g2;
				Color										g3;
				Color										g4;
				Color										textColor;
				Color										bulletLight;
				Color										bulletDark;

				bool operator==(const Win7ButtonColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						backgroundColor == colors.backgroundColor &&
						g1 == colors.g1 &&
						g2 == colors.g2 &&
						g3 == colors.g3 &&
						g4 == colors.g4 &&
						textColor == colors.textColor &&
						bulletLight == colors.bulletLight &&
						bulletDark == colors.bulletDark;
				}

				bool operator!=(const Win7ButtonColors& colors)
				{
					return !(*this==colors);
				}

				void										SetAlphaWithoutText(unsigned char a);

				static Win7ButtonColors						Blend(const Win7ButtonColors& c1, const Win7ButtonColors& c2, int ratio, int total);

				static Win7ButtonColors						ButtonNormal();
				static Win7ButtonColors						ButtonActive();
				static Win7ButtonColors						ButtonPressed();
				static Win7ButtonColors						ButtonDisabled();
				
				static Win7ButtonColors						ItemNormal();
				static Win7ButtonColors						ItemActive();
				static Win7ButtonColors						ItemSelected();
				static Win7ButtonColors						ItemDisabled();
				
				static Win7ButtonColors						CheckedNormal(bool selected);
				static Win7ButtonColors						CheckedActive(bool selected);
				static Win7ButtonColors						CheckedPressed(bool selected);
				static Win7ButtonColors						CheckedDisabled(bool selected);

				static Win7ButtonColors						ToolstripButtonNormal();
				static Win7ButtonColors						ToolstripButtonActive();
				static Win7ButtonColors						ToolstripButtonPressed();
				static Win7ButtonColors						ToolstripButtonDisabled();

				static Win7ButtonColors						MenuBarButtonNormal();
				static Win7ButtonColors						MenuBarButtonActive();
				static Win7ButtonColors						MenuBarButtonPressed();
				static Win7ButtonColors						MenuBarButtonDisabled();

				static Win7ButtonColors						MenuItemButtonNormal();
				static Win7ButtonColors						MenuItemButtonNormalActive();
				static Win7ButtonColors						MenuItemButtonDisabled();
				static Win7ButtonColors						MenuItemButtonDisabledActive();

				static Win7ButtonColors						TabPageHeaderNormal();
				static Win7ButtonColors						TabPageHeaderActive();
				static Win7ButtonColors						TabPageHeaderSelected();
			};

			struct Win7ButtonElements
			{
				elements::GuiSolidBorderElement*			rectBorderElement;
				elements::GuiRoundBorderElement*			roundBorderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		topGradientElement;
				elements::GuiGradientBackgroundElement*		bottomGradientElement;
				elements::GuiSolidLabelElement*				textElement;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			mainComposition;
				compositions::GuiBoundsComposition*			backgroundComposition;
				compositions::GuiTableComposition*			gradientComposition;

				static Win7ButtonElements					Create(bool verticalGradient, bool roundBorder, Alignment::Type horizontal=Alignment::Center, Alignment::Type vertical=Alignment::Center);
				void										Apply(const Win7ButtonColors& colors);
			};

			struct Win7CheckedButtonElements
			{
				elements::GuiSolidBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		outerGradientElement;
				elements::GuiGradientBackgroundElement*		innerGradientElement;
				elements::GuiSolidLabelElement*				textElement;
				elements::GuiSolidLabelElement*				bulletTextElement;
				elements::GuiSolidBackgroundElement*		bulletBackgroundElement;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			mainComposition;

				static Win7CheckedButtonElements			Create(elements::ElementShape::Type shape, bool backgroundVisible);
				void										Apply(const Win7ButtonColors& colors);
			};

			struct Win7MenuItemButtonElements
			{
				elements::GuiRoundBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		gradientElement;
				elements::Gui3DSplitterElement*				splitterElement;
				compositions::GuiCellComposition*			splitterComposition;
				elements::GuiSolidLabelElement*				textElement;
				compositions::GuiBoundsComposition*			textComposition;
				elements::GuiSolidLabelElement*				subMenuTextElement;
				compositions::GuiGraphicsComposition*		subMenuTextComposition;
				compositions::GuiBoundsComposition*			mainComposition;

				static Win7MenuItemButtonElements			Create();
				void										Apply(const Win7ButtonColors& colors);
				void										SetActive(bool value);
				void										SetSubMenuExisting(bool value);
			};

			struct Win7TextBoxColors
			{
				Color										borderColor;
				Color										backgroundColor;

				bool operator==(const Win7TextBoxColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						backgroundColor == colors.backgroundColor;
				}

				bool operator!=(const Win7TextBoxColors& colors)
				{
					return !(*this==colors);
				}

				static Win7TextBoxColors					Blend(const Win7TextBoxColors& c1, const Win7TextBoxColors& c2, int ratio, int total);

				static Win7TextBoxColors					Normal();
				static Win7TextBoxColors					Active();
				static Win7TextBoxColors					Focused();
				static Win7TextBoxColors					Disabled();
			};

/***********************************************************************
Helper Functions
***********************************************************************/
			
			extern int										Win7GetColorAnimationLength();
			extern Color									Win7GetSystemWindowColor();
			extern Color									Win7GetSystemTabContentColor();
			extern Color									Win7GetSystemBorderColor();
			extern Color									Win7GetSystemTextColor(bool enabled);
			extern void										Win7SetFont(elements::GuiSolidLabelElement* element, compositions::GuiBoundsComposition* composition, const FontProperties& fontProperties);
			extern void										Win7CreateSolidLabelElement(elements::GuiSolidLabelElement*& element, compositions::GuiBoundsComposition*& composition, Alignment::Type horizontal, Alignment::Type vertical);

/***********************************************************************
Animation
***********************************************************************/

#define DEFINE_TRANSFERRING_ANIMATION(TSTATE, TSTYLECONTROLLER)\
				class TransferringAnimation : public compositions::GuiTimeBasedAnimation\
				{\
				protected:\
					TSTATE									colorBegin;\
					TSTATE									colorEnd;\
					TSTATE									colorCurrent;\
					TSTYLECONTROLLER*						style;\
					bool									stopped;\
					bool									enableAnimation;\
				public:\
					TransferringAnimation(TSTYLECONTROLLER* _style, const TSTATE& begin);\
					void									Play(int currentPosition, int totalLength)override;\
					void									Stop()override;\
					bool									GetEnableAnimation();\
					void									SetEnableAnimation(bool value);\
					void									Transfer(const TSTATE& end);\
				};\

/***********************************************************************
Container
***********************************************************************/

			class Win7EmptyStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7EmptyStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				Win7EmptyStyle(Color color);
				~Win7EmptyStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			class Win7WindowStyle : public Win7EmptyStyle, public Description<Win7WindowStyle>
			{
			public:
				Win7WindowStyle();
				~Win7WindowStyle();
			};

			class Win7MenuStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7MenuStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
				compositions::GuiBoundsComposition*			containerComposition;
			public:
				Win7MenuStyle();
				~Win7MenuStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			class Win7MenuBarStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7MenuBarStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				Win7MenuBarStyle();
				~Win7MenuBarStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			class Win7GroupBoxStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7GroupBoxStyle>
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Color, Win7GroupBoxStyle)

				compositions::GuiBoundsComposition*			boundsComposition;
				compositions::GuiBoundsComposition*			sinkBorderComposition;
				compositions::GuiBoundsComposition*			raisedBorderComposition;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			textBackgroundComposition;
				compositions::GuiBoundsComposition*			containerComposition;
				elements::GuiSolidLabelElement*				textElement;
				Ptr<TransferringAnimation>					transferringAnimation;

				void										SetMargins(int fontSize);
			public:
				Win7GroupBoxStyle();
				~Win7GroupBoxStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			class Win7TabStyle : public Object, public virtual controls::GuiTab::IStyleController, public Description<Win7TabStyle>
			{
			protected:
				compositions::GuiTableComposition*			boundsComposition;
				compositions::GuiBoundsComposition*			containerComposition;
				compositions::GuiStackComposition*			tabHeaderComposition;
				compositions::GuiBoundsComposition*			tabContentTopLineComposition;
				FontProperties								headerFont;
				controls::GuiTab::ICommandExecutor*			commandExecutor;

				Ptr<controls::GuiSelectableButton::MutexGroupController>	headerController;
				collections::List<controls::GuiSelectableButton*>			headerButtons;

				void										OnHeaderButtonClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										UpdateHeaderZOrder();
			public:
				Win7TabStyle();
				~Win7TabStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;

				void										SetCommandExecutor(controls::GuiTab::ICommandExecutor* value)override;
				void										InsertTab(int index)override;
				void										SetTabText(int index, const WString& value)override;
				void										RemoveTab(int index)override;
				void										MoveTab(int oldIndex, int newIndex)override;
				void										SetSelectedTab(int index)override;
				controls::GuiControl::IStyleController*		CreateTabPageStyleController()override;
			};

/***********************************************************************
Button
***********************************************************************/

			class Win7ButtonStyleBase : public Object, public virtual controls::GuiSelectableButton::IStyleController, public Description<Win7ButtonStyleBase>
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7ButtonStyleBase)

				Win7ButtonElements							elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isSelected;
				bool										transparentWhenInactive;
				bool										transparentWhenDisabled;

				virtual void								TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)=0;
			public:
				Win7ButtonStyleBase(bool verticalGradient, bool roundBorder, const Win7ButtonColors& initialColor, Alignment::Type horizontal, Alignment::Type vertical);
				~Win7ButtonStyleBase();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetSelected(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;

				bool										GetTransparentWhenInactive();
				void										SetTransparentWhenInactive(bool value);
				bool										GetTransparentWhenDisabled();
				void										SetTransparentWhenDisabled(bool value);
				bool										GetAutoSizeForText();
				void										SetAutoSizeForText(bool value);
			};

			class Win7ButtonStyle : public Win7ButtonStyleBase, public Description<Win7ButtonStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7ButtonStyle(bool verticalGradient=true);
				~Win7ButtonStyle();
			};

			class Win7ToolstripButtonStyle : public Win7ButtonStyleBase, public Description<Win7ToolstripButtonStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7ToolstripButtonStyle(bool transparent);
				~Win7ToolstripButtonStyle();
			};

			class Win7CheckBoxStyle : public Object, public virtual controls::GuiSelectableButton::IStyleController, public Description<Win7CheckBoxStyle>
			{
			public:
				enum BulletStyle
				{
					CheckBox,
					RadioButton,
				};
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7CheckBoxStyle)

				Win7CheckedButtonElements					elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isSelected;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected);
			public:
				Win7CheckBoxStyle(BulletStyle bulletStyle, bool backgroundVisible=true);
				~Win7CheckBoxStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetSelected(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

/***********************************************************************
Misc Buttons
***********************************************************************/

			class Win7SelectableItemStyle : public Win7ButtonStyleBase, public Description<Win7SelectableItemStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7SelectableItemStyle();
				~Win7SelectableItemStyle();
			};

			class Win7TabPageHeaderStyle : public Win7ButtonStyleBase, public Description<Win7TabPageHeaderStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7TabPageHeaderStyle();
				~Win7TabPageHeaderStyle();

				void										SetFont(const FontProperties& value)override;
			};

			class Win7ListViewColumnDropDownStyle : public Object, public virtual controls::GuiSelectableButton::IStyleController, public Description<Win7ListViewColumnDropDownStyle>
			{
			protected:
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isSelected;

				compositions::GuiBoundsComposition*			mainComposition;
				compositions::GuiBoundsComposition*			leftBorderComposition;
				compositions::GuiBoundsComposition*			borderComposition;
				compositions::GuiBoundsComposition*			gradientComposition;
				compositions::GuiBoundsComposition*			textComposition;

				elements::GuiGradientBackgroundElement*		leftBorderElement;
				elements::GuiSolidBorderElement*			borderElement;
				elements::GuiGradientBackgroundElement*		gradientElement;
				elements::GuiSolidLabelElement*				textElement;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected);
			public:
				Win7ListViewColumnDropDownStyle();
				~Win7ListViewColumnDropDownStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetSelected(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

			class Win7ListViewColumnHeaderStyle : public Object, public virtual controls::GuiSelectableButton::IStyleController, public Description<Win7ListViewColumnHeaderStyle>
			{
			protected:
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isSelected;

				compositions::GuiBoundsComposition*			mainComposition;
				compositions::GuiBoundsComposition*			rightBorderComposition;
				compositions::GuiBoundsComposition*			borderComposition;
				compositions::GuiBoundsComposition*			gradientComposition;
				compositions::GuiBoundsComposition*			textComposition;

				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		rightBorderElement;
				elements::GuiSolidBorderElement*			borderElement;
				elements::GuiGradientBackgroundElement*		gradientElement;
				elements::GuiSolidLabelElement*				textElement;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected);
			public:
				Win7ListViewColumnHeaderStyle();
				~Win7ListViewColumnHeaderStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetSelected(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

			class Win7TreeViewExpandingButtonStyle : public Object, public virtual controls::GuiSelectableButton::IStyleController, public Description<Win7TreeViewExpandingButtonStyle>
			{
			protected:
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isSelected;

				compositions::GuiBoundsComposition*			mainComposition;
				elements::GuiPolygonElement*				polygonElement;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected);
			public:
				Win7TreeViewExpandingButtonStyle();
				~Win7TreeViewExpandingButtonStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetSelected(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

/***********************************************************************
Menu Button
***********************************************************************/

			class Win7MenuBarButtonStyle : public Object, public virtual controls::GuiMenuButton::IStyleController, public Description<Win7MenuBarButtonStyle>
			{
			protected:
				Win7ButtonElements							elements;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isOpening;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool opening);
			public:
				Win7MenuBarButtonStyle();
				~Win7MenuBarButtonStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				controls::GuiMenu::IStyleController*		CreateSubMenuStyleController()override;
				void										SetSubMenuExisting(bool value)override;
				void										SetSubMenuOpening(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

			class Win7MenuItemButtonStyle : public Object, public virtual controls::GuiMenuButton::IStyleController, public Description<Win7MenuItemButtonStyle>
			{
			protected:
				Win7MenuItemButtonElements					elements;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isOpening;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool opening);
			public:
				Win7MenuItemButtonStyle();
				~Win7MenuItemButtonStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				controls::GuiMenu::IStyleController*		CreateSubMenuStyleController()override;
				void										SetSubMenuExisting(bool value)override;
				void										SetSubMenuOpening(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

			class Win7MenuSplitterStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7MenuSplitterStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				Win7MenuSplitterStyle();
				~Win7MenuSplitterStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

/***********************************************************************
ComboBox
***********************************************************************/
			
#pragma warning(push)
#pragma warning(disable:4250)
			class Win7DropDownComboBoxStyle : public Win7ButtonStyle, public virtual controls::GuiComboBoxBase::IStyleController, public Description<Win7DropDownComboBoxStyle>
			{
			protected:
				controls::GuiComboBoxBase::ICommandExecutor*	commandExecutor;
				compositions::GuiTableComposition*				table;
				compositions::GuiCellComposition*				textComposition;
				compositions::GuiCellComposition*				dropDownComposition;
				elements::GuiSolidLabelElement*					dropDownElement;

				void											TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7DropDownComboBoxStyle();
				~Win7DropDownComboBoxStyle();
				
				compositions::GuiGraphicsComposition*			GetContainerComposition()override;

				void											SetCommandExecutor(controls::GuiComboBoxBase::ICommandExecutor* value)override;
				void											OnClicked()override;
				void											OnPopupOpened()override;
				void											OnPopupClosed()override;
				void											OnItemSelected()override;
				controls::GuiControl::IStyleController*			CreatePopupStyle()override;
			};
#pragma warning(pop)

/***********************************************************************
Scroll
***********************************************************************/

			class Win7ScrollStyle : public common_styles::CommonScrollStyle, public Description<Win7ScrollStyle>
			{
			public:
				static const int							DefaultSize=18;
				static const int							ArrowSize=10;
			protected:
				controls::GuiButton::IStyleController*		CreateDecreaseButtonStyle(Direction direction);
				controls::GuiButton::IStyleController*		CreateIncreaseButtonStyle(Direction direction);
				controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction);
				void										InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)override;
			public:
				Win7ScrollStyle(Direction _direction);
				~Win7ScrollStyle();
			};

			class Win7TrackStyle : public common_styles::CommonTrackStyle, public Description<Win7TrackStyle>
			{
			public:
				static const int							TrackThickness=4;
				static const int							TrackPadding=8;
				static const int							HandleLong=21;
				static const int							HandleShort=10;

			protected:
				controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)override;
				void										InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)override;
				void										InstallTrack(compositions::GuiGraphicsComposition* trackComposition, Direction direction)override;
			public:
				Win7TrackStyle(Direction _direction);
				~Win7TrackStyle();
			};

/***********************************************************************
ScrollView
***********************************************************************/

			class Win7ScrollViewProvider : public Object, public virtual controls::GuiScrollView::IStyleProvider, public Description<Win7ScrollViewProvider>
			{
			public:
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;

				controls::GuiScroll::IStyleController*		CreateHorizontalScrollStyle()override;
				controls::GuiScroll::IStyleController*		CreateVerticalScrollStyle()override;
				int											GetDefaultScrollSize()override;
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override;
			};

/***********************************************************************
TextBox
***********************************************************************/

			class Win7TextBoxBackground : public Object, public Description<Win7TextBoxBackground>
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7TextBoxColors, Win7TextBoxBackground)
					
				elements::GuiRoundBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				compositions::GuiGraphicsComposition*		focusableComposition;
				bool										isMouseEnter;
				bool										isFocused;
				bool										isVisuallyEnabled;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiControl::IStyleController*		styleController;
				elements::GuiColorizedTextElement*			textElement;

				void										UpdateStyle();
				void										Apply(const Win7TextBoxColors& colors);

				void										OnBoundsMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				Win7TextBoxBackground();
				~Win7TextBoxBackground();
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller);
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value);
				void										SetVisuallyEnabled(bool value);
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition);
				void										InitializeTextElement(elements::GuiColorizedTextElement* _textElement);
			};

			class Win7MultilineTextBoxProvider : public Win7ScrollViewProvider, public Description<Win7MultilineTextBoxProvider>
			{
			protected:
				Win7TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				Win7MultilineTextBoxProvider();
				~Win7MultilineTextBoxProvider();
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetVisuallyEnabled(bool value)override;
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override;
			};

			class Win7SinglelineTextBoxProvider : public Object, public virtual controls::GuiSinglelineTextBox::IStyleProvider, public Description<Win7SinglelineTextBoxProvider>
			{
			protected:
				Win7TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				Win7SinglelineTextBoxProvider();
				~Win7SinglelineTextBoxProvider();

				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override;
			};

/***********************************************************************
List
***********************************************************************/

			class Win7TextListProvider : public Object, public virtual controls::list::TextItemStyleProvider::ITextItemStyleProvider, public Description<Win7TextListProvider>
			{
			public:
				Win7TextListProvider();
				~Win7TextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBackgroundStyleController()override;
				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};

			class Win7CheckTextListProvider : public Win7TextListProvider, public Description<Win7CheckTextListProvider>
			{
			public:
				Win7CheckTextListProvider();
				~Win7CheckTextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};

			class Win7RadioTextListProvider : public Win7TextListProvider, public Description<Win7RadioTextListProvider>
			{
			public:
				Win7RadioTextListProvider();
				~Win7RadioTextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};

#pragma warning(push)
#pragma warning(disable:4250)
			class Win7ListViewProvider : public Win7MultilineTextBoxProvider, public virtual controls::GuiListView::IStyleProvider, public Description<Win7ListViewProvider>
			{
			public:
				Win7ListViewProvider();
				~Win7ListViewProvider();

				controls::GuiSelectableButton::IStyleController*		CreateItemBackground()override;
				controls::GuiSelectableButton::IStyleController*		CreateColumnStyle()override;
				Color													GetPrimaryTextColor()override;
				Color													GetSecondaryTextColor()override;
				Color													GetItemSeparatorColor()override;
			};
			
			class Win7TreeViewProvider : public Win7MultilineTextBoxProvider, public virtual controls::GuiTreeView::IStyleProvider, public Description<Win7TreeViewProvider>
			{
			public:
				Win7TreeViewProvider();
				~Win7TreeViewProvider();

				controls::GuiSelectableButton::IStyleController*		CreateItemBackground()override;
				controls::GuiSelectableButton::IStyleController*		CreateItemExpandingDecorator()override;
				Color													GetTextColor()override;
			};
#pragma warning(pop)
		}
	}
}

#endif