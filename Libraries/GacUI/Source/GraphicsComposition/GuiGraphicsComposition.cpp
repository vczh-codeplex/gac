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
GuiSubComponentMeasurer::MeasuringSource
***********************************************************************/

			GuiSubComponentMeasurer::MeasuringSource::MeasuringSource(const WString& _measuringCategory, GuiGraphicsComposition* _mainComposition)
				:measurer(0)
				,measuringCategory(_measuringCategory)
				,mainComposition(_mainComposition)
			{
			}

			GuiSubComponentMeasurer::MeasuringSource::~MeasuringSource()
			{
			}

			bool GuiSubComponentMeasurer::MeasuringSource::AddSubComponent(const WString& name, GuiGraphicsComposition* composition)
			{
				if(subComponents.Keys().Contains(name))
				{
					return false;
				}
				else
				{
					subComponents.Add(name, composition);
					return true;
				}
			}

			void GuiSubComponentMeasurer::MeasuringSource::AttachMeasurer(GuiSubComponentMeasurer* value)
			{
				measurer=value;
			}

			void GuiSubComponentMeasurer::MeasuringSource::DetachMeasurer(GuiSubComponentMeasurer* value)
			{
				measurer=0;
			}

			GuiSubComponentMeasurer* GuiSubComponentMeasurer::MeasuringSource::GetAttachedMeasurer()
			{
				return measurer;
			}

			WString GuiSubComponentMeasurer::MeasuringSource::GetMeasuringCategory()
			{
				return measuringCategory;
			}

			int GuiSubComponentMeasurer::MeasuringSource::GetSubComponentCount()
			{
				return subComponents.Count();
			}

			WString GuiSubComponentMeasurer::MeasuringSource::GetSubComponentName(int index)
			{
				return subComponents.Keys()[index];
			}

			GuiGraphicsComposition* GuiSubComponentMeasurer::MeasuringSource::GetSubComponentComposition(int index)
			{
				return subComponents.Values()[index];
			}

			GuiGraphicsComposition* GuiSubComponentMeasurer::MeasuringSource::GetSubComponentComposition(const WString& name)
			{
				return subComponents[name];
			}

			GuiGraphicsComposition* GuiSubComponentMeasurer::MeasuringSource::GetMainComposition()
			{
				return mainComposition;
			}

			void GuiSubComponentMeasurer::MeasuringSource::SubComponentPreferredMinSizeUpdated()
			{
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

			bool GuiSubComponentMeasurer::AttachMeasuringSource(IMeasuringSource* value)
			{
				if(!value->GetAttachedMeasurer())
				{
					measuringSources.Add(value);
					value->AttachMeasurer(this);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool GuiSubComponentMeasurer::DetachMeasuringSource(IMeasuringSource* value)
			{
				if(value->GetAttachedMeasurer()==this)
				{
					value->DetachMeasurer(this);
					measuringSources.Remove(value);
					return true;
				}
				else
				{
					return false;
				}
			}

			void GuiSubComponentMeasurer::MeasureAndUpdate(const WString& measuringCategory, Direction direction)
			{
				List<IMeasuringSource*> sources;
				FOREACH(IMeasuringSource*, source, measuringSources.Wrap())
				{
					if(source->GetMeasuringCategory()==measuringCategory)
					{
						sources.Add(source);
					}
				}

				Dictionary<WString, int> sizes;
				FOREACH(IMeasuringSource*, source, sources.Wrap())
				{
					int count=source->GetSubComponentCount();
					for(int i=0;i<count;i++)
					{
						WString name=source->GetSubComponentName(i);
						GuiGraphicsComposition* composition=source->GetSubComponentComposition(i);
						composition->SetPreferredMinSize(Size(0, 0));
						Size size=composition->GetBounds().GetSize();
						int sizeComponent=direction==Horizontal?size.x:size.y;

						int index=sizes.Keys().IndexOf(name);
						if(index==-1)
						{
							sizes.Add(name, sizeComponent);
						}
						else if(sizes.Values()[index]<sizeComponent)
						{
							sizes.Set(name, sizeComponent);
						}
					}
				}
				FOREACH(IMeasuringSource*, source, sources.Wrap())
				{
					int count=source->GetSubComponentCount();
					for(int i=0;i<count;i++)
					{
						WString name=source->GetSubComponentName(i);
						GuiGraphicsComposition* composition=source->GetSubComponentComposition(i);
						Size size=composition->GetPreferredMinSize();
						(direction==Horizontal?size.x:size.y)=sizes[name];
						composition->SetPreferredMinSize(size);
						source->SubComponentPreferredMinSizeUpdated();
					}
				}
			}
		}
	}
}