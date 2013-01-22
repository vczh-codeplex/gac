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
				switch(encoding)
				{
				case Utf8:
					return new BomEncoder(BomEncoder::Utf8);
				case Utf16:
					return new BomEncoder(BomEncoder::Utf16);
				case Utf16BigEndian:
					return new BomEncoder(BomEncoder::Utf16BE);
				default:
					return new BomEncoder(BomEncoder::Mbcs);
				}
			}
			else
			{
				switch(encoding)
				{
				case Utf8:
					return new Utf8Encoder;
				case Utf16:
					return new Utf16Encoder;
				case Utf16BigEndian:
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
				case Utf8:
					return new Utf8Decoder;
				case Utf16:
					return new Utf16Decoder;
				case Utf16BigEndian:
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
					encoding=Ansi;
					containsBom=true;
					cachedContent=L"";
					return true;
				}
				buffer.Resize((vint)fileStream.Size());
				fileStream.Read(&buffer[0], size);
			}

			if(buffer.Count()>=3 && strncmp(&buffer[0], "\xEF\xBB\xBF", 3)==0)
			{
				encoding=Utf8;
				containsBom=true;
			}
			else if(buffer.Count()>=2 && strncmp(&buffer[0], "\xFF\xFE", 2)==0)
			{
				encoding=Utf16;
				containsBom=true;
			}
			else if(buffer.Count()>=2 && strncmp(&buffer[0], "\xFE\xFF", 2)==0)
			{
				encoding=Utf16BigEndian;
				containsBom=true;
			}
			else
			{
				encoding=Ansi;
				containsBom=false;

				
				bool notAscii=false;
				bool notUnicode=false;
				bool unicode=false;
				bool reverse=false;
				bool strictUtf8Test=true;
				{
					void* text=&buffer[0];
					int textSize=(int)buffer.Count();
					int flag=0;

					flag=IS_TEXT_UNICODE_NOT_ASCII_MASK;
					notAscii=IsTextUnicode(text, textSize, &flag)!=0;
					flag=IS_TEXT_UNICODE_NOT_UNICODE_MASK;
					notUnicode=IsTextUnicode(text, textSize, &flag)!=0;
					flag=IS_TEXT_UNICODE_UNICODE_MASK;
					unicode=IsTextUnicode(text, textSize, &flag)!=0;
					flag=IS_TEXT_UNICODE_REVERSE_MASK;
					reverse=IsTextUnicode(text, textSize, &flag)!=0;
				}

				for(vint i=0;strictUtf8Test && i<buffer.Count();i++)
				{
					unsigned char c=(unsigned char)buffer[i];
					if(c==0)
					{
						strictUtf8Test=false;
					}
					else
					{
						vint count10xxxxxx=0;
						if((c&0x80)==0x00) /* 0x0xxxxxxx */ count10xxxxxx=0;
						else if((c&0xE0)==0xC0) /* 0x110xxxxx */ count10xxxxxx=1;
						else if((c&0xF0)==0xE0) /* 0x1110xxxx */ count10xxxxxx=2;
						else if((c&0xF8)==0xF0) /* 0x11110xxx */ count10xxxxxx=3;
						else if((c&0xFC)==0xF8) /* 0x111110xx */ count10xxxxxx=4;
						else if((c&0xFE)==0xFC) /* 0x1111110x */ count10xxxxxx=5;

						if(buffer.Count()<=i+count10xxxxxx)
						{
							strictUtf8Test=false;
						}
						else
						{
							for(vint j=0;strictUtf8Test && j<count10xxxxxx;j++)
							{
								c=(unsigned char)buffer[i+j+1];
								if((c&0xC0)!=0x80) /* 0x10xxxxxx */ strictUtf8Test=false;
							}
						}
						i+=count10xxxxxx;
					}
				}

				if(notUnicode)
				{
					if(strictUtf8Test || notAscii)
					{
						encoding=Utf8;
					}
				}
				else
				{
					if(unicode)
					{
						encoding=Utf16;
					}
					else if(reverse)
					{
						encoding=Utf16BigEndian;
					}
					else if(strictUtf8Test || notAscii)
					{
						encoding=Utf8;
					}
				}
				containsBom=encoding==Ansi;
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