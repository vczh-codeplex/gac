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
			,contentProvider(0)
		{
		}

		PlainTextView::~PlainTextView()
		{
		}

		bool PlainTextView::InstallContentProvider(IPlainTextViewContentProvider* value)
		{
			if(contentProvider) return false;
			contentProvider=value;
			return true;
		}

		bool PlainTextView::ActivateProxy(IPlainTextViewContentProxy* value)
		{
			if(!contentProvider) return false;
			return contentProvider->ActivateProxy(value);
		}

		bool PlainTextView::DeactivateProxy(IPlainTextViewContentProxy* value)
		{
			if(!contentProvider) return false;
			return contentProvider->DeactivateProxy(value);
		}

		WString PlainTextView::GetContent()
		{
			if(!contentProvider) return L"";
			return contentProvider->GetContent();
		}

		void PlainTextView::SetContent(const WString& value)
		{
			if(!contentProvider) return;
			contentProvider->SetContent(value, this);
		}

/***********************************************************************
TextDocument
***********************************************************************/

		bool TextDocument::ActivateProxy(IPlainTextViewContentProxy* value)
		{
			if(contentProxy) return false;
			contentProxy=value;
			value->SetContent(cachedContent);
			return true;
		}

		bool TextDocument::DeactivateProxy(IPlainTextViewContentProxy* value)
		{
			if(!contentProxy) return false;
			if(contentProxy!=value) return false;
			cachedContent=value->GetContent();
			return true;
		}

		bool TextDocument::LoadDocumentInternal(const WString& filePath)
		{
			return true;
		}

		bool TextDocument::SaveDocumentInternal(const WString& filePath)
		{
			FileStream fileStream(filePath, FileStream::WriteOnly);
			if(!fileStream.IsAvailable()) return false;
			Ptr<IEncoder> encoder;
			if(containsBom)
			{
				switch(encoding)
				{
				case Utf8:
					encoder=new BomEncoder(BomEncoder::Utf8);
					break;
				case Utf16:
					encoder=new BomEncoder(BomEncoder::Utf16);
					break;
				case Utf16BigEndian:
					encoder=new BomEncoder(BomEncoder::Utf16BE);
					break;
				default:
					encoder=new BomEncoder(BomEncoder::Mbcs);
				}
			}
			else
			{
				switch(encoding)
				{
				case Utf8:
					encoder=new Utf8Encoder;
					break;
				case Utf16:
					encoder=new Utf16Encoder;
					break;
				case Utf16BigEndian:
					encoder=new Utf16BEEncoder;
					break;
				default:
					encoder=new MbcsEncoder;
				}
			}
			EncoderStream encoderStream(fileStream, *encoder.Obj());
			StreamWriter writer(encoderStream);
			writer.WriteString(GetContent());
			return true;
		}

		TextDocument::TextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:FileDocumentFragment(_ownedContainer, _ownedFragment, _friendlyName, _filePath)
			,contentProxy(0)
			,encoding(Utf16)
			,containsBom(true)
		{
			Ptr<PlainTextView> plainTextView=new PlainTextView(this);
			plainTextView->InstallContentProvider(this);
			AddSupportedView(plainTextView);
			SetDefaultView(plainTextView);
		}

		TextDocument::~TextDocument()
		{
		}

		TextDocument::Encoding TextDocument::GetEncoding()
		{
			return encoding;
		}

		void TextDocument::SetEncoding(Encoding value)
		{
			encoding=value;
		}

		bool TextDocument::GetContainsBom()
		{
			return containsBom;
		}

		void TextDocument::SetContainsBom(bool value)
		{
			containsBom=false;
		}

		WString TextDocument::GetContent()
		{
			return contentProxy?contentProxy->GetContent():cachedContent;
		}

		void TextDocument::SetContent(const WString& value, DocumentView* senderView)
		{
			if(contentProxy)
			{
				contentProxy->SetContent(value);
			}
			else
			{
				cachedContent=value;
			}
			NotifyUpdateFragmentAndViews(senderView);
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