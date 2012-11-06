#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Regex\Regex.h"

#include "..\..\Source\NativeWindow\Windows\Direct2D\WinDirect2DApplication.h"
#include "..\..\Source\NativeWindow\Windows\GDI\WinGDIApplication.h"
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
			,text(_text)
		{
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

	class ScriptRun
	{
	public:
		DocumentFragment*				documentFragment;
		SCRIPT_ITEM*					scriptItem;
		int								start;
		int								length;
		const wchar_t*					runText;

		SCRIPT_CACHE					scriptCache;
		Array<WORD>						glyphs;
		Array<SCRIPT_VISATTR>			glyphVisattrs;
		Array<int>						glyphAdvances;
		Array<GOFFSET>					glyphOffsets;
		Array<WORD>						charCluster;
		Array<SCRIPT_LOGATTR>			charLogattrs;
		ABC								runAbc;

		ScriptRun()
			:documentFragment(0)
			,scriptItem(0)
			,start(0)
			,length(0)
			,scriptCache(0)
		{
			memset(&runAbc, 0, sizeof(runAbc));
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
		}

		bool BuildUniscribeData(WinDC* dc)
		{
			ClearUniscribeData();
			int glyphCount=(int)(1.5*length+16);
			{
				// generate shape information

				WinDC* dcParameter=0;
				glyphs.Resize(glyphCount);
				glyphVisattrs.Resize(glyphCount);
				charCluster.Resize(length);

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
						glyphCount+=length;
					}
					else
					{
						goto BUILD_UNISCRIBE_DATA_FAILED;
					}
				}
				glyphs.Resize(glyphCount);
				glyphVisattrs.Resize(glyphCount);
			}
			{
				// generate place information
				WinDC* dcParameter=0;
				glyphAdvances.Resize(glyphCount);
				glyphOffsets.Resize(glyphCount);

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

			return true;
BUILD_UNISCRIBE_DATA_FAILED:
			ClearUniscribeData();
			return false;
		}
	};

	class ScriptLine
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

	class ScriptParagraph
	{
	public:
		List<Ptr<ScriptLine>>			lines;
	};

	void BuildScriptParagraphs(List<Ptr<DocumentFragment>>& fragments, List<Ptr<ScriptParagraph>>& script)
	{
		script.Clear();
		Regex regex(L"\r\n");
		Ptr<ScriptParagraph> currentParagraph;
		Ptr<ScriptLine> currentLine;

		FOREACH(Ptr<DocumentFragment>, fragment, fragments.Wrap())
		{
			if(!currentParagraph)
			{
				currentParagraph=new ScriptParagraph;
				script.Add(currentParagraph);
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
		FOREACH(Ptr<ScriptParagraph>, paragraph, script.Wrap())
		{
			FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
			{
				line->BuildUniscribeData(&dc);
			}
		}
		DeleteDC(hdc);
	}

/***********************************************************************
TestWindow
***********************************************************************/

	class TestWindow : public GuiWindow
	{
	protected:
		List<Ptr<ScriptParagraph>>		script;
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
			MoveToScreenCenter();

			GetApplication()->InvokeAsync([=]()
			{
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					SetText(L"GuiUISrc Test Application (executing BuildDocumentFragments() ...)");
				});
				List<Ptr<DocumentFragment>> fragments;
				BuildDocumentFragments(L"..\\GacUISrcCodepackedTest\\Resources\\document.txt", fragments);
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					SetText(L"GuiUISrc Test Application (executing BuildScriptParagraphs() ...)");
				});
				BuildScriptParagraphs(fragments, script);
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					SetText(L"GacUISrc Test Application (ready)");
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