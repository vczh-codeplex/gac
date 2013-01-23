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
		class IPlainTextViewContentProxy : public Interface
		{
		public:
			virtual WString						GetContent()=0;
			virtual void						SetContent(const WString& value)=0;
		};

		class IPlainTextViewContentProvider : public Interface
		{
		public:
			virtual bool						ActivateProxy(IPlainTextViewContentProxy* value)=0;
			virtual bool						DeactivateProxy(IPlainTextViewContentProxy* value)=0;
			virtual WString						GetContent()=0;
			virtual void						SetContent(const WString& value, DocumentView* senderView)=0;
		};

		class PlainTextView : public DocumentView
		{
		protected:
			IPlainTextViewContentProvider*		contentProvider;
		public:
			static const wchar_t*				ViewTypeId;

			PlainTextView(DocumentFragment* _ownedFragment);
			~PlainTextView();

			bool								InstallContentProvider(IPlainTextViewContentProvider* value);
			bool								ActivateProxy(IPlainTextViewContentProxy* value);
			bool								DeactivateProxy(IPlainTextViewContentProxy* value);
			WString								GetContent();
			void								SetContent(const WString& value);
		};

		class TextDocument : public FileDocumentFragment, private IPlainTextViewContentProvider
		{
		private:
			BomEncoder::Encoding				encoding;
			bool								containsBom;
			WString								cachedContent;
			IPlainTextViewContentProxy*			contentProxy;
			
			bool								ActivateProxy(IPlainTextViewContentProxy* value)override;
			bool								DeactivateProxy(IPlainTextViewContentProxy* value)override;
		protected:
			
			Ptr<IEncoder>						CreateEncoder();
			Ptr<IDecoder>						CreateDecoder();
			bool								LoadDocumentInternal(const WString& filePath)override;
			bool								SaveDocumentInternal(const WString& filePath)override;
		public:

			TextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath=L"", DocumentFragment* _ownedFragment=0, const WString& _friendlyName=L"Text Document");
			~TextDocument();

			BomEncoder::Encoding				GetEncoding();
			void								SetEncoding(BomEncoder::Encoding value);
			bool								GetContainsBom();
			void								SetContainsBom(bool value);
			WString								GetContent()override;
			void								SetContent(const WString& value, DocumentView* senderView=0)override;
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