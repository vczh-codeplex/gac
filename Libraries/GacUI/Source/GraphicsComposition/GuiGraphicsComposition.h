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
			/// <summary>Measuring source for <see cref="GuiSubComponentMeasurer"/>.</summary>
			class GuiSubComponentMeasurerSource : public Object, public Description<GuiSubComponentMeasurerSource>
			{
				friend class GuiSubComponentMeasurer;
			public:
				/// <summary>The measure direction.</summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.compositions.GuiSubComponentMeasurerSource.Direction]Measuring width.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.compositions.GuiSubComponentMeasurerSource.Direction]Measuring height.</summary>
					Vertical,
				};
			protected:
				struct SubComponent
				{
					WString							name;
					GuiGraphicsComposition*			composition;
					Direction						direction;
				};

				GuiSubComponentMeasurer*			measurer;
				GuiGraphicsComposition*				containerComposition;
				collections::List<SubComponent>		subComponents;

			public:
				/// <summary>Create a measuring source.</summary>
				GuiSubComponentMeasurerSource(GuiGraphicsComposition* _containerComposition);
				~GuiSubComponentMeasurerSource();

				/// <summary>Get the container composition.</summary>
				/// <returns>The container composition.</returns>
				GuiGraphicsComposition*				GetContainerComposition();

				/// <summary>Add a sub component to the measuring source.</summary>
				/// <param name="name">The name of the sub component.</param>
				/// <param name="composition">The composition of the sub component.</param>
				/// <param name="direction">The direction of the sub component.</param>
				void								AddSubComponent(const WString& name, GuiGraphicsComposition* composition, Direction direction);
				/// <summary>Get the <see cref="GuiSubComponentMeasurer"/> that this measuring source attached to.</summary>
				/// <returns>The attached measurer.</returns>
				GuiSubComponentMeasurer*			GetAttachedMeasurer();
				/// <summary>Get the sub component count.</summary>
				/// <returns>The sub component count.</returns>
				vint								GetSubComponentCount();
				/// <summary>Get the name of the specified sub component.</summary>
				/// <returns>The name of the specified sub component.</returns>
				/// <param name="index">The index of the specified sub component.</param>
				WString								GetSubComponentName(vint index);
				/// <summary>Get the composition of the specified sub component.</summary>
				/// <returns>The composition of the specified sub component.</returns>
				/// <param name="index">The index of the specified sub component.</param>
				GuiGraphicsComposition*				GetSubComponentComposition(vint index);
				/// <summary>Get the direction of the specified sub component.</summary>
				/// <returns>The direction of the specified sub component.</returns>
				/// <param name="index">The index of the specified sub component.</param>
				Direction							GetSubComponentDirection(vint index);
			};

			/// <summary>Sub component measurer. This class is used to measure and update sub components of a group of controls.</summary>
			class GuiSubComponentMeasurer : public Object, public Description<GuiSubComponentMeasurer>
			{
			public:
				/// <summary>The measure direction.</summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.compositions.GuiSubComponentMeasurer.Direction]Measuring width.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.compositions.GuiSubComponentMeasurer.Direction]Measuring height.</summary>
					Vertical,
				};

			protected:
				typedef collections::List<GuiSubComponentMeasurerSource*>	MeasuringSourceList;

				MeasuringSourceList						measuringSources;
			public:
				/// <summary>Create a sub component measurer.</summary>
				GuiSubComponentMeasurer();
				~GuiSubComponentMeasurer();

				/// <summary>Attach a measuring source.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The measuring source.</param>
				bool									AttachMeasuringSource(GuiSubComponentMeasurerSource* value);
				/// <summary>Detach a measuring source.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The measuring source.</param>
				bool									DetachMeasuringSource(GuiSubComponentMeasurerSource* value);
				/// <summary>Measure and update all sub components under the specified measuring category.</summary>
				/// <param name="measuringCategory">The measuring category.</param>
				/// <param name="direction">The measuring direction.</param>
				void									MeasureAndUpdate();
			};
		}
	}
}

#endif