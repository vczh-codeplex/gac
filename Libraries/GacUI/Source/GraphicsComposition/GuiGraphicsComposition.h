/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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
			class GuiSubComponentMeasurer : public Object, public Description<GuiSubComponentMeasurer>
			{
			public:
				class IMeasuringSource : public Interface
				{
				public:
					virtual void						AttachMeasurer(GuiSubComponentMeasurer* value)=0;
					virtual void						DetachMeasurer(GuiSubComponentMeasurer* value)=0;
					virtual GuiSubComponentMeasurer*	GetAttachedMeasurer()=0;
					virtual WString						GetMeasuringCategory()=0;
					virtual int							GetSubComponentCount()=0;
					virtual WString						GetSubComponentName(int index)=0;
					virtual GuiGraphicsComposition*		GetSubComponentComposition(int index)=0;
					virtual GuiGraphicsComposition*		GetSubComponentComposition(const WString& name)=0;
					virtual GuiGraphicsComposition*		GetMainComposition()=0;
					virtual void						SubComponentPreferredMinSizeUpdated()=0;
				};

				enum Direction
				{
					Horizontal,
					Vertical,
				};

				class MeasuringSource : public Object, public IMeasuringSource
				{
					typedef collections::Dictionary<WString, GuiGraphicsComposition*>	SubComponentMap;
				protected:
					GuiSubComponentMeasurer*			measurer;
					WString								measuringCategory;
					GuiGraphicsComposition*				mainComposition;
					SubComponentMap						subComponents;

				public:
					MeasuringSource(const WString& _measuringCategory, GuiGraphicsComposition* _mainComposition);
					~MeasuringSource();

					bool								AddSubComponent(const WString& name, GuiGraphicsComposition* composition);
					void								AttachMeasurer(GuiSubComponentMeasurer* value);
					void								DetachMeasurer(GuiSubComponentMeasurer* value);
					GuiSubComponentMeasurer*			GetAttachedMeasurer();
					WString								GetMeasuringCategory();
					int									GetSubComponentCount();
					WString								GetSubComponentName(int index);
					GuiGraphicsComposition*				GetSubComponentComposition(int index);
					GuiGraphicsComposition*				GetSubComponentComposition(const WString& name);
					GuiGraphicsComposition*				GetMainComposition();
				};
			protected:
				typedef collections::List<IMeasuringSource*>	MeasuringSourceList;

				MeasuringSourceList						measuringSources;
			public:
				GuiSubComponentMeasurer();
				~GuiSubComponentMeasurer();

				bool									AttachMeasuringSource(IMeasuringSource* value);
				bool									DetachMeasuringSource(IMeasuringSource* value);
				void									MeasureAndUpdate(const WString& measuringCategory, Direction direction);
			};
		}
	}
}

#endif