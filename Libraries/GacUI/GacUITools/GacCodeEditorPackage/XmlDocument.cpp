#include "XmlDocument.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
XmlTextView
***********************************************************************/

		const wchar_t* XmlTextView::ViewTypeId = L"vl::gactools::XmlTextView";

		XmlTextView::XmlTextView(DocumentFragment* _ownedFragment, const WString& viewTypeId, const WString& friendlyName)
			:PlainTextView(_ownedFragment, viewTypeId, friendlyName)
		{
		}

		XmlTextView::~XmlTextView()
		{
		}

/***********************************************************************
XmlTextDocument
***********************************************************************/

		XmlTextDocument::XmlTextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:TextDocument(_ownedContainer, _filePath, _ownedFragment, _friendlyName)
		{
			Ptr<XmlTextView> xmlTextView=new XmlTextView(this);
			xmlTextView->InstallContentProvider(this);
			AddSupportedView(xmlTextView);
			SetDefaultView(xmlTextView);
		}

		XmlTextDocument::~XmlTextDocument()
		{
		}

/***********************************************************************
XmlTextFileType
***********************************************************************/

		const wchar_t* XmlTextFileType::FileTypeId = L"vl::gactools::XmlTextFileType";

		Ptr<IDocumentFragment> XmlTextFileType::NewDocumentFragment(IDocumentContainer* container)
		{
			return new XmlTextDocument(container);
		}

		Ptr<IDocumentFragment> XmlTextFileType::LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)
		{
			return new XmlTextDocument(container, filePath);
		}

		XmlTextFileType::XmlTextFileType()
			:DocumentFileType(L".xml", FileTypeId, L"Xml Document")
		{
		}

		XmlTextFileType::~XmlTextFileType()
		{
		}
	}
}