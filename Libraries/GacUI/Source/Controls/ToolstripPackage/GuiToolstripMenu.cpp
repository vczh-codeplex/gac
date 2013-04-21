#include "GuiToolstripMenu.h"
#include "..\Styles\GuiThemeStyleFactory.h"

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

			bool GuiToolstripCollection::RemoveAtInternal(vint index, GuiControl* const& control)
			{
				GuiStackItemComposition* stackItem=stackComposition->GetStackItems().Get(index);

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
				items.RemoveAt(index);
				InvokeUpdateLayout();
				return true;
			}

			bool GuiToolstripCollection::InsertInternal(vint index, GuiControl* const& control)
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
				return true;
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

/***********************************************************************
GuiToolstripBuilder
***********************************************************************/

			GuiToolstripBuilder::GuiToolstripBuilder(Environment _environment, GuiToolstripCollection* _toolstripItems)
				:environment(_environment)
				,toolstripItems(_toolstripItems)
				,previousBuilder(0)
				,theme(0)
				,lastCreatedButton(0)
			{
			}

			GuiToolstripBuilder::~GuiToolstripBuilder()
			{
			}

			GuiToolstripBuilder* GuiToolstripBuilder::Button(Ptr<GuiImageData> image, const WString& text, GuiToolstripButton** result)
			{
				lastCreatedButton=0;
				switch(environment)
				{
				case Menu:
					lastCreatedButton=new GuiToolstripButton(theme->CreateMenuItemButtonStyle());
					break;
				case MenuBar:
					lastCreatedButton=new GuiToolstripButton(theme->CreateMenuBarButtonStyle());
					break;
				case Toolbar:
					lastCreatedButton=new GuiToolstripButton(theme->CreateToolbarButtonStyle());
					break;
				}
				if(lastCreatedButton)
				{
					lastCreatedButton->SetImage(image);
					lastCreatedButton->SetText(text);
					if(result)
					{
						*result=lastCreatedButton;
					}
					toolstripItems->Add(lastCreatedButton);
				}
				return this;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::Button(GuiToolstripCommand* command, GuiToolstripButton** result)
			{
				lastCreatedButton=0;
				switch(environment)
				{
				case Menu:
					lastCreatedButton=new GuiToolstripButton(theme->CreateMenuItemButtonStyle());
					break;
				case MenuBar:
					lastCreatedButton=new GuiToolstripButton(theme->CreateMenuBarButtonStyle());
					break;
				case Toolbar:
					lastCreatedButton=new GuiToolstripButton(theme->CreateToolbarButtonStyle());
					break;
				}
				if(lastCreatedButton)
				{
					lastCreatedButton->SetCommand(command);
					if(result)
					{
						*result=lastCreatedButton;
					}
					toolstripItems->Add(lastCreatedButton);
				}
				return this;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::DropdownButton(Ptr<GuiImageData> image, const WString& text, GuiToolstripButton** result)
			{
				lastCreatedButton=0;
				switch(environment)
				{
				case Toolbar:
					lastCreatedButton=new GuiToolstripButton(theme->CreateToolbarDropdownButtonStyle());
					break;
				}
				if(lastCreatedButton)
				{
					lastCreatedButton->SetImage(image);
					lastCreatedButton->SetText(text);
					if(result)
					{
						*result=lastCreatedButton;
					}
					toolstripItems->Add(lastCreatedButton);
				}
				return this;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::DropdownButton(GuiToolstripCommand* command, GuiToolstripButton** result)
			{
				lastCreatedButton=0;
				switch(environment)
				{
				case Toolbar:
					lastCreatedButton=new GuiToolstripButton(theme->CreateToolbarDropdownButtonStyle());
					break;
				}
				if(lastCreatedButton)
				{
					lastCreatedButton->SetCommand(command);
					if(result)
					{
						*result=lastCreatedButton;
					}
					toolstripItems->Add(lastCreatedButton);
				}
				return this;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::SplitButton(Ptr<GuiImageData> image, const WString& text, GuiToolstripButton** result)
			{
				lastCreatedButton=0;
				switch(environment)
				{
				case Toolbar:
					lastCreatedButton=new GuiToolstripButton(theme->CreateToolbarSplitButtonStyle());
					break;
				}
				if(lastCreatedButton)
				{
					lastCreatedButton->SetImage(image);
					lastCreatedButton->SetText(text);
					if(result)
					{
						*result=lastCreatedButton;
					}
					toolstripItems->Add(lastCreatedButton);
				}
				return this;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::SplitButton(GuiToolstripCommand* command, GuiToolstripButton** result)
			{
				lastCreatedButton=0;
				switch(environment)
				{
				case Toolbar:
					lastCreatedButton=new GuiToolstripButton(theme->CreateToolbarSplitButtonStyle());
					break;
				}
				if(lastCreatedButton)
				{
					lastCreatedButton->SetCommand(command);
					if(result)
					{
						*result=lastCreatedButton;
					}
					toolstripItems->Add(lastCreatedButton);
				}
				return this;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::Splitter()
			{
				lastCreatedButton=0;
				switch(environment)
				{
				case Menu:
					toolstripItems->Add(new GuiControl(theme->CreateMenuSplitterStyle()));
					break;
				case Toolbar:
					toolstripItems->Add(new GuiControl(theme->CreateToolbarSplitterStyle()));
					break;
				}
				return this;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::Control(GuiControl* control)
			{
				toolstripItems->Add(control);
				return this;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::BeginSubMenu()
			{
				if(lastCreatedButton)
				{
					lastCreatedButton->CreateToolstripSubMenu();
					GuiToolstripMenu* menu=lastCreatedButton->GetToolstripSubMenu();
					if(menu)
					{
						menu->GetBuilder()->previousBuilder=this;
						return menu->GetBuilder();
					}
				}
				return 0;
			}

			GuiToolstripBuilder* GuiToolstripBuilder::EndSubMenu()
			{
				return previousBuilder;
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
				builder=new GuiToolstripBuilder(GuiToolstripBuilder::Menu, toolstripItems.Obj());
			}

			GuiToolstripMenu::~GuiToolstripMenu()
			{
			}

			GuiToolstripCollection& GuiToolstripMenu::GetToolstripItems()
			{
				return *toolstripItems.Obj();
			}

			GuiToolstripBuilder* GuiToolstripMenu::GetBuilder(theme::ITheme* themeObject)
			{
				builder->theme=themeObject?themeObject:theme::GetCurrentTheme();
				return builder.Obj();
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
				builder=new GuiToolstripBuilder(GuiToolstripBuilder::MenuBar, toolstripItems.Obj());
			}

			GuiToolstripMenuBar::~GuiToolstripMenuBar()
			{
			}

			GuiToolstripCollection& GuiToolstripMenuBar::GetToolstripItems()
			{
				return *toolstripItems.Obj();
			}

			GuiToolstripBuilder* GuiToolstripMenuBar::GetBuilder(theme::ITheme* themeObject)
			{
				builder->theme=themeObject?themeObject:theme::GetCurrentTheme();
				return builder.Obj();
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
				builder=new GuiToolstripBuilder(GuiToolstripBuilder::Toolbar, toolstripItems.Obj());
			}

			GuiToolstripToolbar::~GuiToolstripToolbar()
			{
			}

			GuiToolstripCollection& GuiToolstripToolbar::GetToolstripItems()
			{
				return *toolstripItems.Obj();
			}

			GuiToolstripBuilder* GuiToolstripToolbar::GetBuilder(theme::ITheme* themeObject)
			{
				builder->theme=themeObject?themeObject:theme::GetCurrentTheme();
				return builder.Obj();
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
				,command(0)
			{
				Clicked.AttachMethod(this, &GuiToolstripButton::OnClicked);
			}

			GuiToolstripButton::~GuiToolstripButton()
			{
			}

			GuiToolstripCommand* GuiToolstripButton::GetCommand()
			{
				return command;
			}

			void GuiToolstripButton::SetCommand(GuiToolstripCommand* value)
			{
				if(command!=value)
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