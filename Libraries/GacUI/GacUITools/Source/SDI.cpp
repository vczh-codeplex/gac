#include "SDI.h"
#include "Implementation\DocumentManager.h"
#include "PackageManager.h"
#include "ServiceImpl\FileDialogService.h"
#include "ServiceImpl\EditingDocumentService.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
SDIConfiguration
***********************************************************************/

		ISDIApplication* sdiApplication=0;

/***********************************************************************
SDIApplicationPackage
***********************************************************************/

		class SDIApplicationPackage : public MainApplicationPackage
		{
		protected:

			Ptr<GuiResource> LoadPackageResource()override
			{
				return sdiApplication->GetApplicationResource();
			}
		public:
			static const wchar_t*					PackageId;

			SDIApplicationPackage()
			{
			}

			~SDIApplicationPackage()
			{
			}

			WString GetPackageId()
			{
				return PackageId;
			}
		};
		const wchar_t* SDIApplicationPackage::PackageId = L"SDIApplicationPackage";
		INSTALL_PACKAGE(SDIApplicationPackage);

/***********************************************************************
MainWindow
***********************************************************************/

		class MainWindow : public GuiWindow
		{
		protected:
			class SDIEditingDocumentService : public EditingDocumentService
			{
			protected:
				MainWindow*										window;

				bool CanInstallNewEditor()override
				{
					return true;
				}

				bool InstallEditor(IDocumentEditor* editor)override
				{
					editor->GetEditorControl()->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					window->editorControlContainer->AddChild(editor->GetEditorControl()->GetBoundsComposition());
					return true;
				}

				bool UninstallEditor(IDocumentEditor* editor)override
				{
					window->editorControlContainer->RemoveChild(editor->GetEditorControl()->GetBoundsComposition());
					return true;
				}
			public:
				SDIEditingDocumentService(MainWindow* _window)
					:window(_window)
				{
				}
			};
		protected:
			Ptr<GuiResource>									resource;
			SDIApplicationPackage*								appPackage;

			GuiToolstripMenuBar*								mainMenu;
			GuiToolstripToolbar*								mainToolbar;
			GuiGraphicsComposition*								editorControlContainer;

			Ptr<SDIEditingDocumentService>						editingDocumentService;

			void InitializeSDIAppplication()
			{
				List<Ptr<XmlElement>> packages;
				EnumeratePackages(resource, packages);
				LoadLegalDocumentPackages(resource, packages);
				
				editingDocumentService=new SDIEditingDocumentService(this);
				GetDocumentManager()->RegisterService(editingDocumentService);
				appPackage=GetDocumentManager()->GetPackage<SDIApplicationPackage>();

				if(appPackage)
				{
					appPackage->BuildApplicationMenu(mainMenu, L"Menu");
					appPackage->BuildApplicationToolbar(mainToolbar, L"Toolbar");
				}
			}
		public:
			MainWindow()
				:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			{
				resource=sdiApplication->GetApplicationResource();
				WString applicationName=L"SDI Application";
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
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(2, 0, 1, 1);

					editorControlContainer=cell;
				}

				InitializeSDIAppplication();
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