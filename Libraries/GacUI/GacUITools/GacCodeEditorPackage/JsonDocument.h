/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Json Document

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_JSONDOCUMENT
#define GACUI_TOOLS_JSONDOCUMENT

#include "TextDocument.h"

namespace vl
{
	namespace gactools
	{
		class JsonTextView : public PlainTextView
		{
		public:
			static const wchar_t*				ViewTypeId;

			JsonTextView(DocumentFragment* _ownedFragment, const WString& viewTypeId=ViewTypeId, const WString& friendlyName=L"Json Text View");
			~JsonTextView();
		};

		class JsonTextDocument : public TextDocument
		{
		public:
			JsonTextDocument(IDocumentFileType* _ownedFileType, IDocumentContainer* _ownedContainer, const WString& _filePath=L"", DocumentFragment* _ownedFragment=0, const WString& _friendlyName=L"Json Document");
			~JsonTextDocument();
		};

		class JsonTextFileType : public DocumentFileType
		{
		public:
			static const wchar_t*				FileTypeId;
		protected:
			
			Ptr<IDocumentFragment>				NewDocumentFragment(IDocumentContainer* container)override;
			Ptr<IDocumentFragment>				LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)override;
		public:
			JsonTextFileType();
			~JsonTextFileType();
		};
	}
}

#endif