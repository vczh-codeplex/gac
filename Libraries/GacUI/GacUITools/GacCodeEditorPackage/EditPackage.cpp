#include "EditPackage.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
EditPackage
***********************************************************************/

		void EditPackage::EditUndo(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void EditPackage::EditRedo(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void EditPackage::EditCut(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void EditPackage::EditCopy(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void EditPackage::EditPaste(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void EditPackage::EditDelete(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void EditPackage::EditSelectAll(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
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
		{
		}

		WString EditPackage::GetPackageId()
		{
			return PackageId;
		}

		void EditPackage::OnCurrentEditorUpdated(IDocumentEditor* editor)
		{
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