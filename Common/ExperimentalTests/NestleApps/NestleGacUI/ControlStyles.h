#ifndef NESTLEGACUI_CONTROLS_CONTROLSTYLES
#define NESTLEGACUI_CONTROLS_CONTROLSTYLES

#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIIncludes.h"

class ContainerControlStyle : public GuiControl::IStyleController
{
protected:
	GuiBoundsComposition*					composition;
public:
	ContainerControlStyle();

	compositions::GuiBoundsComposition*		GetBoundsComposition();
	compositions::GuiGraphicsComposition*	GetContainerComposition();
	void									SetFocusableComposition(compositions::GuiGraphicsComposition* value);
	void									SetText(const WString& value);
	void									SetFont(const FontProperties& value);
	void									SetVisuallyEnabled(bool value);
};

class TransparentListBoxStyle : public Object, public virtual controls::GuiScrollView::IStyleProvider
{
public:
	TransparentListBoxStyle();
	~TransparentListBoxStyle();

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

#endif