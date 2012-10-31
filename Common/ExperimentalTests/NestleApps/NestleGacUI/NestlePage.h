#ifndef NESTLEGACUI_PAGES_NESTLEPAGE
#define NESTLEGACUI_PAGES_NESTLEPAGE

#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIIncludes.h"
#include "..\NestleConsole\NestleSDK.h"

using namespace vl::nestle;

class NestlePage : public GuiControl
{
private:
	class StyleController : public GuiControl::IStyleController
	{
	protected:
		GuiBoundsComposition*					composition;
	public:
		StyleController();

		compositions::GuiBoundsComposition*		GetBoundsComposition();
		compositions::GuiGraphicsComposition*	GetContainerComposition();
		void									SetFocusableComposition(compositions::GuiGraphicsComposition* value);
		void									SetText(const WString& value);
		void									SetFont(const FontProperties& value);
		void									SetVisuallyEnabled(bool value);
	};
public:
	NestlePage();
};

#endif