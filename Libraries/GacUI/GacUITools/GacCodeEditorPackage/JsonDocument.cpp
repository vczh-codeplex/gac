#include "JsonDocument.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
JsonTextView
***********************************************************************/

		const wchar_t* JsonTextView::ViewTypeId = L"vl::gactools::JsonTextView";

		JsonTextView::JsonTextView(DocumentFragment* _ownedFragment)
			:PlainTextView(_ownedFragment)
		{
		}

		JsonTextView::~JsonTextView()
		{
		}

/***********************************************************************
JsonTextDocument
***********************************************************************/

		JsonTextDocument::JsonTextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:TextDocument(_ownedContainer, _filePath, _ownedFragment, _friendlyName)
		{
			Ptr<JsonTextView> jsonTextView=new JsonTextView(this);
			jsonTextView->InstallContentProvider(this);
			AddSupportedView(jsonTextView);
			SetDefaultView(jsonTextView);
		}

		JsonTextDocument::~JsonTextDocument()
		{
		}

/***********************************************************************
JsonTextFileType
***********************************************************************/

		const wchar_t* JsonTextFileType::FileTypeId = L"vl::gactools::JsonTextFileType";

		Ptr<IDocumentFragment> JsonTextFileType::NewDocumentFragment(IDocumentContainer* container)
		{
			return new JsonTextDocument(container);
		}

		Ptr<IDocumentFragment> JsonTextFileType::LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)
		{
			return new JsonTextDocument(container, filePath);
		}

		JsonTextFileType::JsonTextFileType()
			:DocumentFileType(L".Json", FileTypeId, L"Json Document")
		{
		}

		JsonTextFileType::~JsonTextFileType()
		{
		}
	}
}