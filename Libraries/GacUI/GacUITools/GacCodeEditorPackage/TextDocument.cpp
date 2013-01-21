#include "TextDocument.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
PlainTextView
***********************************************************************/

		const wchar_t* PlainTextView::ViewTypeId = L"vl::gactools::PlainTextView";

/***********************************************************************
TextDocument
***********************************************************************/

		bool TextDocument::LoadDocumentInternal(const WString& filePath)
		{
			return false;
		}

		bool TextDocument::SaveDocumentInternal(const WString& filePath)
		{
			return false;
		}

		TextDocument::TextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:FileDocumentFragment(_ownedContainer, _ownedFragment, _friendlyName, _filePath)
		{
		}

		TextDocument::~TextDocument()
		{
		}

/***********************************************************************
TextFileType
***********************************************************************/

		Ptr<IDocumentFragment> TextFileType::NewDocumentFragment(IDocumentContainer* container)
		{
			return new TextDocument(container);
		}

		Ptr<IDocumentFragment> TextFileType::LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)
		{
			return new TextDocument(container, filePath);
		}

		TextFileType::TextFileType()
			:DocumentFileType(L".txt", FileTypeId, L"Text Document")
		{
		}

		TextFileType::~TextFileType()
		{
		}

		const wchar_t* TextFileType::FileTypeId = L"vl::gactools::TextFileType";
	}
}