#include "NestlePage.h"

/***********************************************************************
NestlePage::StyleController
***********************************************************************/

NestlePage::StyleController::StyleController()
{
	composition=new GuiBoundsComposition();
	composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
}

compositions::GuiBoundsComposition* NestlePage::StyleController::GetBoundsComposition()
{
	return composition;
}

compositions::GuiGraphicsComposition* NestlePage::StyleController::GetContainerComposition()
{
	return composition;
}

void NestlePage::StyleController::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
{
}

void NestlePage::StyleController::SetText(const WString& value)
{
}

void NestlePage::StyleController::SetFont(const FontProperties& value)
{
}

void NestlePage::StyleController::SetVisuallyEnabled(bool value)
{
}

/***********************************************************************
NestlePage
***********************************************************************/

NestlePage::NestlePage()
	:GuiControl(new StyleController)
{
}