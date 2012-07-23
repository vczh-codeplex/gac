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
			/// <summary>Sub component measurer. This class is used to measure and update sub components of a group of controls.</summary>
			class GuiSubComponentMeasurer : public Object, public Description<GuiSubComponentMeasurer>
			{
			public:
				/// <summary>Measuring source for <see cref="GuiSubComponentMeasurer"/>.</summary>
				class IMeasuringSource : public Interface
				{
				public:
					/// <summary>Called when a this measuring source is attached to a <see cref="GuiSubComponentMeasurer"/>.</summary>
					/// <param name="value">The measurer to attach to.</param>
					virtual void						AttachMeasurer(GuiSubComponentMeasurer* value)=0;
					/// <summary>Called when a this measuring source is detached from a <see cref="GuiSubComponentMeasurer"/>.</summary>
					/// <param name="value">The measurer to detach from.</param>
					virtual void						DetachMeasurer(GuiSubComponentMeasurer* value)=0;
					/// <summary>Get the <see cref="GuiSubComponentMeasurer"/> that this measuring source attached to.</summary>
					/// <returns>The attached measurer.</returns>
					virtual GuiSubComponentMeasurer*	GetAttachedMeasurer()=0;
					/// <summary>Get the measuring category for this measuring source. Only measuring sources that has the same category will be put together for measuring.</summary>
					/// <returns>The measuring category for this measuring source.</returns>
					virtual WString						GetMeasuringCategory()=0;
					/// <summary>Get the sub component count.</summary>
					/// <returns>The sub component count.</returns>
					virtual int							GetSubComponentCount()=0;
					/// <summary>Get the name of the specified sub component.</summary>
					/// <returns>The name of the specified sub component.</returns>
					/// <param name="index">The index of the specified sub component.</param>
					virtual WString						GetSubComponentName(int index)=0;
					/// <summary>Get the composition of the specified sub component.</summary>
					/// <returns>The composition of the specified sub component.</returns>
					/// <param name="index">The index of the specified sub component.</param>
					virtual GuiGraphicsComposition*		GetSubComponentComposition(int index)=0;
					/// <summary>Get the composition of the specified sub component.</summary>
					/// <returns>The composition of the specified sub component.</returns>
					/// <param name="name">The name of the specified sub component.</param>
					virtual GuiGraphicsComposition*		GetSubComponentComposition(const WString& name)=0;
					/// <summary>The composition for the control.</summary>
					/// <returns>The composition for the control.</returns>
					virtual GuiGraphicsComposition*		GetMainComposition()=0;
					/// <summary>Called when all preferred min sizes of all sub components is modified via a call to <see cref="GuiGraphicsComposition::SetPreferredMinSize"/>.</summary>
					virtual void						SubComponentPreferredMinSizeUpdated()=0;
				};
				
				/// <summary>The measure direction.</summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.compositions.GuiSubComponentMeasurer.Direction]Measuring width.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.compositions.GuiSubComponentMeasurer.Direction]Measuring height.</summary>
					Vertical,
				};

				/// <summary>The default implementation for <see cref="IMeasuringSource"/>.</summary>
				class MeasuringSource : public Object, public IMeasuringSource
				{
					typedef collections::Dictionary<WString, GuiGraphicsComposition*>	SubComponentMap;
				protected:
					GuiSubComponentMeasurer*			measurer;
					WString								measuringCategory;
					GuiGraphicsComposition*				mainComposition;
					SubComponentMap						subComponents;

				public:
					/// <summary>Create a measuring source.</summary>
					/// <param name="_measuringCategory">The measuring category.</param>
					/// <param name="_mainComposition">The main composition.</param>
					MeasuringSource(const WString& _measuringCategory, GuiGraphicsComposition* _mainComposition);
					~MeasuringSource();

					/// <summary>Add a sub component to the measuring source.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="name">The name of the sub component.</param>
					/// <param name="composition">The composition of the sub component.</param>
					bool								AddSubComponent(const WString& name, GuiGraphicsComposition* composition);
					void								AttachMeasurer(GuiSubComponentMeasurer* value)override;
					void								DetachMeasurer(GuiSubComponentMeasurer* value)override;
					GuiSubComponentMeasurer*			GetAttachedMeasurer()override;
					WString								GetMeasuringCategory()override;
					int									GetSubComponentCount()override;
					WString								GetSubComponentName(int index)override;
					GuiGraphicsComposition*				GetSubComponentComposition(int index)override;
					GuiGraphicsComposition*				GetSubComponentComposition(const WString& name)override;
					GuiGraphicsComposition*				GetMainComposition()override;
					void								SubComponentPreferredMinSizeUpdated()override;
				};
			protected:
				typedef collections::List<IMeasuringSource*>	MeasuringSourceList;

				MeasuringSourceList						measuringSources;
			public:
				/// <summary>Create a sub component measurer.</summary>
				GuiSubComponentMeasurer();
				~GuiSubComponentMeasurer();

				/// <summary>Attach a measuring source.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The measuring source.</param>
				bool									AttachMeasuringSource(IMeasuringSource* value);
				/// <summary>Detach a measuring source.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The measuring source.</param>
				bool									DetachMeasuringSource(IMeasuringSource* value);
				/// <summary>Measure and update all sub components under the specified measuring category.</summary>
				/// <param name="measuringCategory">The measuring category.</param>
				/// <param name="direction">The measuring direction.</param>
				void									MeasureAndUpdate(const WString& measuringCategory, Direction direction);
			};
		}
	}
}

#endif