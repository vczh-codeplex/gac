#include "SDI.h"
#include "Implementation\DocumentManager.h"
#include "PackageManager.h"
#include "ServiceImpl\FileDialogService.h"
#include "ServiceImpl\EditingDocumentService.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

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
			GuiWindow*								mainWindow;
			IEditingDocumentService*				editingDocumentService;

			Ptr<GuiResource> LoadPackageResource()override
			{
				return sdiApplication->GetApplicationResource();
			}

			void SDIOpenDocument(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				editingDocumentService->LoadDocumentByDialog(L"Dialog.OpenFile", L"", true);
			}

			void SDISaveDocumentInternal(bool forceToSaveSeparately)
			{
				if(editingDocumentService->GetActiveEditorCount()>0)
				{
					IDocumentEditor* editor=editingDocumentService->GetActiveEditor(0);
					if(!editor) goto FAILED_TO_SAVE;
					IDocumentView* view=editor->GetEditingView();
					if(!view) goto FAILED_TO_SAVE;
					IDocumentFragment* fragment=view->GetOwnedFragment();
					if(!fragment) goto FAILED_TO_SAVE;
					if(!fragment->CanSaveSeparately()) goto FAILED_TO_SAVE;
					if(forceToSaveSeparately || fragment->GetFilePath()==L"")
					{
						if(!fragment->CanSaveToAnotherFile()) goto FAILED_TO_SAVE;
						editingDocumentService->SaveDocumentByDialog(editor, L"Dialog.SaveFile", true);
					}
					else
					{
						if(!fragment->SaveDocument()) goto FAILED_TO_SAVE;
					}
					return;
				}
			FAILED_TO_SAVE:
				GetCurrentController()->DialogService()->ShowMessageBox(
					mainWindow->GetNativeWindow(),
					L"Failed to save the current document.",
					mainWindow->GetText()
					);
			}

			void SDISaveDocument(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				SDISaveDocumentInternal(false);
			}

			void SDISaveDocumentAs(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				SDISaveDocumentInternal(true);
			}

			void SDIExitApplication(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				mainWindow->Close();
			}
		public:
			static const wchar_t*					PackageId;

			SDIApplicationPackage()
				:mainWindow(0)
				,editingDocumentService(0)
			{
			}

			~SDIApplicationPackage()
			{
			}

			void SetMainWindow(GuiWindow* window)
			{
				mainWindow=window;
			}

			WString GetPackageId()override
			{
				return PackageId;
			}

			void AfterInitialization()override
			{
				MainApplicationPackage::AfterInitialization();
				editingDocumentService=GetDocumentManager()->GetService<IEditingDocumentService>();
			}

			void InstallToolstripCommand(DocumentToolstripCommand* command)override
			{
				if(command->GetCommandId()==L"SDI.OpenDocument")
				{
					command->Executed.AttachMethod(this, &SDIApplicationPackage::SDIOpenDocument);
				}
				else if(command->GetCommandId()==L"SDI.SaveDocument")
				{
					command->Executed.AttachMethod(this, &SDIApplicationPackage::SDISaveDocument);
				}
				else if(command->GetCommandId()==L"SDI.SaveDocumentAs")
				{
					command->Executed.AttachMethod(this, &SDIApplicationPackage::SDISaveDocumentAs);
				}
				else if(command->GetCommandId()==L"SDI.ExitApplication")
				{
					command->Executed.AttachMethod(this, &SDIApplicationPackage::SDIExitApplication);
				}
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
			class SDIEditingDocumentService : public EditingDocumentService, public IDocumentFragment::ICallback
			{
			private:
				void OnAttach(IDocumentFragment* sender)override
				{
				}

				void OnDetach(IDocumentFragment* sender)override
				{
				}

				void OnFilePathUpdated(IDocumentFragment* sender)override
				{
					if(GetActiveDocumentCount()>0 && GetActiveDocument(0)==sender->GetOwnedContainer())
					{
						window->DisplayWindowTitle(sender->GetFilePath());
					}
				}

				void OnFragmentUpdated(IDocumentFragment* sender)override
				{
				}

				void OnFragmentDestroyed(IDocumentFragment* sender)override
				{
				}
			protected:
				MainWindow*										window;

				bool CanInstallNewEditor(bool promptDialog)override
				{
					return true;
				}

				bool CanUninstallEditor(IDocumentEditor* editor, bool promptDialog)
				{
					return true;
				}

				bool InstallEditor(IDocumentEditor* editor)override
				{
					editor->GetEditingView()->GetOwnedFragment()->AttachCallback(this);
					editor->GetEditorControl()->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					window->editorControlContainer->AddChild(editor->GetEditorControl()->GetBoundsComposition());
					window->DisplayWindowTitle(editor->GetEditingView()->GetOwnedFragment()->GetFilePath());
					return true;
				}

				bool UninstallEditor(IDocumentEditor* editor)override
				{
					window->editorControlContainer->RemoveChild(editor->GetEditorControl()->GetBoundsComposition());
					window->DisplayWindowTitle(L"");
					editor->GetEditingView()->GetOwnedFragment()->DetachCallback(this);
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
			WString												appName;

			GuiToolstripMenuBar*								mainMenu;
			GuiToolstripToolbar*								mainToolbar;
			GuiGraphicsComposition*								editorControlContainer;

			Ptr<SDIEditingDocumentService>						editingDocumentService;

			void InitializeSDIAppplication()
			{
				List<Ptr<XmlElement>> packages;
				EnumeratePackages(resource, packages);
				LoadLegalDocumentPackages(resource, packages);
				GetDocumentManager()->RunPackageBeforeInitialization();
				
				editingDocumentService=new SDIEditingDocumentService(this);
				GetDocumentManager()->RegisterService(editingDocumentService);
				appPackage=GetDocumentManager()->GetPackage<SDIApplicationPackage>();

				if(appPackage)
				{
					appPackage->SetMainWindow(this);
					appPackage->BuildApplicationMenu(mainMenu, L"Menu");
					appPackage->BuildApplicationToolbar(mainToolbar, L"Toolbar");
				}
				GetDocumentManager()->RunPackageAfterInitialization();
			}

			void DisplayWindowTitle(const WString& filePath)
			{
				if(filePath==L"")
				{
					this->SetText(appName+L" (www.gaclib.net)");
				}
				else
				{
					this->SetText(appName+L" ["+GetFileName(filePath)+L"] (www.gaclib.net)");
				}
			}

			void MainWindow_Closing(GuiGraphicsComposition* sender, GuiRequestEventArgs& arguments)
			{
				while(editingDocumentService->GetActiveEditorCount()>0)
				{
					if(!editingDocumentService->CloseEditor(editingDocumentService->GetActiveEditor(0), true))
					{
						arguments.cancel=true;
						return;
					}
				}
			}
		public:
			MainWindow()
				:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
				,appName(L"GacUI Tools")
			{
				resource=sdiApplication->GetApplicationResource();
				if(auto name=resource->GetValueByPath(L"Application\\Name").Cast<ObjectBox<WString>>())
				{
					appName=name->Unbox();
				}
				DisplayWindowTitle(L"");

				this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
				this->ForceCalculateSizeImmediately();
				this->MoveToScreenCenter();
				this->WindowClosing.AttachMethod(this, &MainWindow::MainWindow_Closing);

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