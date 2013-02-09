#include "TextEditorPackage.h"
#include "TextDocument.h"
#include "TextDocumentEditor.h"
#include "XmlDocument.h"
#include "XmlDocumentEditor.h"
#include "JsonDocument.h"
#include "JsonDocumentEditor.h"
#include "MoeDocument.h"
#include "MoeDocumentEditor.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
TextEditorPackage
***********************************************************************/

		const wchar_t* TextEditorPackage::PackageId = L"TextEditorPackage";

		void TextEditorPackage::NewDocument(const WString& fileTypeId)
		{
			if(!editingDocumentService)
			{
				SHOW_ERROR_AND_RETURN(true, L"Cannot not find IEditingDocumentService.");
			}
			if(editingDocumentService->PrepareToAcceptNewEditor(true))
			{
				editingDocumentService->NewDocument(fileTypeId, L"", true);
			}
		}

		void TextEditorPackage::TextEditorNewText(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			NewDocument(TextFileType::FileTypeId);
		}

		void TextEditorPackage::TextEditorNewXml(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			NewDocument(XmlTextFileType::FileTypeId);
		}

		void TextEditorPackage::TextEditorNewJson(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			NewDocument(JsonTextFileType::FileTypeId);
		}

		void TextEditorPackage::TextEditorNewMoe(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			NewDocument(MoeTextFileType::FileTypeId);
		}

		TextEditorPackage::TextEditorPackage()
			:editingDocumentService(0)
		{
		}

		WString TextEditorPackage::GetPackageId()
		{
			return PackageId;
		}

		void TextEditorPackage::OnBeforeInit()
		{
			{
				GetDocumentManager()->RegisterFileType(new TextFileType);
				Ptr<TextDocumentEditorFactory> editorFactory=new TextDocumentEditorFactory;
				GetDocumentManager()->RegisterEditorFactory(editorFactory);
				GetDocumentManager()->BindDefaultEditor(PlainTextView::ViewTypeId, editorFactory.Obj());
			}
			{
				GetDocumentManager()->RegisterFileType(new XmlTextFileType);
				Ptr<XmlTextDocumentEditorFactory> editorFactory=new XmlTextDocumentEditorFactory;
				GetDocumentManager()->RegisterEditorFactory(editorFactory);
				GetDocumentManager()->BindDefaultEditor(XmlTextView::ViewTypeId, editorFactory.Obj());
			}
			{
				GetDocumentManager()->RegisterFileType(new JsonTextFileType);
				Ptr<JsonTextDocumentEditorFactory> editorFactory=new JsonTextDocumentEditorFactory;
				GetDocumentManager()->RegisterEditorFactory(editorFactory);
				GetDocumentManager()->BindDefaultEditor(JsonTextView::ViewTypeId, editorFactory.Obj());
			}
			{
				GetDocumentManager()->RegisterFileType(new MoeTextFileType);
				Ptr<MoeTextDocumentEditorFactory> editorFactory=new MoeTextDocumentEditorFactory;
				GetDocumentManager()->RegisterEditorFactory(editorFactory);
				GetDocumentManager()->BindDefaultEditor(MoeTextView::ViewTypeId, editorFactory.Obj());
			}
		}

		void TextEditorPackage::OnAfterInit()
		{
			editingDocumentService=GetDocumentManager()->GetService<IEditingDocumentService>();
		}

		void TextEditorPackage::OnInstallCommand(DocumentToolstripCommand* command)
		{
			if(command->GetCommandId()==L"TextEditor.NewText")
			{
				command->Executed.AttachMethod(this, &TextEditorPackage::TextEditorNewText);
			}
			else if(command->GetCommandId()==L"TextEditor.NewXml")
			{
				command->Executed.AttachMethod(this, &TextEditorPackage::TextEditorNewXml);
			}
			else if(command->GetCommandId()==L"TextEditor.NewJson")
			{
				command->Executed.AttachMethod(this, &TextEditorPackage::TextEditorNewJson);
			}
			else if(command->GetCommandId()==L"TextEditor.NewMoe")
			{
				command->Executed.AttachMethod(this, &TextEditorPackage::TextEditorNewMoe);
			}
		}
	}
}