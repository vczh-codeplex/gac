#include "MoeDocument.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
MoeTextView
***********************************************************************/

		const wchar_t* MoeTextView::ViewTypeId = L"vl::gactools::MoeTextView";

		MoeTextView::MoeTextView(DocumentFragment* _ownedFragment, const WString& viewTypeId, const WString& friendlyName)
			:PlainTextView(_ownedFragment, viewTypeId, friendlyName)
		{
		}

		MoeTextView::~MoeTextView()
		{
		}

/***********************************************************************
MoeTextDocument
***********************************************************************/

		MoeTextDocument::MoeTextDocument(IDocumentFileType* _ownedFileType, IDocumentContainer* _ownedContainer, const WString& _filePath, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:TextDocument(_ownedFileType, _ownedContainer, _filePath, _ownedFragment, _friendlyName)
		{
			Ptr<MoeTextView> moeTextView=new MoeTextView(this);
			moeTextView->InstallContentProvider(this);
			AddSupportedView(moeTextView);
			SetDefaultView(moeTextView);
		}

		MoeTextDocument::~MoeTextDocument()
		{
		}

/***********************************************************************
MoeTextFileType
***********************************************************************/

		const wchar_t* MoeTextFileType::FileTypeId = L"vl::gactools::MoeTextFileType";

		Ptr<IDocumentFragment> MoeTextFileType::NewDocumentFragment(IDocumentContainer* container)
		{
			return new MoeTextDocument(this, container);
		}

		Ptr<IDocumentFragment> MoeTextFileType::LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)
		{
			return new MoeTextDocument(this, container, filePath);
		}

		MoeTextFileType::MoeTextFileType()
			:DocumentFileType(L".Moe", FileTypeId, L"Moe Document")
		{
		}

		MoeTextFileType::~MoeTextFileType()
		{
		}
	}
}