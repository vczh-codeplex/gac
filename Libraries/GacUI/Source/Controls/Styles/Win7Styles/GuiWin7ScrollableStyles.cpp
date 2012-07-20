#include "GuiWin7ScrollableStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{
			using namespace collections;
			using namespace elements;
			using namespace compositions;
			using namespace controls;

/***********************************************************************
Win7ScrollStyle
***********************************************************************/

			controls::GuiButton::IStyleController* Win7ScrollStyle::CreateDecreaseButtonStyle(Direction direction)
			{
				Win7ButtonStyle* decreaseButtonStyle=new Win7ButtonStyle(direction==Horizontal);
				decreaseButtonStyle->SetTransparentWhenInactive(true);
				decreaseButtonStyle->SetTransparentWhenDisabled(true);
				return decreaseButtonStyle;
			}

			controls::GuiButton::IStyleController* Win7ScrollStyle::CreateIncreaseButtonStyle(Direction direction)
			{
				Win7ButtonStyle* increaseButtonStyle=new Win7ButtonStyle(direction==Horizontal);
				increaseButtonStyle->SetTransparentWhenInactive(true);
				increaseButtonStyle->SetTransparentWhenDisabled(true);
				return increaseButtonStyle;
			}

			controls::GuiButton::IStyleController* Win7ScrollStyle::CreateHandleButtonStyle(Direction direction)
			{
				Win7ButtonStyle* handleButtonStyle=new Win7ButtonStyle(direction==Horizontal);
				handleButtonStyle->SetTransparentWhenDisabled(true);
				return handleButtonStyle;
			}

			void Win7ScrollStyle::InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)
			{
				Color sinkColor=Win7GetSystemBorderSinkColor();
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(Win7GetSystemWindowColor());
				boundsComposition->SetOwnedElement(element);

				{
					GuiSideAlignedComposition* composition=new GuiSideAlignedComposition;
					composition->SetMaxLength(DefaultSize);
					composition->SetMaxRatio(0.2);
					boundsComposition->AddChild(composition);

					GuiGradientBackgroundElement* gradient=GuiGradientBackgroundElement::Create();
					gradient->SetColors(sinkColor, Win7GetSystemWindowColor());
					composition->SetOwnedElement(gradient);

					switch(direction)
					{
					case Horizontal:
						composition->SetDirection(GuiSideAlignedComposition::Top);
						gradient->SetDirection(GuiGradientBackgroundElement::Vertical);
						break;
					case Vertical:
						composition->SetDirection(GuiSideAlignedComposition::Left);
						gradient->SetDirection(GuiGradientBackgroundElement::Horizontal);
						break;
					}
				}
				{
					GuiSideAlignedComposition* composition=new GuiSideAlignedComposition;
					composition->SetMaxLength(DefaultSize);
					composition->SetMaxRatio(0.2);
					boundsComposition->AddChild(composition);

					GuiGradientBackgroundElement* gradient=GuiGradientBackgroundElement::Create();
					gradient->SetColors(Win7GetSystemWindowColor(), sinkColor);
					composition->SetOwnedElement(gradient);

					switch(direction)
					{
					case Horizontal:
						composition->SetDirection(GuiSideAlignedComposition::Bottom);
						gradient->SetDirection(GuiGradientBackgroundElement::Vertical);
						break;
					case Vertical:
						composition->SetDirection(GuiSideAlignedComposition::Right);
						gradient->SetDirection(GuiGradientBackgroundElement::Horizontal);
						break;
					}
				}
				{
					GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
					element->SetColor(sinkColor);

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					composition->SetOwnedElement(element);
					boundsComposition->AddChild(composition);
				}
			}

			Win7ScrollStyle::Win7ScrollStyle(Direction _direction)
				:CommonScrollStyle(_direction)
			{
				BuildStyle(DefaultSize, ArrowSize);
			}

			Win7ScrollStyle::~Win7ScrollStyle()
			{
			}

/***********************************************************************
Win7TrackStyle
***********************************************************************/

			controls::GuiButton::IStyleController* Win7TrackStyle::CreateHandleButtonStyle(Direction direction)
			{
				Win7ButtonStyle* handleButtonStyle=new Win7ButtonStyle(direction==Horizontal);
				return handleButtonStyle;
			}

			void Win7TrackStyle::InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(Win7GetSystemWindowColor());
				boundsComposition->SetOwnedElement(element);
			}

			void Win7TrackStyle::InstallTrack(compositions::GuiGraphicsComposition* trackComposition, Direction direction)
			{
				Gui3DBorderElement* element=Gui3DBorderElement::Create();
				element->SetColors(Color(176, 176, 176), Color(252, 252, 252));
				trackComposition->SetOwnedElement(element);
			}

			Win7TrackStyle::Win7TrackStyle(Direction _direction)
				:CommonTrackStyle(_direction)
			{
				BuildStyle(TrackThickness, TrackPadding, HandleLong, HandleShort);
			}

			Win7TrackStyle::~Win7TrackStyle()
			{
			}

/***********************************************************************
Win7ScrollViewProvider
***********************************************************************/

			Win7ScrollViewProvider::Win7ScrollViewProvider()
			{
			}

			Win7ScrollViewProvider::~Win7ScrollViewProvider()
			{
			}

			void Win7ScrollViewProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
			}
			
			void Win7ScrollViewProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7ScrollViewProvider::SetText(const WString& value)
			{
			}

			void Win7ScrollViewProvider::SetFont(const FontProperties& value)
			{
			}

			void Win7ScrollViewProvider::SetVisuallyEnabled(bool value)
			{
			}

			controls::GuiScroll::IStyleController* Win7ScrollViewProvider::CreateHorizontalScrollStyle()
			{
				return new Win7ScrollStyle(Win7ScrollStyle::Horizontal);
			}

			controls::GuiScroll::IStyleController* Win7ScrollViewProvider::CreateVerticalScrollStyle()
			{
				return new Win7ScrollStyle(Win7ScrollStyle::Vertical);
			}

			int Win7ScrollViewProvider::GetDefaultScrollSize()
			{
				return Win7ScrollStyle::DefaultSize;
			}

			compositions::GuiGraphicsComposition* Win7ScrollViewProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				GuiSolidBorderElement* border=GuiSolidBorderElement::Create();
				border->SetColor(Win7GetSystemBorderColor());
				boundsComposition->SetOwnedElement(border);
				boundsComposition->SetInternalMargin(Margin(1, 1, 1, 1));
				
				GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
				background->SetColor(Win7GetSystemWindowColor());

				GuiBoundsComposition* backgroundComposition=new GuiBoundsComposition;
				boundsComposition->AddChild(backgroundComposition);
				backgroundComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				backgroundComposition->SetOwnedElement(background);

				return boundsComposition;
			}

/***********************************************************************
Win7TextBoxBackground
***********************************************************************/

#define HOST_GETTER_BY_FOCUSABLE_COMPOSITION(STYLE) (style->focusableComposition->GetRelatedGraphicsHost())

			IMPLEMENT_TRANSFERRING_ANIMATION_BASE(Win7TextBoxColors, Win7TextBoxBackground, HOST_GETTER_BY_FOCUSABLE_COMPOSITION)
			{
				colorCurrent=Win7TextBoxColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->Apply(colorCurrent);
			}

			void Win7TextBoxBackground::UpdateStyle()
			{
				if(!isVisuallyEnabled)
				{
					transferringAnimation->Transfer(Win7TextBoxColors::Disabled());
				}
				else if(isFocused)
				{
					transferringAnimation->Transfer(Win7TextBoxColors::Focused());
				}
				else if(isMouseEnter)
				{
					transferringAnimation->Transfer(Win7TextBoxColors::Active());
				}
				else
				{
					transferringAnimation->Transfer(Win7TextBoxColors::Normal());
				}
			}

			void Win7TextBoxBackground::Apply(const Win7TextBoxColors& colors)
			{
				borderElement->SetColor(colors.borderColor);
				backgroundElement->SetColor(colors.backgroundColor);
			}

			void Win7TextBoxBackground::OnBoundsMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				isMouseEnter=true;
				UpdateStyle();
			}

			void Win7TextBoxBackground::OnBoundsMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				isMouseEnter=false;
				UpdateStyle();
			}

			void Win7TextBoxBackground::OnBoundsGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				isFocused=true;
				UpdateStyle();
			}

			void Win7TextBoxBackground::OnBoundsLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				isFocused=false;
				UpdateStyle();
			}

			Win7TextBoxBackground::Win7TextBoxBackground()
				:backgroundElement(0)
				,borderElement(0)
				,focusableComposition(0)
				,isMouseEnter(false)
				,isFocused(false)
				,isVisuallyEnabled(false)
				,styleController(0)
				,textElement(0)
			{
				transferringAnimation=new TransferringAnimation(this, Win7TextBoxColors::Normal());
			}

			Win7TextBoxBackground::~Win7TextBoxBackground()
			{
				transferringAnimation->Disable();
			}

			void Win7TextBoxBackground::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
			}
			
			void Win7TextBoxBackground::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				focusableComposition=value;
				focusableComposition->GetEventReceiver()->mouseEnter.AttachMethod(this, &Win7TextBoxBackground::OnBoundsMouseEnter);
				focusableComposition->GetEventReceiver()->mouseLeave.AttachMethod(this, &Win7TextBoxBackground::OnBoundsMouseLeave);
				focusableComposition->GetEventReceiver()->gotFocus.AttachMethod(this, &Win7TextBoxBackground::OnBoundsGotFocus);
				focusableComposition->GetEventReceiver()->lostFocus.AttachMethod(this, &Win7TextBoxBackground::OnBoundsLostFocus);
			}

			void Win7TextBoxBackground::SetVisuallyEnabled(bool value)
			{
				isVisuallyEnabled=value;
				UpdateStyle();
			}

			compositions::GuiGraphicsComposition* Win7TextBoxBackground::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				{
					GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
					background->SetColor(Color(255, 255, 255));

					GuiBoundsComposition* backgroundComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(backgroundComposition);
					backgroundComposition->SetAlignmentToParent(Margin(1, 1, 1, 1));
					backgroundComposition->SetOwnedElement(background);
					backgroundElement=background;
				}
				{
					GuiRoundBorderElement* border=GuiRoundBorderElement::Create();
					border->SetColor(Win7GetSystemBorderColor());
					border->SetRadius(2);
					borderElement=border;

					GuiBoundsComposition* borderComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(borderComposition);
					borderComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					borderComposition->SetOwnedElement(border);
				}
				Apply(Win7TextBoxColors::Normal());
				{
					GuiBoundsComposition* containerComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(containerComposition);
					containerComposition->SetAlignmentToParent(Margin(2, 2, 2, 2));
					return containerComposition;
				}
			}

			void Win7TextBoxBackground::InitializeTextElement(elements::GuiColorizedTextElement* _textElement)
			{
				textElement=_textElement;

				Array<text::ColorEntry> colors;
				colors.Resize(1);
				{
					colors[0]=Win7GetTextBoxTextColor();
				}
				textElement->SetColors(colors);
				textElement->SetCaretColor(Color(0, 0, 0));
			}

/***********************************************************************
Win7MultilineTextBoxProvider
***********************************************************************/

			Win7MultilineTextBoxProvider::Win7MultilineTextBoxProvider()
				:styleController(0)
			{
			}

			Win7MultilineTextBoxProvider::~Win7MultilineTextBoxProvider()
			{
			}

			void Win7MultilineTextBoxProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
				background.AssociateStyleController(controller);
			}
			
			void Win7MultilineTextBoxProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				background.SetFocusableComposition(value);
				GuiMultilineTextBox::StyleController* textBoxController=dynamic_cast<GuiMultilineTextBox::StyleController*>(styleController);
				if(textBoxController)
				{
					background.InitializeTextElement(textBoxController->GetTextElement());
				}
			}

			void Win7MultilineTextBoxProvider::SetVisuallyEnabled(bool value)
			{
				background.SetVisuallyEnabled(value);
			}

			compositions::GuiGraphicsComposition* Win7MultilineTextBoxProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				return background.InstallBackground(boundsComposition);
			}

/***********************************************************************
Win7SinglelineTextBoxProvider
***********************************************************************/

			Win7SinglelineTextBoxProvider::Win7SinglelineTextBoxProvider()
				:styleController(0)
			{
			}

			Win7SinglelineTextBoxProvider::~Win7SinglelineTextBoxProvider()
			{
			}

			void Win7SinglelineTextBoxProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
				background.AssociateStyleController(controller);
			}
			
			void Win7SinglelineTextBoxProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				background.SetFocusableComposition(value);
				GuiSinglelineTextBox::StyleController* textBoxController=dynamic_cast<GuiSinglelineTextBox::StyleController*>(styleController);
				background.InitializeTextElement(textBoxController->GetTextElement());
			}

			void Win7SinglelineTextBoxProvider::SetText(const WString& value)
			{
			}

			void Win7SinglelineTextBoxProvider::SetFont(const FontProperties& value)
			{
			}

			void Win7SinglelineTextBoxProvider::SetVisuallyEnabled(bool value)
			{
				background.SetVisuallyEnabled(value);
			}

			compositions::GuiGraphicsComposition* Win7SinglelineTextBoxProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				return background.InstallBackground(boundsComposition);
			}
		}
	}
}