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

		Ptr<IEncoder> TextDocument::CreateEncoder()
		{
			if(containsBom)
			{
				return new BomEncoder(encoding);
			}
			else
			{
				switch(encoding)
				{
				case BomEncoder::Utf8:
					return new Utf8Encoder;
				case BomEncoder::Utf16:
					return new Utf16Encoder;
				case BomEncoder::Utf16BE:
					return new Utf16BEEncoder;
				default:
					return new MbcsEncoder;
				}
			}
		}

		Ptr<IDecoder> TextDocument::CreateDecoder()
		{
			if(containsBom)
			{
				return new BomDecoder;
			}
			else
			{
				switch(encoding)
				{
				case BomEncoder::Utf8:
					return new Utf8Decoder;
				case BomEncoder::Utf16:
					return new Utf16Decoder;
				case BomEncoder::Utf16BE:
					return new Utf16BEDecoder;
				default:
					return new MbcsDecoder;
				}
			}
		}

		bool TextDocument::LoadDocumentInternal(const WString& filePath)
		{
			Array<char> buffer;
			{
				FileStream fileStream(filePath, FileStream::ReadOnly);
				if(!fileStream.IsAvailable()) return false;
				vint size=(vint)fileStream.Size();
				if(size==0)
				{
					encoding=BomEncoder::Mbcs;
					containsBom=true;
					cachedContent=L"";
					return true;
				}
				buffer.Resize((vint)fileStream.Size());
				fileStream.Read(&buffer[0], size);
			}

			if(buffer.Count()>=3 && strncmp(&buffer[0], "\xEF\xBB\xBF", 3)==0)
			{
				encoding=BomEncoder::Utf8;
				containsBom=true;
			}
			else if(buffer.Count()>=2 && strncmp(&buffer[0], "\xFF\xFE", 2)==0)
			{
				encoding=BomEncoder::Utf16;
				containsBom=true;
			}
			else if(buffer.Count()>=2 && strncmp(&buffer[0], "\xFE\xFF", 2)==0)
			{
				encoding=BomEncoder::Utf16BE;
				containsBom=true;
			}
			else
			{
				TestEncoding((unsigned char*)&buffer[0], buffer.Count(), encoding, containsBom);
			}

			MemoryWrapperStream memoryStream(&buffer[0], buffer.Count());
			Ptr<IDecoder> decoder=CreateDecoder();
			DecoderStream decoderStream(memoryStream, *decoder.Obj());
			StreamReader reader(decoderStream);
			cachedContent=reader.ReadToEnd();
			return true;
		}

		bool TextDocument::SaveDocumentInternal(const WString& filePath)
		{
			FileStream fileStream(filePath, FileStream::WriteOnly);
			if(!fileStream.IsAvailable()) return false;
			Ptr<IEncoder> encoder=CreateEncoder();
			EncoderStream encoderStream(fileStream, *encoder.Obj());
			StreamWriter writer(encoderStream);
			writer.WriteString(GetContent());
			return true;
		}

		TextDocument::TextDocument(IDocumentContainer* _ownedContainer, const WString& _filePath, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:FileDocumentFragment(_ownedContainer, _ownedFragment, _friendlyName, _filePath)
			,contentProxy(0)
			,encoding(BomEncoder::Utf16)
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

		BomEncoder::Encoding TextDocument::GetEncoding()
		{
			return encoding;
		}

		void TextDocument::SetEncoding(BomEncoder::Encoding value)
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