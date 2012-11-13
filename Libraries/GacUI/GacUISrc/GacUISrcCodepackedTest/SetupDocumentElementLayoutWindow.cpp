#include "..\..\Public\Source\GacUI.h"

using namespace vl::stream;
using namespace vl::regex;

namespace document
{
	int ConvertHex(wchar_t c)
	{
		if(L'a'<=c && c<=L'f') return c-L'a'+10;
		if(L'A'<=c && c<=L'F') return c-L'A'+10;
		if(L'0'<=c && c<=L'9') return c-L'0';
		return 0;
	}

	Color ConvertColor(const WString& colorString)
	{
		return Color(
			ConvertHex(colorString[1])*16+ConvertHex(colorString[2]),
			ConvertHex(colorString[3])*16+ConvertHex(colorString[4]),
			ConvertHex(colorString[5])*16+ConvertHex(colorString[6])
			);
	}
	
	Ptr<text::DocumentModel> BuildDocumentModel(const WString& fileName)
	{
		HDC dc=CreateCompatibleDC(NULL);
		int dpi=GetDeviceCaps(dc, LOGPIXELSY);
		DeleteDC(dc);
		Ptr<text::DocumentModel> document=new text::DocumentModel;

		WString rawDocument;
		{
			FileStream fileStream(fileName, FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			rawDocument=reader.ReadToEnd();
		}

		WString regexTag_s=L"<(<tag>s)>(<font>[^:]+):(<bold>[^:]+):(<color>[^:]+):(<size>[^:]+):(<text>/.*?)<//s>";
		WString regexTag_i=L"<(<tag>i)>(<cx>[^:]+),(<cy>[^:]+):(<b>[^:]+):(<file>/.*?)<//i>";
		WString regexTag_p=L"<(<tag>p)//>";
		Regex regexTag(regexTag_s+L"|"+regexTag_i+L"|"+regexTag_p);
		Regex regexLine(L"\r\n");
		RegexMatch::List matches;
		regexTag.Search(rawDocument, matches);

		Ptr<text::DocumentParagraph> paragraph=0;
		Ptr<text::DocumentLine> line=0;
		
		for(int i=0;i<matches.Count();i++)
		{
			Ptr<RegexMatch> match=matches[i];
			if(match->Groups()[L"tag"][0].Value()==L"p")
			{
				paragraph=0;
				line=0;
			}
			else if(match->Groups()[L"tag"][0].Value()==L"i")
			{
				int cx=wtoi(match->Groups()[L"cx"][0].Value());
				int cy=wtoi(match->Groups()[L"cy"][0].Value());
				int b=wtoi(match->Groups()[L"b"][0].Value());
				WString file=match->Groups()[L"file"][0].Value();

				if(!paragraph)
				{
					paragraph=new text::DocumentParagraph;
					document->paragraphs.Add(paragraph);
					line=0;
				}
				if(!line)
				{
					line=new text::DocumentLine;
					paragraph->lines.Add(line);
				}

				Ptr<text::DocumentImageRun> run=new text::DocumentImageRun;
				run->size=Size(cx, cy);
				run->baseline=b;
				run->image=GetCurrentController()->ImageService()->CreateImageFromFile(L"Resources\\"+file);
				run->frameIndex=0;
				line->runs.Add(run);
			}
			else if(match->Groups()[L"tag"][0].Value()==L"s")
			{
				FontProperties fontStyle;
				Color fontColor;
				RegexMatch::List lines;
				{
					WString font=match->Groups()[L"font"][0].Value();
					WString bold=match->Groups()[L"bold"][0].Value();
					WString color=match->Groups()[L"color"][0].Value();
					WString size=match->Groups()[L"size"][0].Value();
					WString text=match->Groups()[L"text"][0].Value();

					fontStyle.fontFamily=font;
					fontStyle.bold=bold==L"true";
					fontStyle.size=(int)(wtof(size)*dpi/72);
					fontColor=ConvertColor(color);
					regexLine.Split(text, true, lines);
				}

				for(int j=0;j<lines.Count();j++)
				{
					WString lineText=lines[j]->Result().Value();
					if(!paragraph)
					{
						paragraph=new text::DocumentParagraph;
						document->paragraphs.Add(paragraph);
						line=0;
					}
					if(!line || j>0)
					{
						line=new text::DocumentLine;
						paragraph->lines.Add(line);
					}

					Ptr<text::DocumentTextRun> run=new text::DocumentTextRun;
					run->style=fontStyle;
					run->color=fontColor;
					run->text=lineText;
					line->runs.Add(run);
				}
			}
		}

		return document;
	}
}
using namespace document;

void SetupDocumentElementLayoutWindow(GuiControlHost* controlHost, GuiControl* container, const WString& filename)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	
	GuiScrollContainer* scriptDocumentView=new GuiScrollContainer(GetCurrentTheme()->CreateMultilineTextBoxStyle());
	scriptDocumentView->SetExtendToFullWidth(true);
	scriptDocumentView->SetHorizontalAlwaysVisible(false);
	scriptDocumentView->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
	scriptDocumentView->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));
	
	GetApplication()->InvokeAsync([=]()
	{
		Ptr<text::DocumentModel> document=BuildDocumentModel(filename);
		GetApplication()->InvokeInMainThreadAndWait([=]()
		{
			scriptDocumentView->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());
			GuiDocumentElement* element=GuiDocumentElement::Create();
			element->SetDocument(document);

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			composition->SetOwnedElement(element);
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
			composition->SetAlignmentToParent(Margin(10, 10, 10, 10));
			scriptDocumentView->GetContainerComposition()->AddChild(composition);
		});
	});

	container->GetContainerComposition()->AddChild(scriptDocumentView->GetBoundsComposition());
}