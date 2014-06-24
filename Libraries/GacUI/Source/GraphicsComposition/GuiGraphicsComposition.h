/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Composition System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSCOMPOSITION
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSCOMPOSITION

#include "GuiGraphicsCompositionBase.h"
#include "GuiGraphicsBasicComposition.h"
#include "GuiGraphicsTableComposition.h"
#include "GuiGraphicsStackComposition.h"
#include "GuiGraphicsSpecializedComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace compositions
		{
			class GuiSharedSizeItemComposition;
			class GuiSharedSizeRootComposition;

			class GuiSharedSizeItemComposition : public GuiBoundsComposition
			{
			protected:
				GuiSharedSizeRootComposition*						parentRoot;
				WString												group;
				bool												sharedWidth;
				bool												sharedHeight;

				void												Update();
				void												OnParentLineChanged()override;
			public:
				GuiSharedSizeItemComposition();
				~GuiSharedSizeItemComposition();

				const WString&										GetGroup();
				void												SetGroup(const WString& value);
				bool												GetSharedWidth();
				void												SetSharedWidth(bool value);
				bool												GetSharedHeight();
				void												SetSharedHeight(bool value);
			};

			class GuiSharedSizeRootComposition :public GuiBoundsComposition
			{
				friend class GuiSharedSizeItemComposition;
			protected:
				collections::List<GuiSharedSizeItemComposition*>	childItems;

			public:
				GuiSharedSizeRootComposition();
				~GuiSharedSizeRootComposition();

				void												ForceCalculateSizeImmediately()override;
			};
		}
	}
}

#endif