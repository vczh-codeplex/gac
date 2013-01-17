#include "SDI.h"
#include "Implementation\DocumentManager.h"
#include "PackageManager.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
SDIConfiguration
***********************************************************************/

		ISDIApplication* sdiApplication=0;

/***********************************************************************
MainWindow
***********************************************************************/

		class MainWindow : public GuiWindow
		{
		protected:
			Ptr<GuiResource>									resource;
			List<Ptr<XmlElement>>								packages;
			Dictionary<WString, Ptr<GuiToolstripCommand>>		commands;

			GuiToolstripMenuBar*								mainMenu;
			GuiToolstripToolbar*								mainToolbar;
		public:
			MainWindow()
				:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			{
				resource=sdiApplication->GetApplicationResource();
				WString applicationName;
				if(auto name=resource->GetValueByPath(L"Application\\Name").Cast<ObjectBox<WString>>())
				{
					applicationName=name->Unbox();
				}

				this->SetText(L"GacUI Tools "+applicationName+L" (www.gaclib.net)");
				this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
				this->ForceCalculateSizeImmediately();
				this->MoveToScreenCenter();

				GuiTableComposition* table=new GuiTableComposition;
				table->SetRowsAndColumns(3, 1);
				table->SetRowOption(0, GuiCellOption::MinSizeOption());
				table->SetRowOption(1, GuiCellOption::MinSizeOption());
				table->SetRowOption(2, GuiCellOption::PercentageOption(1.0));
				table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				table->SetAlignmentToParent(Margin(0, 0, 0, 0));
				this->GetContainerComposition()->AddChild(table);
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(0, 0, 1, 1);

					mainMenu=g::NewMenuBar();
					mainMenu->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					cell->AddChild(mainMenu->GetBoundsComposition());
				}
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(1, 0, 1, 1);

					mainToolbar=g::NewToolbar();
					mainToolbar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					cell->AddChild(mainToolbar->GetBoundsComposition());
				}

				Group<WString, ProprityMenuGroup> existingMenuGroups;
				EnumeratePackages(resource, packages);
				EnumerateCommands(resource, packages, commands);
				EnumerateMenuDefinitions(resource, packages, existingMenuGroups);
				BuildMenu(resource, packages, mainMenu, L"Menu", commands, existingMenuGroups);
				BuildToolbar(resource, packages, mainToolbar, L"Toolbar", commands, existingMenuGroups);
			}

			~MainWindow()
			{
			}
		};

/***********************************************************************
StartSDI
***********************************************************************/

		int StartSDI(ISDIApplication* application)
		{
			sdiApplication=application;
			int result=SetupWindowsDirect2DRenderer();
			sdiApplication=0;
			return result;
		}
	}
}

/***********************************************************************
GuiMain
***********************************************************************/

using namespace vl::gactools;

void GuiMain()
{
	DocumentManager documentManager;
	SetDocumentManager(&documentManager);
	{
		MainWindow window;
		GetApplication()->Run(&window);
	}
	SetDocumentManager(0);
}