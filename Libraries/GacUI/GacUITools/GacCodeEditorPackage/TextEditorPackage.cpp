#include "TextEditorPackage.h"
#include "TextDocument.h"
#include "TextDocumentEditor.h"
#include "XmlDocument.h"
#include "XmlDocumentEditor.h"
#include "JsonDocument.h"
#include "JsonDocumentEditor.h"

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
			if(!editingDocumentService) goto FAILED_TO_NEW;
			if(!editingDocumentService->NewDocument(fileTypeId, L"", true)) goto FAILED_TO_NEW;
			return;
		FAILED_TO_NEW:
			INativeWindow* window=GetCurrentController()->WindowService()->GetMainWindow();
			GetCurrentController()->DialogService()->ShowMessageBox(
				window,
				L"Failed to open the document.",
				window->GetTitle()
				);
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
		}
	}
}