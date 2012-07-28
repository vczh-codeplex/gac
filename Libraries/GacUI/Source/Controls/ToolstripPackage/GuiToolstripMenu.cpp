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
GuiToolstripCollection
***********************************************************************/

			void GuiToolstripCollection::InvokeUpdateLayout()
			{
				if(contentCallback)
				{
					contentCallback->UpdateLayout();
				}
			}

			void GuiToolstripCollection::OnInterestingMenuButtonPropertyChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				InvokeUpdateLayout();
			}

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
					if(menuButton)
					{
						GuiSubComponentMeasurer::IMeasuringSource* measuringSource=
							dynamic_cast<GuiMenuButton::IStyleController*>(
								menuButton->GetStyleController()
								)->GetMeasuringSource();
						if(measuringSource)
						{
							subComponentMeasurer->DetachMeasuringSource(measuringSource);
						}
					}
				}
				delete control;
				InvokeUpdateLayout();
			}

			void GuiToolstripCollection::InsertInternal(vint index, GuiControl* control)
			{
				items.Insert(index, control);
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				control->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(control->GetBoundsComposition());
				stackComposition->InsertChild(index, stackItem);

				if(subComponentMeasurer)
				{
					GuiMenuButton* menuButton=dynamic_cast<GuiMenuButton*>(control);
					if(menuButton)
					{
						GuiSubComponentMeasurer::IMeasuringSource* measuringSource=
							dynamic_cast<GuiMenuButton::IStyleController*>(
								menuButton->GetStyleController()
								)->GetMeasuringSource();
						if(measuringSource)
						{
							subComponentMeasurer->AttachMeasuringSource(measuringSource);
						}
						menuButton->TextChanged.AttachMethod(this, &GuiToolstripCollection::OnInterestingMenuButtonPropertyChanged);
						menuButton->ShortcutTextChanged.AttachMethod(this, &GuiToolstripCollection::OnInterestingMenuButtonPropertyChanged);
					}
				}
				InvokeUpdateLayout();
			}

			GuiToolstripCollection::GuiToolstripCollection(IContentCallback* _contentCallback, compositions::GuiStackComposition* _stackComposition, Ptr<compositions::GuiSubComponentMeasurer> _subComponentMeasurer)
				:contentCallback(_contentCallback)
				,stackComposition(_stackComposition)
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
				if(0<=index && index<=items.Count() && item && !item->GetBoundsComposition()->GetParent())
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
				if(0<=index && index<items.Count() && item && !item->GetBoundsComposition()->GetParent())
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

			void GuiToolstripMenu::UpdateLayout()
			{
				subComponentMeasurer->MeasureAndUpdate(GuiMenuButton::MenuItemSubComponentMeasuringCategoryName, GuiSubComponentMeasurer::Horizontal);
			}

			GuiToolstripMenu::GuiToolstripMenu(IStyleController* _styleController, GuiControl* _owner)
				:GuiMenu(_styleController, _owner)
			{
				stackComposition=new GuiStackComposition;
				stackComposition->SetDirection(GuiStackComposition::Vertical);
				stackComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				GetContainerComposition()->AddChild(stackComposition);

				subComponentMeasurer=new GuiSubComponentMeasurer;
				toolstripItems=new GuiToolstripCollection(this, stackComposition, subComponentMeasurer);
			}

			GuiToolstripMenu::~GuiToolstripMenu()
			{
			}

			GuiToolstripCollection& GuiToolstripMenu::GetToolstripItems()
			{
				return *toolstripItems.Obj();
			}

/***********************************************************************
GuiToolstripMenuBar
***********************************************************************/
			
			GuiToolstripMenuBar::GuiToolstripMenuBar(IStyleController* _styleController)
				:GuiMenuBar(_styleController)
			{
				stackComposition=new GuiStackComposition;
				stackComposition->SetDirection(GuiStackComposition::Horizontal);
				stackComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				GetContainerComposition()->AddChild(stackComposition);

				toolstripItems=new GuiToolstripCollection(0, stackComposition, 0);
			}

			GuiToolstripMenuBar::~GuiToolstripMenuBar()
			{
			}

			GuiToolstripCollection& GuiToolstripMenuBar::GetToolstripItems()
			{
				return *toolstripItems.Obj();
			}

/***********************************************************************
GuiToolstripToolbar
***********************************************************************/
				
			GuiToolstripToolbar::GuiToolstripToolbar(IStyleController* _styleController)
				:GuiControl(_styleController)
			{
				stackComposition=new GuiStackComposition;
				stackComposition->SetDirection(GuiStackComposition::Horizontal);
				stackComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				GetContainerComposition()->AddChild(stackComposition);

				toolstripItems=new GuiToolstripCollection(0, stackComposition, 0);
			}

			GuiToolstripToolbar::~GuiToolstripToolbar()
			{
			}

			GuiToolstripCollection& GuiToolstripToolbar::GetToolstripItems()
			{
				return *toolstripItems.Obj();
			}

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

			GuiToolstripMenu* GuiToolstripButton::GetToolstripSubMenu()
			{
				return dynamic_cast<GuiToolstripMenu*>(GetSubMenu());
			}

			void GuiToolstripButton::CreateToolstripSubMenu(GuiToolstripMenu::IStyleController* subMenuStyleController)
			{
				if(!subMenu)
				{
					GuiToolstripMenu* newSubMenu=new GuiToolstripMenu(subMenuStyleController?subMenuStyleController:styleController->CreateSubMenuStyleController(), this);
					SetSubMenu(newSubMenu, true);
				}
			}
		}
	}
}