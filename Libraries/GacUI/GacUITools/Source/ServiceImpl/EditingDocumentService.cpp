#include "EditingDocumentService.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
EditingDocumentService
***********************************************************************/

		EditingDocumentService::EditingDocumentService()
		{
		}

		EditingDocumentService::~EditingDocumentService()
		{
		}

		bool EditingDocumentService::NewDocument(const WString& fileTypeId, const WString& editorTypeId)
		{
			throw 0;
		}

		bool EditingDocumentService::LoadDocumentFromFile(const WString& filePath, const WString& editorTypeId)
		{
			throw 0;
		}

		bool EditingDocumentService::LoadDocumentFromView(IDocumentView* view, const WString& editorTypeId)
		{
			throw 0;
		}

		bool EditingDocumentService::LoadDocumentByDialog(const WString& dialogId, const WString& editorTypeId)
		{
			throw 0;
		}

		vint EditingDocumentService::GetActiveEditorCount()
		{
			throw 0;
		}

		IDocumentEditor* EditingDocumentService::GetActiveEditor(vint index)
		{
			throw 0;
		}

		bool EditingDocumentService::SaveDocumentByDialog(IDocumentEditor* editor, const WString& dialogId)
		{
			throw 0;
		}

		bool EditingDocumentService::CloseEditor(IDocumentEditor* editor)
		{
			throw 0;
		}
	}
}