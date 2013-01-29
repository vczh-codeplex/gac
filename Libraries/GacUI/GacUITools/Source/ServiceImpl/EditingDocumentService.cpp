#include "EditingDocumentService.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
EditingDocumentService
***********************************************************************/

		void EditingDocumentService::OnAttach(IDocumentEditor* sender)
		{
		}

		void EditingDocumentService::OnDetach(IDocumentEditor* sender)
		{
		}

		void EditingDocumentService::OnEditorUpdated(IDocumentEditor* sender)
		{
		}

		void EditingDocumentService::OnEditorDestroyed(IDocumentEditor* sender)
		{
		}

		void EditingDocumentService::OnFinishEdit(IDocumentEditor* sender)
		{
			UninstallEditor(sender);
		}

		void EditingDocumentService::OnLostActiveView(IDocumentEditor* sender)
		{
			UninstallEditor(sender);
		}

		EditingDocumentService::EditingDocumentService()
		{
		}

		EditingDocumentService::~EditingDocumentService()
		{
		}

		IDocumentEditor* EditingDocumentService::NewDocument(const WString& fileTypeId, const WString& editorTypeId, bool promptDialog)
		{
			if(!CanInstallNewEditor(promptDialog)) return 0;
			IDocumentFileType* fileType=GetDocumentManager()->FindFileTypeById(fileTypeId);
			if(!fileType) return 0;

			Ptr<IDocumentContainer> document=fileType->CreateDocument();
			if(!document) return 0;
			return LoadDocumentFromContainer(document, editorTypeId, promptDialog);
		}

		IDocumentEditor* EditingDocumentService::LoadDocumentFromView(IDocumentView* view, const WString& editorTypeId, bool promptDialog)
		{
			if(!CanInstallNewEditor(promptDialog)) return 0;
			WString selectedEditorTypeId=editorTypeId;
			if(selectedEditorTypeId==L"")
			{
				selectedEditorTypeId=GetDocumentManager()->GetDefaultEditorTypeId(view->GetViewTypeId());
			}
			if(selectedEditorTypeId==L"") return 0;
			IDocumentEditorFactory* editorFactory=GetDocumentManager()->FindEditorFactoryById(selectedEditorTypeId);
			if(!editorFactory) return 0;
			if(!editorFactory->IsSupportedViewTypeId(view->GetViewTypeId())) return 0;

			Ptr<IDocumentEditor> editor=editorFactory->CreateEditor(view);
			if(!editor) return 0;
			editor->AttachCallback(this);
			if(!InstallEditor(editor.Obj())) return 0;

			activeEditors.Add(editor);
			return editor.Obj();
		}

		IDocumentEditor* EditingDocumentService::LoadDocumentFromContainer(Ptr<IDocumentContainer> document, const WString& editorTypeId, bool promptDialog)
		{
			if(!CanInstallNewEditor(promptDialog)) return 0;
			IDocumentFragment* fragment=document->GetRootFragment();
			if(!fragment) return 0;
			IDocumentView* view=fragment->GetView(fragment->GetDefaultViewTypeId());
			if(!view) return 0;

			IDocumentEditor* editor=LoadDocumentFromView(view, editorTypeId, promptDialog);
			if(editor)
			{
				activeDocuments.Add(document);
			}
			return editor;
		}

		IDocumentEditor* EditingDocumentService::LoadDocumentFromFile(const WString& filePath, const WString& editorTypeId, bool promptDialog)
		{
			if(!CanInstallNewEditor(promptDialog)) return 0;
			vint index=Locale::Invariant().FindLast(filePath, L".", Locale::None).key;
			if(index==-1)
			{
				return 0;
			}
			else
			{
				WString extension=filePath.Sub(index, filePath.Length()-index);
				IDocumentFileType* fileType=GetDocumentManager()->FindFileTypeByExtension(extension);
				if(!fileType) return 0;

				Ptr<IDocumentContainer> document=fileType->CreateDocumentFromFile(filePath);
				if(!document) return 0;
				IDocumentFragment* fragment=document->GetRootFragment();
				if(!fragment) return 0;
				if(!fragment->ReloadDocument()) return 0;
				return LoadDocumentFromContainer(document, editorTypeId, promptDialog);
			}
		}

		IDocumentEditor* EditingDocumentService::LoadDocumentByDialog(const WString& dialogId, const WString& editorTypeId, bool promptDialog)
		{
			if(!CanInstallNewEditor(promptDialog)) return 0;
			IFileDialogService* dialogService=GetDocumentManager()->GetService<IFileDialogService>();
			if(!dialogService) return 0;
			WString filePath=dialogService->OpenDialogForSingleFile(dialogId);
			if(filePath==L"") return 0;
			return LoadDocumentFromFile(filePath, editorTypeId, promptDialog);
		}

		bool EditingDocumentService::SaveDocumentByDialog(IDocumentEditor* editor, const WString& dialogId, bool promptDialog)
		{
			IDocumentView* view=editor->GetEditingView();
			if(!view) return false;
			IDocumentFragment* fragment=view->GetOwnedFragment();
			if(!fragment) return false;
			if(!fragment->CanSaveSeparately()) return false;
			if(!fragment->CanSaveToAnotherFile()) return false;

			IFileDialogService* dialogService=GetDocumentManager()->GetService<IFileDialogService>();
			if(!dialogService) return false;
			WString filePath=dialogService->SaveDialogForSingleFile(dialogId, fragment->GetOwnedContainer()->GetOwnedFileType()->GetFileExtension());
			if(filePath==L"") return false;

			return fragment->SaveDocumentAs(filePath);
		}

		bool EditingDocumentService::CloseEditor(IDocumentEditor* editor, bool promptDialog)
		{
			if(!CanUninstallEditor(editor, promptDialog)) return false;
			if(!activeEditors.Contains(editor)) return false;
			IDocumentView* view=editor->GetEditingView();
			editor->FinishEdit();
			activeEditors.Remove(editor);
			activeDocuments.Remove(view->GetOwnedFragment()->GetOwnedContainer());
			return true;
		}

		vint EditingDocumentService::GetActiveEditorCount()
		{
			return activeEditors.Count();
		}

		IDocumentEditor* EditingDocumentService::GetActiveEditor(vint index)
		{
			return activeEditors[index].Obj();
		}

		vint EditingDocumentService::GetActiveDocumentCount()
		{
			return activeDocuments.Count();
		}

		IDocumentContainer* EditingDocumentService::GetActiveDocument(vint index)
		{
			return activeDocuments[index].Obj();
		}
	}
}