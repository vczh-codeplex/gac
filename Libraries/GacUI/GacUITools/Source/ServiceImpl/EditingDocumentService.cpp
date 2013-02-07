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
			if(!fileType)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"File type \""+fileTypeId+L"\" is not registered.", 0);
			}

			Ptr<IDocumentContainer> document=fileType->CreateDocument();
			if(!document)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to create an empty document using file type \""+fileTypeId+L"\".", 0);
			}
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
			if(selectedEditorTypeId==L"")
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Default editor for view type \""+view->GetViewTypeId()+L"\" is not registered.", 0);
			}
			IDocumentEditorFactory* editorFactory=GetDocumentManager()->FindEditorFactoryById(selectedEditorTypeId);
			if(!editorFactory)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Editor factory for editor type \""+selectedEditorTypeId+L"\" is not registered.", 0);
			}
			if(!editorFactory->IsSupportedViewTypeId(view->GetViewTypeId()))
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Editor factory of editor type \""+selectedEditorTypeId+L"\" does not support view type \""+view->GetViewTypeId()+L"\".", 0);
			}

			Ptr<IDocumentEditor> editor=editorFactory->CreateEditor(view);
			if(!editor)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to create an editor of editor type \""+selectedEditorTypeId+L"\" for view type \""+view->GetViewTypeId()+L"\".", 0);
			}
			editor->AttachCallback(this);
			if(!InstallEditor(editor.Obj()))
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to show the created editor.", 0);
			}
			activeEditors.Add(editor);
			return editor.Obj();
		}

		IDocumentEditor* EditingDocumentService::LoadDocumentFromContainer(Ptr<IDocumentContainer> document, const WString& editorTypeId, bool promptDialog)
		{
			if(!CanInstallNewEditor(promptDialog)) return 0;
			IDocumentFragment* fragment=document->GetRootFragment();
			if(!fragment)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to get the root document fragment from the document container.", 0);
			}
			IDocumentView* view=fragment->GetView(fragment->GetDefaultViewTypeId());
			if(!view)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to get the default view from the document fragment.", 0);
			}

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
				SHOW_ERROR_AND_RETURN(promptDialog, L"Cannot determine the file type for file \""+filePath+L"\".", 0);
			}
			else
			{
				WString extension=filePath.Sub(index, filePath.Length()-index);
				IDocumentFileType* fileType=GetDocumentManager()->FindFileTypeByExtension(extension);
				if(!fileType)
				{
					SHOW_ERROR_AND_RETURN(promptDialog, L"Cannot determine the file type for file extension \""+extension+L"\".", 0);
				}

				Ptr<IDocumentContainer> document=fileType->CreateDocumentFromFile(filePath);
				if(!document)
				{
					SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to load a file using file type \""+fileType->GetFileTypeId()+L"\".", 0);
				}
				IDocumentFragment* fragment=document->GetRootFragment();
				if(!fragment)
				{
					SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to get the root document fragment from the document container.", 0);
				}
				if(!document->GetRootFragment()->ReloadDocument())
				{
					SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to read the file.", 0);
				}
				return LoadDocumentFromContainer(document, editorTypeId, promptDialog);
			}
		}

		IDocumentEditor* EditingDocumentService::LoadDocumentByDialog(const WString& dialogId, const WString& editorTypeId, bool promptDialog)
		{
			if(!CanInstallNewEditor(promptDialog)) return 0;
			IFileDialogService* dialogService=GetDocumentManager()->GetService<IFileDialogService>();
			if(!dialogService)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Cannot find IFileDialogService.", 0);
			}
			WString filePath=dialogService->OpenDialogForSingleFile(dialogId);
			if(filePath==L"") return 0;
			return LoadDocumentFromFile(filePath, editorTypeId, promptDialog);
		}

		bool EditingDocumentService::SaveDocumentByDialog(IDocumentEditor* editor, const WString& dialogId, bool promptDialog)
		{
			IDocumentView* view=editor->GetEditingView();
			if(!view)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"The current document editor doesn't bind to a document view.", false);
			}
			IDocumentFragment* fragment=view->GetOwnedFragment();
			if(!fragment)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Cannot get the document fragment from the editing document view.", false);
			}
			if(!fragment->CanSaveSeparately() || !fragment->CanSaveToAnotherFile())
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"The current document cannot be saved.", false);
			}

			IFileDialogService* dialogService=GetDocumentManager()->GetService<IFileDialogService>();
			if(!dialogService)
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Cannot find IFileDialogService.", false);
			}
			WString filePath=dialogService->SaveDialogForSingleFile(dialogId, fragment->GetOwnedFileType()->GetFileExtension());
			if(filePath==L"") return false;

			if(!fragment->SaveDocumentAs(filePath))
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Failed to save the current document.", false);
			}
			return true;
		}

		bool EditingDocumentService::CloseEditor(IDocumentEditor* editor, bool promptDialog)
		{
			if(!CanUninstallEditor(editor, promptDialog)) return false;
			if(!activeEditors.Contains(editor))
			{
				SHOW_ERROR_AND_RETURN(promptDialog, L"Internal error: this editor is not created using IEditingDocumentService.", false);
			}
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