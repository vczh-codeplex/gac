/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Xml Document

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_XMLDOCUMENT
#define GACUI_TOOLS_XMLDOCUMENT

#include "TextDocument.h"

namespace vl
{
	namespace gactools
	{
		class XmlTextView : public PlainTextView
		{
		public:
			static const wchar_t*				ViewTypeId;

			XmlTextView(DocumentFragment* _ownedFragment);
			~XmlTextView();
		};

		class XmlTextDocument : public TextDocument
		{
		public:
			XmlTextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath=L"", DocumentFragment* _ownedFragment=0, const WString& _friendlyName=L"Xml Document");
			~XmlTextDocument();
		};

		class XmlTextFileType : public DocumentFileType
		{
		public:
			static const wchar_t*				FileTypeId;
		protected:
			
			Ptr<IDocumentFragment>				NewDocumentFragment(IDocumentContainer* container)override;
			Ptr<IDocumentFragment>				LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)override;
		public:
			XmlTextFileType();
			~XmlTextFileType();
		};
	}
}

#endif