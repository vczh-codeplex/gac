#include "GuiWin8ControlStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{
			using namespace collections;
			using namespace elements;
			using namespace compositions;
			using namespace controls;

/***********************************************************************
Win8EmptyStyle
***********************************************************************/

			Win8EmptyStyle::Win8EmptyStyle(Color color)
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(color);
				
				boundsComposition=new GuiBoundsComposition;
				boundsComposition->SetOwnedElement(element);
			}

			Win8EmptyStyle::~Win8EmptyStyle()
			{
			}

			compositions::GuiBoundsComposition* Win8EmptyStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win8EmptyStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win8EmptyStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win8EmptyStyle::SetText(const WString& value)
			{
			}

			void Win8EmptyStyle::SetFont(const FontProperties& value)
			{
			}

			void Win8EmptyStyle::SetVisuallyEnabled(bool value)
			{
			}

/***********************************************************************
Win8WindowStyle
***********************************************************************/

			Win8WindowStyle::Win8WindowStyle()
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(Win8GetSystemWindowColor());
				
				boundsComposition=new GuiBoundsComposition;
				boundsComposition->SetOwnedElement(element);
			}

			Win8WindowStyle::~Win8WindowStyle()
			{
			}

			compositions::GuiBoundsComposition* Win8WindowStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win8WindowStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win8WindowStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win8WindowStyle::SetText(const WString& value)
			{
			}

			void Win8WindowStyle::SetFont(const FontProperties& value)
			{
			}

			void Win8WindowStyle::SetVisuallyEnabled(bool value)
			{
			}

/***********************************************************************
Win8LabelStyle
***********************************************************************/

			Win8LabelStyle::Win8LabelStyle()
			{
				textElement=GuiSolidLabelElement::Create();
				textElement->SetColor(GetDefaultTextColor());
				
				boundsComposition=new GuiBoundsComposition;
				boundsComposition->SetOwnedElement(textElement);
				boundsComposition->SetMinSizeLimitation(GuiBoundsComposition::LimitToElement);
			}

			Win8LabelStyle::~Win8LabelStyle()
			{
			}

			compositions::GuiBoundsComposition* Win8LabelStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win8LabelStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win8LabelStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win8LabelStyle::SetText(const WString& value)
			{
				textElement->SetText(value);
			}

			void Win8LabelStyle::SetFont(const FontProperties& value)
			{
				textElement->SetFont(value);
			}

			void Win8LabelStyle::SetVisuallyEnabled(bool value)
			{
			}

			Color Win8LabelStyle::GetDefaultTextColor()
			{
				return Win8GetSystemTextColor(true);
			}

			void Win8LabelStyle::SetTextColor(Color value)
			{
				textElement->SetColor(value);
			}
		}
	}
}