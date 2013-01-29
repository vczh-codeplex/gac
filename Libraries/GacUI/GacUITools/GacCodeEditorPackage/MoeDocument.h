/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Moe Document

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_MOEDOCUMENT
#define GACUI_TOOLS_MOEDOCUMENT

#include "TextDocument.h"

namespace vl
{
	namespace gactools
	{
		class MoeTextView : public PlainTextView
		{
		public:
			static const wchar_t*				ViewTypeId;

			MoeTextView(DocumentFragment* _ownedFragment, const WString& viewTypeId=ViewTypeId, const WString& friendlyName=L"Moe Text View");
			~MoeTextView();
		};

		class MoeTextDocument : public TextDocument
		{
		public:
			MoeTextDocument(IDocumentFileType* _ownedFileType, IDocumentContainer* _ownedContainer, const WString& _filePath=L"", DocumentFragment* _ownedFragment=0, const WString& _friendlyName=L"Moe Document");
			~MoeTextDocument();
		};

		class MoeTextFileType : public DocumentFileType
		{
		public:
			static const wchar_t*				FileTypeId;
		protected:
			
			Ptr<IDocumentFragment>				NewDocumentFragment(IDocumentContainer* container)override;
			Ptr<IDocumentFragment>				LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)override;
		public:
			MoeTextFileType();
			~MoeTextFileType();
		};
	}
}

#endif