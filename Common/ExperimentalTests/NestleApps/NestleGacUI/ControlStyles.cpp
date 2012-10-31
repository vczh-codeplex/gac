#include "ControlStyles.h"

/***********************************************************************
ContainerControlStyle
***********************************************************************/

ContainerControlStyle::ContainerControlStyle()
{
	composition=new GuiBoundsComposition();
	composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
}

compositions::GuiBoundsComposition* ContainerControlStyle::GetBoundsComposition()
{
	return composition;
}

compositions::GuiGraphicsComposition* ContainerControlStyle::GetContainerComposition()
{
	return composition;
}

void ContainerControlStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
{
}

void ContainerControlStyle::SetText(const WString& value)
{
}

void ContainerControlStyle::SetFont(const FontProperties& value)
{
}

void ContainerControlStyle::SetVisuallyEnabled(bool value)
{
}

/***********************************************************************
TransparentListBoxStyle
***********************************************************************/

TransparentListBoxStyle::TransparentListBoxStyle()
{
}

TransparentListBoxStyle::~TransparentListBoxStyle()
{
}

void TransparentListBoxStyle::AssociateStyleController(controls::GuiControl::IStyleController* controller)
{
}
			
void TransparentListBoxStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
{
}

void TransparentListBoxStyle::SetText(const WString& value)
{
}

void TransparentListBoxStyle::SetFont(const FontProperties& value)
{
}

void TransparentListBoxStyle::SetVisuallyEnabled(bool value)
{
}

controls::GuiScroll::IStyleController* TransparentListBoxStyle::CreateHorizontalScrollStyle()
{
	return GetCurrentTheme()->CreateHScrollStyle();
}

controls::GuiScroll::IStyleController* TransparentListBoxStyle::CreateVerticalScrollStyle()
{
	return GetCurrentTheme()->CreateVScrollStyle();
}

int TransparentListBoxStyle::GetDefaultScrollSize()
{
	return GetCurrentTheme()->GetScrollDefaultSize();
}

compositions::GuiGraphicsComposition* TransparentListBoxStyle::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
{
	return boundsComposition;
}