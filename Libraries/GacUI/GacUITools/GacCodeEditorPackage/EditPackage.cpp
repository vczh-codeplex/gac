#include "EditPackage.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
EditPackage
***********************************************************************/

		const wchar_t* IEditorSelectionOperation::OperationTypeId = L"vl::gactools::IEditorSelectionOperation";

		WString IEditorSelectionOperation::GetOperationTypeId()
		{
			return OperationTypeId;
		}

/***********************************************************************
EditPackage
***********************************************************************/

		void EditPackage::EditUndo(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(currentEditor)
			{
				IEditorSelectionOperation* operation=currentEditor->GetOperation<IEditorSelectionOperation>();
				if(operation)
				{
					if(operation->CanUndo())
					{
						operation->PerformUndo();
					}
					else
					{
						SHOW_ERROR_AND_RETURN(true, L"Internal error: IEditorSelectionOperation::CanUndo returns false.");
					}
				}
			}
		}

		void EditPackage::EditRedo(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(currentEditor)
			{
				IEditorSelectionOperation* operation=currentEditor->GetOperation<IEditorSelectionOperation>();
				if(operation)
				{
					if(operation->CanRedo())
					{
						operation->PerformRedo();
					}
					else
					{
						SHOW_ERROR_AND_RETURN(true, L"Internal error: IEditorSelectionOperation::CanRedo returns false.");
					}
				}
			}
		}

		void EditPackage::EditCut(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(currentEditor)
			{
				IEditorSelectionOperation* operation=currentEditor->GetOperation<IEditorSelectionOperation>();
				if(operation)
				{
					if(operation->CanCut())
					{
						operation->PerformCut();
					}
					else
					{
						SHOW_ERROR_AND_RETURN(true, L"Internal error: IEditorSelectionOperation::CanCut returns false.");
					}
				}
			}
		}

		void EditPackage::EditCopy(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(currentEditor)
			{
				IEditorSelectionOperation* operation=currentEditor->GetOperation<IEditorSelectionOperation>();
				if(operation)
				{
					if(operation->CanCopy())
					{
						operation->PerformCopy();
					}
					else
					{
						SHOW_ERROR_AND_RETURN(true, L"Internal error: IEditorSelectionOperation::CanCopy returns false.");
					}
				}
			}
		}

		void EditPackage::EditPaste(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(currentEditor)
			{
				IEditorSelectionOperation* operation=currentEditor->GetOperation<IEditorSelectionOperation>();
				if(operation)
				{
					if(operation->CanPaste())
					{
						operation->PerformPaste();
					}
					else
					{
						SHOW_ERROR_AND_RETURN(true, L"Internal error: IEditorSelectionOperation::CanPaste returns false.");
					}
				}
			}
		}

		void EditPackage::EditDelete(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(currentEditor)
			{
				IEditorSelectionOperation* operation=currentEditor->GetOperation<IEditorSelectionOperation>();
				if(operation)
				{
					if(operation->CanDelete())
					{
						operation->PerformDelete();
					}
					else
					{
						SHOW_ERROR_AND_RETURN(true, L"Internal error: IEditorSelectionOperation::CanDelete returns false.");
					}
				}
			}
		}

		void EditPackage::EditSelectAll(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(currentEditor)
			{
				IEditorSelectionOperation* operation=currentEditor->GetOperation<IEditorSelectionOperation>();
				if(operation)
				{
					if(operation->CanSelectAll())
					{
						operation->PerformSelectAll();
					}
					else
					{
						SHOW_ERROR_AND_RETURN(true, L"Internal error: IEditorSelectionOperation::CanSelectAll returns false.");
					}
				}
			}
		}

		void EditPackage::OnStateUpdated(IEditorSelectionOperation* sender)
		{
			if(sender)
			{
				if(sender->GetOwnedEditor()==currentEditor)
				{
					commandEditUndo->SetEnabled(sender->CanUndo());
					commandEditRedo->SetEnabled(sender->CanRedo());
					commandEditCut->SetEnabled(sender->CanCut());
					commandEditCopy->SetEnabled(sender->CanCopy());
					commandEditPaste->SetEnabled(sender->CanPaste());
					commandEditDelete->SetEnabled(sender->CanDelete());
					commandEditSelectAll->SetEnabled(sender->CanSelectAll());
				}
			}
			else
			{
				commandEditUndo->SetEnabled(false);
				commandEditRedo->SetEnabled(false);
				commandEditCut->SetEnabled(false);
				commandEditCopy->SetEnabled(false);
				commandEditPaste->SetEnabled(false);
				commandEditDelete->SetEnabled(false);
				commandEditSelectAll->SetEnabled(false);
			}
		}

		const wchar_t* EditPackage::PackageId = L"EditPackage";

		EditPackage::EditPackage()
			:commandEditUndo(0)
			,commandEditRedo(0)
			,commandEditCut(0)
			,commandEditCopy(0)
			,commandEditPaste(0)
			,commandEditDelete(0)
			,commandEditSelectAll(0)
			,currentEditor(0)
		{
		}

		WString EditPackage::GetPackageId()
		{
			return PackageId;
		}

		void EditPackage::OnCurrentEditorUpdated(IDocumentEditor* editor)
		{
			currentEditor=editor;
			if(currentEditor)
			{
				IEditorSelectionOperation* operation=currentEditor->GetOperation<IEditorSelectionOperation>();
				if(operation && !operation->IsInitialized())
				{
					operation->Initialize(this);
				}
				OnStateUpdated(operation);
			}
			else
			{
				OnStateUpdated(0);
			}
		}

		void EditPackage::OnInstallCommand(DocumentToolstripCommand* command)
		{
			if(command->GetCommandId()==L"Edit.Undo")
			{
				commandEditUndo=command;
				commandEditUndo->SetEnabled(false);
				commandEditUndo->Executed.AttachMethod(this, &EditPackage::EditUndo);
			}
			else if(command->GetCommandId()==L"Edit.Redo")
			{
				commandEditRedo=command;
				commandEditRedo->SetEnabled(false);
				commandEditRedo->Executed.AttachMethod(this, &EditPackage::EditRedo);
			}
			else if(command->GetCommandId()==L"Edit.Cut")
			{
				commandEditCut=command;
				commandEditCut->SetEnabled(false);
				commandEditCut->Executed.AttachMethod(this, &EditPackage::EditCut);
			}
			else if(command->GetCommandId()==L"Edit.Copy")
			{
				commandEditCopy=command;
				commandEditCopy->SetEnabled(false);
				commandEditCopy->Executed.AttachMethod(this, &EditPackage::EditCopy);
			}
			else if(command->GetCommandId()==L"Edit.Paste")
			{
				commandEditPaste=command;
				commandEditPaste->SetEnabled(false);
				commandEditPaste->Executed.AttachMethod(this, &EditPackage::EditPaste);
			}
			else if(command->GetCommandId()==L"Edit.Delete")
			{
				commandEditDelete=command;
				commandEditDelete->SetEnabled(false);
				commandEditDelete->Executed.AttachMethod(this, &EditPackage::EditDelete);
			}
			else if(command->GetCommandId()==L"Edit.SelectAll")
			{
				commandEditSelectAll=command;
				commandEditSelectAll->SetEnabled(false);
				commandEditSelectAll->Executed.AttachMethod(this, &EditPackage::EditSelectAll);
			}
		}
	}
}