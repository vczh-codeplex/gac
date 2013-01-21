/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Text Document

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_TEXTDOCUMENT
#define GACUI_TOOLS_TEXTDOCUMENT

#include "..\Source\Implementation\DocumentView.h"
#include "..\Source\Implementation\DocumentFragment.h"
#include "..\Source\Implementation\DocumentFileType.h"

namespace vl
{
	namespace gactools
	{
		class PlainTextView : public DocumentView
		{
		public:
			static const wchar_t*				ViewTypeId;

			PlainTextView(DocumentFragment* _ownedFragment);
			~PlainTextView();
		};

		class TextDocument : public FileDocumentFragment
		{
		protected:
			
			bool								LoadDocumentInternal(const WString& filePath)override;
			bool								SaveDocumentInternal(const WString& filePath)override;
		public:
			TextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath=L"", DocumentFragment* _ownedFragment=0, const WString& _friendlyName=L"Text Document");
			~TextDocument();
		};

		class TextFileType : public DocumentFileType
		{
		public:
			static const wchar_t*				FileTypeId;
		protected:
			
			Ptr<IDocumentFragment>				NewDocumentFragment(IDocumentContainer* container)override;
			Ptr<IDocumentFragment>				LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)override;
		public:
			TextFileType();
			~TextFileType();
		};
	}
}

#endif