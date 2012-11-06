#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Regex\Regex.h"

#include "..\..\Source\GraphicsElement\WindowsDirect2D\GuiGraphicsWindowsDirect2D.h"
#include "..\..\Source\GraphicsElement\WindowsGDI\GuiGraphicsWindowsGDI.h"
#include <usp10.h>

#pragma comment(lib, "usp10.lib")

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
using namespace vl::presentation::windows;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsGDIRenderer();
}

/***********************************************************************
Uniscribe
***********************************************************************/

bool operator==(const SCRIPT_ITEM&, const SCRIPT_ITEM&){return false;}
bool operator!=(const SCRIPT_ITEM&, const SCRIPT_ITEM&){return false;}

bool operator==(const SCRIPT_VISATTR&, const SCRIPT_VISATTR&){return false;}
bool operator!=(const SCRIPT_VISATTR&, const SCRIPT_VISATTR&){return false;}

bool operator==(const GOFFSET&, const GOFFSET&){return false;}
bool operator!=(const GOFFSET&, const GOFFSET&){return false;}

bool operator==(const SCRIPT_LOGATTR&, const SCRIPT_LOGATTR&){return false;}
bool operator!=(const SCRIPT_LOGATTR&, const SCRIPT_LOGATTR&){return false;}

namespace test
{

/***********************************************************************
DocumentFragment
***********************************************************************/

	class DocumentFragment : public Object
	{
	public:
		bool				paragraph;
		WString				font;
		bool				bold;
		Color				color;
		int					size;
		WString				text;
		Ptr<WinFont>		fontObject;

		DocumentFragment()
			:paragraph(false)
			,bold(false)
			,size(0)
		{
		}

		DocumentFragment(Ptr<DocumentFragment> prototype, const WString& _text)
			:paragraph(prototype->paragraph)
			,font(prototype->font)
			,bold(prototype->bold)
			,color(prototype->color)
			,size(prototype->size)
			,fontObject(prototype->fontObject)
			,text(_text)
		{
		}

		WString GetFingerPrint()
		{
			return font+L":"+(bold?L"B":L"N")+L":"+itow(size);
		}
	};

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

	void BuildDocumentFragments(const WString& fileName, List<Ptr<DocumentFragment>>& fragments)
	{
		fragments.Clear();
		WString rawDocument;
		{
			FileStream fileStream(fileName, FileStream::ReadOnly);
			Utf8Decoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			rawDocument=reader.ReadToEnd();
		}

		Regex regex(L"<(<tag>s)>(<font>[^:]+):(<bold>[^:]+):(<color>[^:]+):(<size>[^:]+):(<text>/.*?)<//s>|<(<tag>p)//>");
		RegexMatch::List matches;
		regex.Search(rawDocument, matches);
		
		for(int i=0;i<matches.Count();i++)
		{
			Ptr<RegexMatch> match=matches[i];
			Ptr<DocumentFragment> fragment=new DocumentFragment;
			fragments.Add(fragment);
			if(match->Groups()[L"tag"][0].Value()==L"p")
			{
				fragment->paragraph=true;
			}
			else
			{
				WString font=match->Groups()[L"tag"][0].Value();
				WString bold=match->Groups()[L"bold"][0].Value();
				WString color=match->Groups()[L"color"][0].Value();
				WString size=match->Groups()[L"size"][0].Value();
				WString text=match->Groups()[L"text"][0].Value();

				fragment->font=font;
				fragment->bold=bold==L"true";
				fragment->size=wtoi(size);
				fragment->color=ConvertColor(color);
				fragment->text=text;
			}
		}
	}

/***********************************************************************
ScriptFragment
***********************************************************************/
	
	struct GlyphData
	{
		Array<WORD>					glyphs;
		Array<SCRIPT_VISATTR>		glyphVisattrs;
		Array<int>					glyphAdvances;
		Array<GOFFSET>				glyphOffsets;
		Array<WORD>					charCluster;
		ABC							runAbc;

		GlyphData()
		{
			memset(&runAbc, 0, sizeof(runAbc));
		}

		void ClearUniscribeData(int glyphCount, int length)
		{
			glyphs.Resize(glyphCount);
			glyphVisattrs.Resize(glyphCount);
			glyphAdvances.Resize(glyphCount);
			glyphOffsets.Resize(glyphCount);
			charCluster.Resize(length);
			memset(&runAbc, 0, sizeof(runAbc));
		}
			
		bool BuildUniscribeData(WinDC* dc, DocumentFragment* documentFragment, SCRIPT_ITEM* scriptItem, SCRIPT_CACHE& scriptCache, const wchar_t* runText, int length)
		{
			int glyphCount=glyphs.Count();
			bool resizeGlyphData=false;
			if(glyphCount==0)
			{
				glyphCount=(int)(1.5*length+16);
				resizeGlyphData=true;
			}
			{
				// generate shape information
				WinDC* dcParameter=0;
				if(resizeGlyphData)
				{
					glyphs.Resize(glyphCount);
					glyphVisattrs.Resize(glyphCount);
					charCluster.Resize(length);
				}

				while(true)
				{
					int availableGlyphCount=0;
					HRESULT hr=ScriptShape(
						(dcParameter?dcParameter->GetHandle():NULL),
						&scriptCache,
						runText,
						length,
						glyphCount,
						&scriptItem->a,
						&glyphs[0],
						&charCluster[0],
						&glyphVisattrs[0],
						&availableGlyphCount
						);
					if(hr==0)
					{
						glyphCount=availableGlyphCount;
						break;
					}
					else if(hr==E_PENDING)
					{
						dcParameter=dc;
					}
					else if(hr==E_OUTOFMEMORY)
					{
						if(resizeGlyphData)
						{
							glyphCount+=length;
						}
						else
						{
							goto BUILD_UNISCRIBE_DATA_FAILED;
						}
					}
					else
					{
						goto BUILD_UNISCRIBE_DATA_FAILED;
					}
				}
				if(resizeGlyphData)
				{
					glyphs.Resize(glyphCount);
					glyphVisattrs.Resize(glyphCount);
				}
			}
			{
				// generate place information
				WinDC* dcParameter=0;
				if(resizeGlyphData)
				{
					glyphAdvances.Resize(glyphCount);
					glyphOffsets.Resize(glyphCount);
				}
				while(true)
				{
					HRESULT hr=ScriptPlace(
						(dcParameter?dcParameter->GetHandle():NULL),
						&scriptCache,
						&glyphs[0],
						glyphCount,
						&glyphVisattrs[0],
						&scriptItem->a,
						&glyphAdvances[0],
						&glyphOffsets[0],
						&runAbc
						);
					if(hr==0)
					{
						break;
					}
					else if(hr==E_PENDING)
					{
						dcParameter=dc;
					}
					else
					{
						goto BUILD_UNISCRIBE_DATA_FAILED;
					}
				}
			}

			return true;
BUILD_UNISCRIBE_DATA_FAILED:
			return false;
		}
	};

	class ScriptRun : public Object
	{
	public:

		DocumentFragment*				documentFragment;
		SCRIPT_ITEM*					scriptItem;
		int								start;
		int								length;
		const wchar_t*					runText;

		SCRIPT_CACHE					scriptCache;
		Array<SCRIPT_LOGATTR>			charLogattrs;
		int								advance;
		GlyphData						wholeGlyph;
		GlyphData						tempGlyph;

		ScriptRun()
			:documentFragment(0)
			,scriptItem(0)
			,start(0)
			,length(0)
			,scriptCache(0)
			,advance(0)
		{
		}

		~ScriptRun()
		{
			ClearUniscribeData();
		}

		void ClearUniscribeData()
		{
			if(scriptCache)
			{
				ScriptFreeCache(&scriptCache);
				scriptCache=0;
			}
			charLogattrs.Resize(0);
			advance=0;
			wholeGlyph.ClearUniscribeData(0, 0);
			tempGlyph.ClearUniscribeData(0, 0);
		}

		bool BuildUniscribeData(WinDC* dc)
		{
			ClearUniscribeData();
			{
				// generate break information
				charLogattrs.Resize(length);

				HRESULT hr=ScriptBreak(
					runText,
					length,
					&scriptItem->a,
					&charLogattrs[0]
					);
				if(hr!=0)
				{
					goto BUILD_UNISCRIBE_DATA_FAILED;
				}
			}

			dc->SetFont(documentFragment->fontObject);
			if(!wholeGlyph.BuildUniscribeData(dc, documentFragment, scriptItem, scriptCache, runText, length))
			{
				goto BUILD_UNISCRIBE_DATA_FAILED;
			}
			tempGlyph.ClearUniscribeData(wholeGlyph.glyphs.Count(), length);
			advance=wholeGlyph.runAbc.abcA+wholeGlyph.runAbc.abcB+wholeGlyph.runAbc.abcC;

			return true;
BUILD_UNISCRIBE_DATA_FAILED:
			ClearUniscribeData();
			return false;
		}

		int SumWidth(int charStart, int charLength)
		{
			int width=0;
			for(int i=0;i<charLength;i++)
			{
				width+=wholeGlyph.glyphAdvances[i];
			}
			return width;
		}

		void SearchForLineBreak(int tempStart, int maxWidth, bool firstRun, int& charLength, int& charAdvances)
		{
			int width=0;
			charLength=0;
			charAdvances=0;
			for(int i=tempStart;i<=length;i++)
			{
				if(i==length || charLogattrs[i].fSoftBreak==TRUE)
				{
					if(width<=maxWidth || (firstRun && charLength==0))
					{
						charLength=i-tempStart;
						charAdvances=width;
					}
				}
				if(i==length) break;
				width+=wholeGlyph.glyphAdvances[i];
			}
		}

		bool BuildUniscribeDataTemp(WinDC* dc, int tempStart, int tempLength)
		{
			return tempGlyph.BuildUniscribeData(dc, documentFragment, scriptItem, scriptCache, runText+tempStart, tempLength);
		}
	};

	class ScriptLine : public Object
	{
	public:
		List<Ptr<DocumentFragment>>		documentFragments;
		WString							lineText;

		Array<SCRIPT_ITEM>				scriptItems;
		List<Ptr<ScriptRun>>			scriptRuns;

		void CLearUniscribeData()
		{
			scriptItems.Resize(0);
			scriptRuns.Clear();
		}

		bool BuildUniscribeData(WinDC* dc)
		{
			lineText=L"";
			CLearUniscribeData();

			FOREACH(Ptr<DocumentFragment>, fragment, documentFragments.Wrap())
			{
				lineText+=fragment->text;
			}

			if(lineText!=L"")
			{
				{
					// itemize a line
					scriptItems.Resize(lineText.Length()+2);
					int scriptItemCount=0;
					HRESULT hr=ScriptItemize(
						lineText.Buffer(),
						lineText.Length(),
						scriptItems.Count()-1,
						NULL,
						NULL,
						&scriptItems[0],
						&scriptItemCount
						);
					if(hr!=0)
					{
						goto BUILD_UNISCRIBE_DATA_FAILED;
					}
					scriptItems.Resize(scriptItemCount+1);
				}
				{
					// use item and document fragment information to produce runs
					// one item is constructed by one or more runs
					// characters in each run contains the same style
					int fragmentIndex=0;
					int fragmentStart=0;
					for(int i=0;i<scriptItems.Count()-1;i++)
					{
						SCRIPT_ITEM* scriptItem=&scriptItems[i];
						int start=scriptItem[0].iCharPos;
						int length=scriptItem[1].iCharPos-scriptItem[0].iCharPos;
						int currentStart=start;

						while(currentStart<start+length)
						{
							DocumentFragment* fragment=0;
							int itemRemainLength=length-(currentStart-start);
							int fragmentRemainLength=0;
							while(true)
							{
								fragment=documentFragments[fragmentIndex].Obj();
								fragmentRemainLength=fragment->text.Length()-(currentStart-fragmentStart);
								if(fragmentRemainLength<=0)
								{
									fragmentStart+=fragment->text.Length();
									fragmentIndex++;
								}
								else
								{
									break;
								}
							}
							int shortLength=itemRemainLength<fragmentRemainLength?itemRemainLength:fragmentRemainLength;

							Ptr<ScriptRun> run=new ScriptRun;
							run->documentFragment=fragment;
							run->scriptItem=scriptItem;
							run->start=currentStart;
							run->length=shortLength;
							run->runText=lineText.Buffer()+currentStart;
							scriptRuns.Add(run);
							currentStart+=shortLength;
						}
					}

					// for each run, generate shape information
					FOREACH(Ptr<ScriptRun>, run, scriptRuns.Wrap())
					{
						if(!run->BuildUniscribeData(dc))
						{
							goto BUILD_UNISCRIBE_DATA_FAILED;
						}
					}
				}
			}
			return true;
BUILD_UNISCRIBE_DATA_FAILED:
			CLearUniscribeData();
			return false;
		}
	};

	class ScriptParagraph : public Object
	{
	public:
		List<Ptr<ScriptLine>>			lines;
	};

	class ScriptDocument : public Object
	{
	public:
		List<Ptr<ScriptParagraph>>		paragraphs;
	};

	Ptr<ScriptDocument> BuildScriptParagraphs(List<Ptr<DocumentFragment>>& fragments)
	{
		Ptr<ScriptDocument> document=new ScriptDocument;
		document->paragraphs.Clear();
		Regex regex(L"\r\n");
		Ptr<ScriptParagraph> currentParagraph;
		Ptr<ScriptLine> currentLine;
		Dictionary<WString, Ptr<WinFont>> fonts;

		FOREACH(Ptr<DocumentFragment>, fragment, fragments.Wrap())
		{
			WString fragmentFingerPrint=fragment->GetFingerPrint();
			int index=fonts.Keys().IndexOf(fragmentFingerPrint);
			if(index==-1)
			{
				fragment->fontObject=new WinFont(fragment->font, fragment->size, 0, 0, 0, (fragment->bold?FW_BOLD:FW_NORMAL), false, false, false, true);
				fonts.Add(fragmentFingerPrint, fragment->fontObject);
			}
			else
			{
				fragment->fontObject=fonts.Values()[index];
			}

			if(!currentParagraph)
			{
				currentParagraph=new ScriptParagraph;
				document->paragraphs.Add(currentParagraph);
			}
			
			if(fragment->paragraph)
			{
				currentParagraph=0;
				currentLine=0;
			}
			else
			{
				RegexMatch::List matches;
				regex.Split(fragment->text, true, matches);
				for(int i=0;i<matches.Count();i++)
				{
					Ptr<RegexMatch> match=matches[i];
					if(i>0)
					{
						currentLine=0;
					}
					if(!currentLine)
					{
						currentLine=new ScriptLine;
						currentParagraph->lines.Add(currentLine);
					}
					currentLine->documentFragments.Add(new DocumentFragment(fragment, match->Result().Value()));
				}
			}
		}

		HDC hdc=CreateCompatibleDC(NULL);
		WinProxyDC dc;
		dc.Initialize(hdc);
		FOREACH(Ptr<ScriptParagraph>, paragraph, document->paragraphs.Wrap())
		{
			FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
			{
				line->BuildUniscribeData(&dc);
			}
		}
		DeleteDC(hdc);

		return document;
	}

/***********************************************************************
TestWindow
***********************************************************************/

	class TestWindow : public GuiWindow
	{
	protected:
		Ptr<ScriptDocument>				document;
		Ptr<WinFont>					messageFont;

		void element_Rendering(GuiGraphicsComposition* composition, GuiGDIElementEventArgs& arguments)
		{
			WinDC* dc=arguments.dc;
			Rect bounds=arguments.bounds;
			if(document)
			{
				int x=bounds.Left()+10;
				int y=bounds.Top()+10;
				int w=bounds.Width()-20;
				int h=bounds.Height()-10;
				int cx=0;
				int cy=0;
				const int lineDistance=5;
				const int paragraphDistance=10;

				FOREACH(Ptr<ScriptParagraph>, paragraph, document->paragraphs.Wrap())
				{
					if(cy>=h) break;
					FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
					{
						if(line->scriptRuns.Count()==0)
						{
							// if this line doesn't contains any run, skip and render a blank line
							cy+=line->documentFragments[0]->size+lineDistance;
						}
						else
						{
							// render this line into linces with auto line wrapping
							int startRun=0;
							int startRunOffset=0;
							int lastRun=0;
							int lastRunOffset=0;
							int currentWidth=0;

							while(startRun<line->scriptRuns.Count())
							{
								int currentWidth=0;
								bool firstRun=true;
								// search for a range to fit in the given width
								for(int i=startRun;i<line->scriptRuns.Count();i++)
								{
									int charLength=0;
									int charAdvances=0;
									ScriptRun* run=line->scriptRuns[i].Obj();
									run->SearchForLineBreak(lastRunOffset, w-currentWidth, firstRun, charLength, charAdvances);
									firstRun=false;

									if(charLength==run->length-lastRunOffset)
									{
										lastRun=i+1;
										lastRunOffset=0;
										currentWidth+=charAdvances;
									}
									else
									{
										lastRun=i;
										lastRunOffset=lastRunOffset+charLength;
										break;
									}
								}

								// if the range is empty, than this should be the end of line, ignore it
								if(startRun<lastRun || (startRun==lastRun && startRunOffset<lastRunOffset))
								{
									// calculate the max line height in this range;
									int maxHeight=0;
									for(int i=startRun;i<=lastRun && i<line->scriptRuns.Count();i++)
									{
										int size=line->scriptRuns[i]->documentFragment->size;
										if(maxHeight<size)
										{
											maxHeight=size;
										}
									}

									// render all runs inside this range
									for(int i=startRun;i<=lastRun && i<line->scriptRuns.Count();i++)
									{
										ScriptRun* run=line->scriptRuns[i].Obj();
										int start=i==startRun?startRunOffset:0;
										int end=i==lastRun?lastRunOffset:run->length;
										int length=end-start;
											
										Color color=run->documentFragment->color;
										dc->SetFont(run->documentFragment->fontObject);
										dc->SetTextColor(RGB(color.r, color.g, color.b));
										dc->DrawBuffer(x+cx, y+cy+(maxHeight-run->documentFragment->size), run->runText+start, length);

										cx+=run->SumWidth(start, length);
									}

									cx=0;
									cy+=maxHeight+lineDistance;
								}

								startRun=lastRun;
								startRunOffset=lastRunOffset;
							}
						}
					}
					cy+=paragraphDistance;
				}
			}
			else
			{
				dc->SetFont(messageFont);
				WString message=L"Initializing uniscribe data...";
				SIZE size=dc->MeasureString(message);
				int x=bounds.Left()+(bounds.Width()-size.cx)/2;
				int y=bounds.Top()+(bounds.Height()-size.cy)/2;
				dc->DrawString(x, y, message);
			}
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			MoveToScreenCenter();
			{
				GuiGDIElement* element=GuiGDIElement::Create();
				element->Rendering.AttachMethod(this, &TestWindow::element_Rendering);
			
				GuiBoundsComposition* composition=new GuiBoundsComposition;
				composition->SetOwnedElement(element);
				composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				GetContainerComposition()->AddChild(composition);

				messageFont=new WinFont(L"Segoe UI", 56, 0, 0, 0,FW_NORMAL, false, false, false, true);
			}
			GetApplication()->InvokeAsync([=]()
			{
				List<Ptr<DocumentFragment>> fragments;
				BuildDocumentFragments(L"..\\GacUISrcCodepackedTest\\Resources\\document.txt", fragments);
				Ptr<ScriptDocument> scriptDocument=BuildScriptParagraphs(fragments);
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					document=scriptDocument;
				});
			});
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}