#include "TextDocument.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
PlainTextView
***********************************************************************/

		const wchar_t* PlainTextView::ViewTypeId = L"vl::gactools::PlainTextView";

		PlainTextView::PlainTextView(DocumentFragment* _ownedFragment)
			:DocumentView(_ownedFragment, ViewTypeId, L"Plain Text View")
		{
		}

		PlainTextView::~PlainTextView()
		{
		}

/***********************************************************************
TextDocument
***********************************************************************/

		bool TextDocument::LoadDocumentInternal(const WString& filePath)
		{
			return true;
		}

		bool TextDocument::SaveDocumentInternal(const WString& filePath)
		{
			return true;
		}

		TextDocument::TextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:FileDocumentFragment(_ownedContainer, _ownedFragment, _friendlyName, _filePath)
		{
			Ptr<PlainTextView> plainTextView=new PlainTextView(this);
			AddSupportedView(plainTextView);
			SetDefaultView(plainTextView);
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