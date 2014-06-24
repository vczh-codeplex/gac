#include "GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace compositions
		{
			using namespace collections;
			using namespace controls;
			using namespace elements;

/***********************************************************************
GuiSubComponentMeasurerSource
***********************************************************************/

			GuiSubComponentMeasurerSource::GuiSubComponentMeasurerSource(GuiGraphicsComposition* _containerComposition)
				:measurer(0)
				, containerComposition(_containerComposition)
			{
			}

			GuiSubComponentMeasurerSource::~GuiSubComponentMeasurerSource()
			{
			}

			GuiGraphicsComposition* GuiSubComponentMeasurerSource::GetContainerComposition()
			{
				return containerComposition;
			}

			void GuiSubComponentMeasurerSource::AddSubComponent(const WString& name, GuiGraphicsComposition* composition, Direction direction)
			{
				SubComponent sc;
				sc.name = name;
				sc.composition = composition;
				sc.direction = direction;
				subComponents.Add(sc);
			}

			GuiSubComponentMeasurer* GuiSubComponentMeasurerSource::GetAttachedMeasurer()
			{
				return measurer;
			}

			vint GuiSubComponentMeasurerSource::GetSubComponentCount()
			{
				return subComponents.Count();
			}

			WString GuiSubComponentMeasurerSource::GetSubComponentName(vint index)
			{
				return subComponents[index].name;
			}

			GuiGraphicsComposition* GuiSubComponentMeasurerSource::GetSubComponentComposition(vint index)
			{
				return subComponents[index].composition;
			}

			GuiSubComponentMeasurerSource::Direction GuiSubComponentMeasurerSource::GetSubComponentDirection(vint index)
			{
				return subComponents[index].direction;
			}

/***********************************************************************
GuiSubComponentMeasurer
***********************************************************************/

			GuiSubComponentMeasurer::GuiSubComponentMeasurer()
			{
			}

			GuiSubComponentMeasurer::~GuiSubComponentMeasurer()
			{
			}

			bool GuiSubComponentMeasurer::AttachMeasuringSource(GuiSubComponentMeasurerSource* value)
			{
				if(!value->GetAttachedMeasurer())
				{
					measuringSources.Add(value);
					value->measurer = this;
					return true;
				}
				else
				{
					return false;
				}
			}

			bool GuiSubComponentMeasurer::DetachMeasuringSource(GuiSubComponentMeasurerSource* value)
			{
				if(value->GetAttachedMeasurer()==this)
				{
					value->measurer = 0;
					measuringSources.Remove(value);
					return true;
				}
				else
				{
					return false;
				}
			}

			void GuiSubComponentMeasurer::MeasureAndUpdate()
			{
				Dictionary<WString, vint> sizes;
				FOREACH(GuiSubComponentMeasurerSource*, source, measuringSources)
				{
					vint count = source->GetSubComponentCount();
					for (vint i = 0; i < count; i++)
					{
						auto name = source->GetSubComponentName(i);
						auto composition = source->GetSubComponentComposition(i);
						auto direction = source->GetSubComponentDirection(i);

						composition->SetPreferredMinSize(Size(0, 0));
						Size size = composition->GetPreferredBounds().GetSize();
						vint sizeComponent = direction == Horizontal ? size.x : size.y;

						vint index = sizes.Keys().IndexOf(name);
						if (index == -1)
						{
							sizes.Add(name, sizeComponent);
						}
						else if (sizes.Values().Get(index) < sizeComponent)
						{
							sizes.Set(name, sizeComponent);
						}
					}
				}

				FOREACH(GuiSubComponentMeasurerSource*, source, measuringSources)
				{
					vint count = source->GetSubComponentCount();
					for (vint i = 0; i < count; i++)
					{
						auto name = source->GetSubComponentName(i);
						auto composition = source->GetSubComponentComposition(i);
						auto direction = source->GetSubComponentDirection(i);
						Size size = composition->GetPreferredMinSize();
						(direction == Horizontal ? size.x : size.y) = sizes[name];
						composition->SetPreferredMinSize(size);
					}
					source->GetContainerComposition()->ForceCalculateSizeImmediately();
				}
			}
		}
	}
}