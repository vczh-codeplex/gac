#include "GuiToolstripMenu.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace compositions;

/***********************************************************************
GuiToolstripButton
***********************************************************************/

			void GuiToolstripButton::UpdateCommandContent()
			{
				if(command)
				{
					SetImage(command->GetImage());
					SetText(command->GetText());
					SetEnabled(command->GetEnabled());
					if(command->GetShortcut())
					{
						SetShortcutText(command->GetShortcut()->GetName());
					}
					else
					{
						SetShortcutText(L"");
					}
				}
				else
				{
					SetImage(0);
					SetText(L"");
					SetEnabled(true);
					SetShortcutText(L"");
				}
			}

			void GuiToolstripButton::OnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(command)
				{
					command->Executed.ExecuteWithNewSender(arguments, sender);
				}
			}

			void GuiToolstripButton::OnCommandDescriptionChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				UpdateCommandContent();
			}

			GuiToolstripButton::GuiToolstripButton(IStyleController* _styleController)
				:GuiMenuButton(_styleController)
			{
			}

			GuiToolstripButton::~GuiToolstripButton()
			{
			}

			Ptr<GuiToolstripCommand> GuiToolstripButton::GetCommand()
			{
				return command;
			}

			void GuiToolstripButton::SetCommand(Ptr<GuiToolstripCommand> value)
			{
				if(command)
				{
					command->DescriptionChanged.Detach(descriptionChangedHandler);
				}
				command=0;
				descriptionChangedHandler=0;
				if(value)
				{
					command=value;
					descriptionChangedHandler=command->DescriptionChanged.AttachMethod(this, &GuiToolstripButton::OnCommandDescriptionChanged);
				}
				UpdateCommandContent();
			}

/***********************************************************************
GuiToolstripCollection
***********************************************************************/

			void GuiToolstripCollection::RemoveAtInternal(vint index)
			{
				GuiControl* control=items[index];
				items.RemoveAt(index);
				GuiStackItemComposition* stackItem=stackComposition->GetStackItems()[index];

				stackComposition->RemoveChild(stackItem);
				stackItem->RemoveChild(control->GetBoundsComposition());
				delete stackItem;

				if(subComponentMeasurer)
				{
					GuiMenuButton* menuButton=dynamic_cast<GuiMenuButton*>(control);
					GuiSubComponentMeasurer::IMeasuringSource* measuringSource=
						dynamic_cast<GuiMenuButton::IStyleController*>(
							menuButton->GetStyleController()
							)->GetMeasuringSource();
					if(measuringSource)
					{
						subComponentMeasurer->DetachMeasuringSource(measuringSource);
					}
				}
				delete control;
			}

			void GuiToolstripCollection::InsertInternal(vint index, GuiControl* control)
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				control->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(control->GetBoundsComposition());
				stackComposition->InsertChild(index, stackItem);

				if(subComponentMeasurer)
				{
					GuiMenuButton* menuButton=dynamic_cast<GuiMenuButton*>(control);
					GuiSubComponentMeasurer::IMeasuringSource* measuringSource=
						dynamic_cast<GuiMenuButton::IStyleController*>(
							menuButton->GetStyleController()
							)->GetMeasuringSource();
					if(measuringSource)
					{
						subComponentMeasurer->AttachMeasuringSource(measuringSource);
					}
				}
			}

			GuiToolstripCollection::GuiToolstripCollection(compositions::GuiStackComposition* _stackComposition, Ptr<compositions::GuiSubComponentMeasurer> _subComponentMeasurer)
				:stackComposition(_stackComposition)
				,subComponentMeasurer(_subComponentMeasurer)
			{
			}

			GuiToolstripCollection::~GuiToolstripCollection()
			{
			}

			collections::IEnumerator<GuiControl*>* GuiToolstripCollection::CreateEnumerator()const
			{
				return items.Wrap().CreateEnumerator();
			}

			bool GuiToolstripCollection::Contains(GuiControl* const& item)const
			{
				return items.Contains(item);
			}

			vint GuiToolstripCollection::Count()const
			{
				return items.Count();
			}

			GuiControl* const& GuiToolstripCollection::Get(vint index)const
			{
				return items.Get(index);
			}

			GuiControl* const& GuiToolstripCollection::operator[](vint index)const
			{
				return items.Get(index);
			}

			vint GuiToolstripCollection::IndexOf(GuiControl* const& item)const
			{
				return items.IndexOf(item);
			}

			vint GuiToolstripCollection::Add(GuiControl* const& item)
			{
				return Insert(items.Count(), item);
			}

			bool GuiToolstripCollection::Remove(GuiControl* const& item)
			{
				return RemoveAt(items.IndexOf(item));
			}

			bool GuiToolstripCollection::RemoveAt(vint index)
			{
				if(0<=index && index<items.Count())
				{
					RemoveAtInternal(index);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool GuiToolstripCollection::RemoveRange(vint index, vint count)
			{
				if(count<=0) return false;
				if(0<=index && index<items.Count() && index+count<=items.Count())
				{
					while(count-->0)
					{
						RemoveAt(index+count);
					}
					return true;
				}
				else
				{
					return false;
				}
			}

			bool GuiToolstripCollection::Clear()
			{
				while(items.Count()>0)
				{
					RemoveAt(items.Count()-1);
				}
				return true;
			}

			vint GuiToolstripCollection::Insert(vint index, GuiControl* const& item)
			{
				if(index<=0 && index<=items.Count() && item && !item->GetBoundsComposition()->GetParent())
				{
					InsertInternal(index, item);
					return index;
				}
				else
				{
					return -1;
				}
			}

			bool GuiToolstripCollection::Set(vint index, GuiControl* const& item)
			{
				if(index<=0 && index<items.Count() && item && !item->GetBoundsComposition()->GetParent())
				{
					RemoveAtInternal(index);
					InsertInternal(index, item);
					return true;
				}
				else
				{
					return false;
				}
			}

/***********************************************************************
GuiToolstripMenu
***********************************************************************/

			GuiToolstripMenu::GuiToolstripMenu(IStyleController* _styleController, GuiControl* _owner)
				:GuiMenu(_styleController, _owner)
			{
			}

			GuiToolstripMenu::~GuiToolstripMenu()
			{
			}

/***********************************************************************
GuiToolstripMenuBar
***********************************************************************/
			
			GuiToolstripMenuBar::GuiToolstripMenuBar(IStyleController* _styleController)
				:GuiMenuBar(_styleController)
			{
			}

			GuiToolstripMenuBar::~GuiToolstripMenuBar()
			{
			}

/***********************************************************************
GuiToolstripToolbar
***********************************************************************/
				
			GuiToolstripToolbar::GuiToolstripToolbar(IStyleController* _styleController)
				:GuiControl(_styleController)
			{
			}

			GuiToolstripToolbar::~GuiToolstripToolbar()
			{
			}
		}
	}
}