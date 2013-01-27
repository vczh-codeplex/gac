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
StartSDI
***********************************************************************/

		ISDIApplication* sdiApplication=0;

		int StartSDI(ISDIApplication* application)
		{
			sdiApplication=application;
			int result=SetupWindowsDirect2DRenderer();
			sdiApplication=0;
			return result;
		}

/***********************************************************************
MainWindow
***********************************************************************/

		class MainWindow : public GuiWindow
		{
		protected:
			Ptr<GuiResource>									resource;
			WString												appName;
			IEditingDocumentService*							editingDocumentService;

			GuiToolstripMenuBar*								mainMenu;
			GuiToolstripToolbar*								mainToolbar;
			GuiGraphicsComposition*								editorControlContainer;

			void InitializeSDIAppplication()
			{
				MainApplicationPackage* appPackage=MainApplicationPackage::BeforeInit(resource, this, L"SDIApplicationPackage");
				if(appPackage)
				{
					appPackage->BuildApplicationMenu(mainMenu, L"Menu");
					appPackage->BuildApplicationToolbar(mainToolbar, L"Toolbar");
				}
				editingDocumentService=GetDocumentManager()->GetService<IEditingDocumentService>();
				MainApplicationPackage::AfterInit();
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
				,editingDocumentService(0)
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

			void InstallEditorControl(GuiControl* control)
			{
				editorControlContainer->AddChild(control->GetBoundsComposition());
			}

			void UninstallEditorControl(GuiControl* control)
			{
				editorControlContainer->RemoveChild(control->GetBoundsComposition());
			}
		};

/***********************************************************************
SDIEditingDocumentService
***********************************************************************/

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
			IDocumentEditor*								currentEditor;

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
				window->InstallEditorControl(editor->GetEditorControl());
				window->DisplayWindowTitle(editor->GetEditingView()->GetOwnedFragment()->GetFilePath());
				currentEditor=editor;
				for(vint i=0;i<GetDocumentManager()->GetPackageCount();i++)
				{
					GetDocumentManager()->GetPackage(i)->OnCurrentEditorUpdated(editor);
				}
				return true;
			}

			bool UninstallEditor(IDocumentEditor* editor)override
			{
				if(currentEditor==editor)
				{
					currentEditor=0;
					for(vint i=0;i<GetDocumentManager()->GetPackageCount();i++)
					{
						GetDocumentManager()->GetPackage(i)->OnCurrentEditorUpdated(0);
					}
				}
				window->UninstallEditorControl(editor->GetEditorControl());
				window->DisplayWindowTitle(L"");
				editor->GetEditingView()->GetOwnedFragment()->DetachCallback(this);
				return true;
			}
		public:
			SDIEditingDocumentService(MainWindow* _window)
				:window(_window)
				,currentEditor(0)
			{
			}

			IDocumentEditor* GetCurrentEditor()override
			{
				return currentEditor;
			}
		};

/***********************************************************************
SDIApplicationPackage
***********************************************************************/

		class SDIApplicationPackage : public MainApplicationPackage
		{
		protected:
			Ptr<SDIEditingDocumentService>			editingDocumentService;

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
				IDocumentEditor* editor=editingDocumentService->GetCurrentEditor();
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
			FAILED_TO_SAVE:
				GetCurrentController()->DialogService()->ShowMessageBox(
					GetMainWindow()->GetNativeWindow(),
					L"Failed to save the current document.",
					GetMainWindow()->GetText()
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
				GetMainWindow()->Close();
			}
		public:
			static const wchar_t*					PackageId;

			SDIApplicationPackage()
			{
			}

			~SDIApplicationPackage()
			{
			}

			WString GetPackageId()override
			{
				return PackageId;
			}

			void OnBeforeInit()override
			{
				MainApplicationPackage::OnBeforeInit();
				editingDocumentService=new SDIEditingDocumentService(dynamic_cast<MainWindow*>(GetMainWindow()));
				GetDocumentManager()->RegisterService(editingDocumentService);
			}

			void OnInstallCommand(DocumentToolstripCommand* command)override
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