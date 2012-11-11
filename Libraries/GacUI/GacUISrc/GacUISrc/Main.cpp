#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\MemoryStream.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Regex\Regex.h"

#include "..\..\Source\GraphicsElement\WindowsDirect2D\GuiGraphicsWindowsDirect2D.h"
#include "..\..\Source\GraphicsElement\WindowsGDI\GuiGraphicsWindowsGDI.h"
#include <usp10.h>
#include <math.h>

#pragma comment(lib, "usp10.lib")

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
using namespace vl::presentation::windows;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result=SetupWindowsDirect2DRenderer();
#endif
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
		bool							paragraph;
		WString							font;
		bool							bold;
		Color							color;
		double							sizeInPoint;
		int								sizeInPixel;
		WString							text;
		Ptr<WinFont>					fontObject;
		ComPtr<IDWriteTextFormat>		textFormatObject;
		ComPtr<ID2D1SolidColorBrush>	textFormatBrush;

		DocumentFragment()
			:paragraph(false)
			,bold(false)
			,sizeInPoint(0)
			,sizeInPixel(0)
		{
		}

		DocumentFragment(Ptr<DocumentFragment> prototype, const WString& _text)
			:paragraph(prototype->paragraph)
			,font(prototype->font)
			,bold(prototype->bold)
			,color(prototype->color)
			,sizeInPoint(prototype->sizeInPoint)
			,sizeInPixel(prototype->sizeInPixel)
			,fontObject(prototype->fontObject)
			,text(_text)
		{
		}

		WString GetFingerPrint()
		{
			return font+L":"+(bold?L"B":L"N")+L":"+itow(sizeInPixel);
		}
	};

	class Document : public Object
	{
	public:
		List<Ptr<DocumentFragment>>		documentFragments;
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

	Ptr<Document> BuildDocumentFragments(const WString& fileName)
	{
		HDC dc=CreateCompatibleDC(NULL);
		int dpi=GetDeviceCaps(dc, LOGPIXELSY);
		DeleteDC(dc);
		Ptr<Document> document=new Document;

		WString rawDocument;
		{
			FileStream fileStream(fileName, FileStream::ReadOnly);
			BomDecoder decoder;
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
			document->documentFragments.Add(fragment);
			if(match->Groups()[L"tag"][0].Value()==L"p")
			{
				fragment->paragraph=true;
			}
			else
			{
				WString font=match->Groups()[L"font"][0].Value();
				WString bold=match->Groups()[L"bold"][0].Value();
				WString color=match->Groups()[L"color"][0].Value();
				WString size=match->Groups()[L"size"][0].Value();
				WString text=match->Groups()[L"text"][0].Value();

				fragment->font=font;
				fragment->bold=bold==L"true";
				fragment->sizeInPoint=wtof(size);
				fragment->sizeInPixel=(int)(fragment->sizeInPoint*dpi/72);
				fragment->color=ConvertColor(color);
				fragment->text=text;
			}
		}

		return document;
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
		SCRIPT_ANALYSIS				sa;

		GlyphData()
		{
			ClearUniscribeData(0, 0);
		}

		void ClearUniscribeData(int glyphCount, int length)
		{
			glyphs.Resize(glyphCount);
			glyphVisattrs.Resize(glyphCount);
			glyphAdvances.Resize(glyphCount);
			glyphOffsets.Resize(glyphCount);
			charCluster.Resize(length);
			memset(&runAbc, 0, sizeof(runAbc));
			memset(&sa, 0, sizeof(sa));
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
			sa=scriptItem->a;
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
						&sa,
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
					else if(hr==USP_E_SCRIPT_NOT_IN_FONT)
					{
						if(sa.eScript==SCRIPT_UNDEFINED)
						{
							goto BUILD_UNISCRIBE_DATA_FAILED;
						}
						else
						{
							sa.eScript=SCRIPT_UNDEFINED;
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
						&sa,
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
		struct RunFragmentBounds
		{
			int							start;
			int							length;
			Rect						bounds;

			bool operator==(const RunFragmentBounds&){return false;}
			bool operator!=(const RunFragmentBounds&){return false;}
		};

		DocumentFragment*				documentFragment;
		SCRIPT_ITEM*					scriptItem;
		int								start;
		int								length;
		const wchar_t*					runText;

		SCRIPT_CACHE					scriptCache;
		Array<SCRIPT_LOGATTR>			charLogattrs;
		int								advance;
		GlyphData						wholeGlyph;
		List<RunFragmentBounds>			fragmentBounds;

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
			advance=wholeGlyph.runAbc.abcA+wholeGlyph.runAbc.abcB+wholeGlyph.runAbc.abcC;

			return true;
BUILD_UNISCRIBE_DATA_FAILED:
			ClearUniscribeData();
			return false;
		}

		void SearchGlyphCluster(int charStart, int charLength, int& cluster, int& nextCluster)
		{
			cluster=wholeGlyph.charCluster[charStart];
			nextCluster
				=charStart+charLength==length
				?wholeGlyph.glyphs.Count()
				:wholeGlyph.charCluster[charStart+charLength];
		}

		int SumWidth(int charStart, int charLength)
		{
			int cluster=0;
			int nextCluster=0;
			SearchGlyphCluster(charStart, charLength, cluster, nextCluster);
			int width=0;
			for(int i=cluster;i<nextCluster;i++)
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
			for(int i=tempStart;i<=length;)
			{
				if(i==length || charLogattrs[i].fSoftBreak==TRUE)
				{
					if(width<=maxWidth || (firstRun && charLength==0))
					{
						charLength=i-tempStart;
						charAdvances=width;
					}
					else
					{
						return;
					}
				}
				if(i==length) break;

				int cluster=wholeGlyph.charCluster[i];
				int clusterLength=1;
				while(i+clusterLength<length)
				{
					if(wholeGlyph.charCluster[i+clusterLength]==cluster)
					{
						clusterLength++;
					}
					else
					{
						break;
					}
				}

				int nextCluster
					=i+clusterLength==length
					?wholeGlyph.glyphs.Count()
					:wholeGlyph.charCluster[i+clusterLength];
				for(int j=cluster;j<nextCluster;j++)
				{
					width+=wholeGlyph.glyphAdvances[j];
				}
				i+=clusterLength;
			}
		}

		void Render(WinDC* dc, int fragmentBoundsIndex, int offsetX, int offsetY)
		{
			Color color=documentFragment->color;
			dc->SetFont(documentFragment->fontObject);
			dc->SetTextColor(RGB(color.r, color.g, color.b));

			RunFragmentBounds fragment=fragmentBounds[fragmentBoundsIndex];
			RECT rect;
			rect.left=fragment.bounds.Left()+offsetX;
			rect.top=fragment.bounds.Top()+offsetY;
			rect.right=fragment.bounds.Right()+offsetX;
			rect.bottom=fragment.bounds.Bottom()+offsetY;
			
			int cluster=0;
			int nextCluster=0;
			SearchGlyphCluster(fragment.start, fragment.length, cluster, nextCluster);

			HRESULT hr=ScriptTextOut(
				dc->GetHandle(),
				&scriptCache,
				rect.left,
				rect.top,
				0,
				&rect,
				&wholeGlyph.sa,
				NULL,
				0,
				&wholeGlyph.glyphs[cluster],
				nextCluster-cluster,
				&wholeGlyph.glyphAdvances[cluster],
				NULL,
				&wholeGlyph.glyphOffsets[cluster]
				);
		}
	};

	class ScriptLine : public Object
	{
	public:
		List<Ptr<DocumentFragment>>		documentFragments;
		WString							lineText;

		Array<SCRIPT_ITEM>				scriptItems;
		List<Ptr<ScriptRun>>			scriptRuns;
		Array<int>						runVisualToLogical;
		Array<int>						runLogicalToVisual;
		Rect							bounds;

		void CLearUniscribeData()
		{
			scriptItems.Resize(0);
			scriptRuns.Clear();
			runVisualToLogical.Resize(0);
			runLogicalToVisual.Resize(0);
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
					SCRIPT_DIGITSUBSTITUTE sds={0};
					ScriptRecordDigitSubstitution(LOCALE_USER_DEFAULT, &sds);
					SCRIPT_CONTROL sc={0};
					SCRIPT_STATE ss={0};
					ScriptApplyDigitSubstitution(&sds, &sc, &ss);

					// itemize a line
					scriptItems.Resize(lineText.Length()+2);
					int scriptItemCount=0;
					HRESULT hr=ScriptItemize(
						lineText.Buffer(),
						lineText.Length(),
						scriptItems.Count()-1,
						&sc,
						&ss,
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

					// layout runs if there are RTL text
					Array<BYTE> levels(scriptRuns.Count());
					runVisualToLogical.Resize(scriptRuns.Count());
					runLogicalToVisual.Resize(scriptRuns.Count());
					for(int i=0;i<scriptRuns.Count();i++)
					{
						levels[i]=scriptRuns[i]->scriptItem->a.s.uBidiLevel;
					}
					ScriptLayout(levels.Count(), &levels[0], &runVisualToLogical[0], &runLogicalToVisual[0]);
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
		Rect							bounds;
	};

	class ScriptDocument : public Object
	{
	public:
		static const int				LineDistance=5;
		static const int				ParagraphDistance=10;

		List<Ptr<ScriptParagraph>>		paragraphs;
		int								lastAvailableWidth;
		Rect							bounds;

		ScriptDocument()
			:lastAvailableWidth(-1)
		{
		}

		void RebuildFontCache()
		{
			FOREACH(Ptr<ScriptParagraph>, paragraph, paragraphs.Wrap())
			{
				FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
				{
					FOREACH(Ptr<DocumentFragment>, fragment, line->documentFragments.Wrap())
					{
						fragment->fontObject=0;
					}
				}
			}

			Dictionary<WString, Ptr<WinFont>> fonts;
			FOREACH(Ptr<ScriptParagraph>, paragraph, paragraphs.Wrap())
			{
				FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
				{
					FOREACH(Ptr<DocumentFragment>, fragment, line->documentFragments.Wrap())
					{
						if(!fragment->fontObject)
						{
							WString fragmentFingerPrint=fragment->GetFingerPrint();
							int index=fonts.Keys().IndexOf(fragmentFingerPrint);
							if(index==-1)
							{
								fragment->fontObject=new WinFont(fragment->font, -fragment->sizeInPixel, 0, 0, 0, (fragment->bold?FW_BOLD:FW_NORMAL), false, false, false, true);
								fonts.Add(fragmentFingerPrint, fragment->fontObject);
							}
							else
							{
								fragment->fontObject=fonts.Values()[index];
							}
						}
					}
				}
			}
		}

		void Layout(int availableWidth)
		{
			if(lastAvailableWidth==availableWidth)
			{
				return;
			}
			lastAvailableWidth=availableWidth;

			int cx=0;
			int cy=0;
			FOREACH(Ptr<ScriptParagraph>, paragraph, paragraphs.Wrap())
			{
				FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
				{
					if(line->scriptRuns.Count()==0)
					{
						// if this line doesn't contains any run, skip and render a blank line
						int height=line->documentFragments[0]->sizeInPixel;
						line->bounds=Rect(Point(cx, cy), Size(0, height));
						cy+=height+LineDistance;
					}
					else
					{
						FOREACH(Ptr<ScriptRun>, run, line->scriptRuns.Wrap())
						{
							run->fragmentBounds.Clear();
						}

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
								ScriptRun* run=line->scriptRuns[line->runVisualToLogical[i]].Obj();
								run->SearchForLineBreak(lastRunOffset, availableWidth-currentWidth, firstRun, charLength, charAdvances);
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
									if(i==lastRun && lastRunOffset==0)
									{
										break;
									}
									int size=line->scriptRuns[line->runVisualToLogical[i]]->documentFragment->sizeInPixel;
									if(maxHeight<size)
									{
										maxHeight=size;
									}
								}

								// render all runs inside this range
								for(int i=startRun;i<=lastRun && i<line->scriptRuns.Count();i++)
								{
									ScriptRun* run=line->scriptRuns[line->runVisualToLogical[i]].Obj();
									int start=i==startRun?startRunOffset:0;
									int end=i==lastRun?lastRunOffset:run->length;
									int length=end-start;

									ScriptRun::RunFragmentBounds fragmentBounds;
									fragmentBounds.start=start;
									fragmentBounds.length=length;
									fragmentBounds.bounds=Rect(
										Point(cx, cy+(maxHeight-run->documentFragment->sizeInPixel)), 
										Size(run->SumWidth(start, length), run->documentFragment->sizeInPixel)
										);
									run->fragmentBounds.Add(fragmentBounds);

									cx+=run->SumWidth(start, length);
								}

								cx=0;
								cy+=maxHeight+LineDistance;
							}

							startRun=lastRun;
							startRunOffset=lastRunOffset;
						}

						// calculate line bounds
						int minX=0;
						int minY=0;
						int maxX=0;
						int maxY=0;
						FOREACH(Ptr<ScriptRun>, run, line->scriptRuns.Wrap())
						{
							FOREACH(ScriptRun::RunFragmentBounds, fragmentBounds, run->fragmentBounds.Wrap())
							{
								Rect bounds=fragmentBounds.bounds;
								if(minX>bounds.Left()) minX=bounds.Left();
								if(minY>bounds.Top()) minX=bounds.Top();
								if(maxX<bounds.Right()) maxX=bounds.Right();
								if(maxY<bounds.Bottom()) maxY=bounds.Bottom();
							}
						}
						line->bounds=Rect(minX, minY, maxX, maxY);
					}
				}
				cy+=ParagraphDistance;

				// calculate paragraph bounds
				int minX=0;
				int minY=0;
				int maxX=0;
				int maxY=0;
				FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
				{
					Rect bounds=line->bounds;
					if(minX>bounds.Left()) minX=bounds.Left();
					if(minY>bounds.Top()) minX=bounds.Top();
					if(maxX<bounds.Right()) maxX=bounds.Right();
					if(maxY<bounds.Bottom()) maxY=bounds.Bottom();
				}
				paragraph->bounds=Rect(minX, minY, maxX, maxY);
			}

			// calculate document bounds
			int minX=0;
			int minY=0;
			int maxX=0;
			int maxY=0;
			FOREACH(Ptr<ScriptParagraph>, paragraph, paragraphs.Wrap())
			{
				Rect bounds=paragraph->bounds;
				if(minX>bounds.Left()) minX=bounds.Left();
				if(minY>bounds.Top()) minX=bounds.Top();
				if(maxX<bounds.Right()) maxX=bounds.Right();
				if(maxY<bounds.Bottom()) maxY=bounds.Bottom();
			}
			bounds=Rect(minX, minY, maxX, maxY);
		}
	};

	Ptr<ScriptDocument> BuildScriptParagraphs(Ptr<Document> document)
	{
		Ptr<ScriptDocument> scriptDocument=new ScriptDocument;
		scriptDocument->paragraphs.Clear();
		Regex regex(L"\r\n");
		Ptr<ScriptParagraph> currentParagraph;
		Ptr<ScriptLine> currentLine;

		FOREACH(Ptr<DocumentFragment>, fragment, document->documentFragments.Wrap())
		{
			if(!currentParagraph)
			{
				currentParagraph=new ScriptParagraph;
				scriptDocument->paragraphs.Add(currentParagraph);
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
		scriptDocument->RebuildFontCache();
		int counter=0;
		FOREACH(Ptr<ScriptParagraph>, paragraph, scriptDocument->paragraphs.Wrap())
		{
			FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
			{
				line->BuildUniscribeData(&dc);
			}
			counter++;
		}
		DeleteDC(hdc);

		return scriptDocument;
	}

/***********************************************************************
ScriptDocumentViewGDI
***********************************************************************/

	class ScriptDocumentViewGDI : public GuiScrollView
	{
	protected:
		static const int				BorderMargin=10;
		Ptr<ScriptDocument>				document;
		Rect							visibleDocumentBounds;
		GuiGDIElement*					canvasElement;
		GuiBoundsComposition*			canvasComposition;
		Ptr<WinFont>					messageFont;

		Size QueryFullSize()
		{
			if(document)
			{
				Size size=document->bounds.GetSize();
				size.x+=BorderMargin*2;
				size.y+=BorderMargin*2;
				return size;
			}
			return Size(0, 0);
		}

		void UpdateView(Rect viewBounds)
		{
			if(document)
			{
				document->Layout(viewBounds.Width()-2*BorderMargin);
			}
			visibleDocumentBounds=viewBounds;
		}

		void canvasElement_Rendering(GuiGraphicsComposition* composition, GuiGDIElementEventArgs& arguments)
		{
			WinDC* dc=arguments.dc;
			Rect bounds=arguments.bounds;
			if(document)
			{
				int x=bounds.Left()+BorderMargin;
				int y=bounds.Top()+BorderMargin-visibleDocumentBounds.Top();
				int w=document->bounds.Width();
				int h=document->bounds.Height();

				canvasComposition->SetPreferredMinSize(Size(0, h+BorderMargin*2));

				FOREACH(Ptr<ScriptParagraph>, paragraph, document->paragraphs.Wrap())
				{
					if(paragraph->bounds.Top()>=visibleDocumentBounds.Bottom())
					{
						break;
					}
					else if(paragraph->bounds.Bottom()<=visibleDocumentBounds.Top())
					{
						continue;
					}
					FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
					{
						if(line->bounds.Top()>=visibleDocumentBounds.Bottom())
						{
							break;
						}
						else if(line->bounds.Bottom()<=visibleDocumentBounds.Top())
						{
							continue;
						}
						FOREACH(Ptr<ScriptRun>, run, line->scriptRuns.Wrap())
						{
							for(int i=0;i<run->fragmentBounds.Count();i++)
							{
								ScriptRun::RunFragmentBounds fragmentBounds=run->fragmentBounds[i];
								if(fragmentBounds.bounds.Top()>=visibleDocumentBounds.Bottom() || fragmentBounds.bounds.Bottom()<=visibleDocumentBounds.Top())
								{
									continue;
								}
								run->Render(dc, i, x, y);
							}
						}
					}
				}
			}
			else
			{
				dc->SetFont(messageFont);
				dc->SetTextColor(RGB(0, 0, 0));
				WString message=L"Initializing uniscribe data...";
				SIZE size=dc->MeasureString(message);
				int x=bounds.Left()+(bounds.Width()-size.cx)/2;
				int y=bounds.Top()+(bounds.Height()-size.cy)/2;
				dc->DrawString(x, y, message);
			}
		}

		void canvasComposition_BoundsChanged(GuiGraphicsComposition* composition, GuiEventArgs& arguments)
		{
			CalculateView();
		}
	public:
		ScriptDocumentViewGDI()
			:GuiScrollView(GetCurrentTheme()->CreateMultilineTextBoxStyle())
		{
			SetHorizontalAlwaysVisible(false);

			canvasElement=GuiGDIElement::Create();
			canvasElement->Rendering.AttachMethod(this, &ScriptDocumentViewGDI::canvasElement_Rendering);
			
			canvasComposition=new GuiBoundsComposition;
			canvasComposition->SetOwnedElement(canvasElement);
			canvasComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			canvasComposition->SetPreferredMinSize(Size(0, 100));
			canvasComposition->BoundsChanged.AttachMethod(this, &ScriptDocumentViewGDI::canvasComposition_BoundsChanged);
			GetContainerComposition()->AddChild(canvasComposition);

			messageFont=new WinFont(L"Segoe UI", -48, 0, 0, 0,FW_NORMAL, false, false, false, true);
		}

		void SetDocument(Ptr<Document> _document)
		{
			GetApplication()->InvokeAsync([=]()
			{
				Ptr<ScriptDocument> scriptDocument=BuildScriptParagraphs(_document);
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					document=scriptDocument;
					CalculateView();
				});
			});
		}
	};

/***********************************************************************
ScriptDocumentViewDirect2D
***********************************************************************/

	class DWriteDocument : public Object
	{
	public:
		Ptr<Document>					document;
		ComPtr<IDWriteTextLayout>		textLayout;

		void BuildColorCache(ID2D1RenderTarget* rt)
		{
			FOREACH(Ptr<DocumentFragment>, fragment, document->documentFragments.Wrap())
			{
				if(!fragment->paragraph)
				{
					fragment->textFormatBrush=0;
				}
			}

			Dictionary<Color, ComPtr<ID2D1SolidColorBrush>> brushes;
			FOREACH(Ptr<DocumentFragment>, fragment, document->documentFragments.Wrap())
			{
				if(!fragment->paragraph)
				{
					if(!fragment->textFormatBrush)
					{
						int index=brushes.Keys().IndexOf(fragment->color);
						if(index==-1)
						{
							ID2D1SolidColorBrush* brush=0;
							HRESULT hr=rt->CreateSolidColorBrush(
								D2D1::ColorF(fragment->color.r/255.0f, fragment->color.g/255.0f, fragment->color.b/255.0f),
								D2D1::BrushProperties(),
								&brush);
							if(!FAILED(hr))
							{
								fragment->textFormatBrush=brush;
							}
							brushes.Add(fragment->color, fragment->textFormatBrush);
						}
						else
						{
							fragment->textFormatBrush=brushes.Values()[index];
						}
					}
				}
			}

			SetTextColors();
		}

		void RebuildFontCache()
		{
			IDWriteFactory* factory=elements_windows_d2d::GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory();

			FOREACH(Ptr<DocumentFragment>, fragment, document->documentFragments.Wrap())
			{
				if(!fragment->paragraph)
				{
					fragment->textFormatObject=0;
				}
			}

			Dictionary<WString, ComPtr<IDWriteTextFormat>> fonts;
			FOREACH(Ptr<DocumentFragment>, fragment, document->documentFragments.Wrap())
			{
				if(!fragment->paragraph)
				{
					if(!fragment->textFormatObject)
					{
						WString fragmentFingerPrint=fragment->GetFingerPrint();
						int index=fonts.Keys().IndexOf(fragmentFingerPrint);
						if(index==-1)
						{
							IDWriteTextFormat* textFormat=0;
							HRESULT hr=factory->CreateTextFormat(
								fragment->font.Buffer(),
								NULL,
								(fragment->bold?DWRITE_FONT_WEIGHT_BOLD:DWRITE_FONT_WEIGHT_NORMAL),
								DWRITE_FONT_STYLE_NORMAL,
								DWRITE_FONT_STRETCH_NORMAL,
								(FLOAT)fragment->sizeInPixel,
								L"",
								&textFormat);
							if(!FAILED(hr))
							{
								fragment->textFormatObject=textFormat;
							}
							fonts.Add(fragmentFingerPrint, fragment->textFormatObject);
						}
						else
						{
							fragment->textFormatObject=fonts.Values()[index];
						}
					}
				}
			}
		}

		WString GetFullText()
		{
			MemoryStream stream;
			{
				StreamWriter writer(stream);
				FOREACH(Ptr<DocumentFragment>, fragment, document->documentFragments.Wrap())
				{
					if(fragment->paragraph)
					{
						writer.WriteString(L"\r\n\r\n");
					}
					else
					{
						writer.WriteString(fragment->text);
					}
				}
			}
			stream.SeekFromBegin(0);
			{
				StreamReader reader(stream);
				return reader.ReadToEnd();
			}
		}

		void BuildTextLayout()
		{
			WString fullText=GetFullText();
			RebuildFontCache();
			
			IDWriteFactory* factory=elements_windows_d2d::GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory();
			IDWriteTextFormat* textFormat=0;
			HRESULT hr=factory->CreateTextFormat(
				L"Segoe UI",
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				(FLOAT)10,
				L"",
				&textFormat);
			if(!FAILED(hr))
			{
				IDWriteTextLayout* rawTextLayout;
				HRESULT hr=factory->CreateTextLayout(
					fullText.Buffer(),
					fullText.Length(),
					textFormat,
					0,
					0,
					&rawTextLayout);
				if(!FAILED(hr))
				{
					rawTextLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
					textLayout=rawTextLayout;
					SetTextFormats();
				}
				textFormat->Release();
			}
		}

		void SetTextFormats()
		{
			DWRITE_TEXT_RANGE range={0};
			FOREACH(Ptr<DocumentFragment>, fragment, document->documentFragments.Wrap())
			{
				if(fragment->paragraph)
				{
					range.length=4;
				}
				else
				{
					range.length=fragment->text.Length();
				}
				textLayout->SetFontFamilyName(fragment->font.Buffer(), range);
				textLayout->SetFontSize((FLOAT)fragment->sizeInPixel, range);
				textLayout->SetFontWeight((fragment->bold?DWRITE_FONT_WEIGHT_BOLD:DWRITE_FONT_WEIGHT_NORMAL), range);
				range.startPosition+=range.length;
			}
		}

		void SetTextColors()
		{
			DWRITE_TEXT_RANGE range={0};
			FOREACH(Ptr<DocumentFragment>, fragment, document->documentFragments.Wrap())
			{
				if(fragment->paragraph)
				{
					range.length=4;
				}
				else
				{
					range.length=fragment->text.Length();
				}
				textLayout->SetDrawingEffect(fragment->textFormatBrush.Obj(), range);
				range.startPosition+=range.length;
			}
		}

		void Layout(int maxWidth)
		{
			textLayout->SetMaxWidth((FLOAT)maxWidth);
		}

		Size GetSize()
		{
			DWRITE_TEXT_METRICS metrics;
			HRESULT hr=textLayout->GetMetrics(&metrics);
			return Size(0, (int)metrics.height);
		}
	};

	class ScriptDocumentViewDirect2D : public GuiScrollView
	{
	protected:
		static const int				BorderMargin=10;
		Ptr<DWriteDocument>				document;
		bool							colorCache;
		Rect							visibleDocumentBounds;
		GuiDirect2DElement*				canvasElement;
		GuiBoundsComposition*			canvasComposition;

		ComPtr<IDWriteTextLayout>		message;
		ComPtr<ID2D1SolidColorBrush>	messageBrush;

		Size QueryFullSize()
		{
			if(document)
			{
				Size size=document->GetSize();
				return Size(size.x+2*BorderMargin, size.y+2*BorderMargin);
			}
			return Size(0, 0);
		}

		void UpdateView(Rect viewBounds)
		{
			if(document)
			{
				document->Layout(viewBounds.Width()-2*BorderMargin);
			}
			visibleDocumentBounds=viewBounds;
		}

		void canvasElement_Rendering(GuiGraphicsComposition* composition, GuiDirect2DElementEventArgs& arguments)
		{
			if(document && messageBrush)
			{
				if(!colorCache)
				{
					document->BuildColorCache(arguments.rt);
					colorCache=true;
				}
				Rect bounds=arguments.bounds;
				int x=bounds.Left()+BorderMargin;
				int y=bounds.Top()+BorderMargin-visibleDocumentBounds.Top();
				arguments.rt->DrawTextLayout(
					D2D1::Point2F((FLOAT)x, (FLOAT)y),
					document->textLayout.Obj(),
					messageBrush.Obj(),
					D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
					);
			}
			else if(message && messageBrush)
			{
				DWRITE_TEXT_METRICS metrics;
				HRESULT hr=message->GetMetrics(&metrics);
				if(!FAILED(hr))
				{
					int width=(int)ceil(metrics.widthIncludingTrailingWhitespace);
					int height=(int)ceil(metrics.height);
					int x=arguments.bounds.Left()+(arguments.bounds.Width()-width)/2;
					int y=arguments.bounds.Top()+(arguments.bounds.Height()-height)/2;

					arguments.rt->DrawTextLayout(
						D2D1::Point2F((FLOAT)x, (FLOAT)y),
						message.Obj(),
						messageBrush.Obj(),
						D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
						);
				}
			}
		}

		void canvasElement_BeforeRenderTargetChanged(GuiGraphicsComposition* composition, GuiDirect2DElementEventArgs& arguments)
		{
			message=0;
			messageBrush=0;
		}

		void canvasElement_AfterRenderTargetChanged(GuiGraphicsComposition* composition, GuiDirect2DElementEventArgs& arguments)
		{
			if(document)
			{
				document->BuildColorCache(arguments.rt);
				colorCache=true;
			}
			{
				IDWriteTextFormat* textFormat=0;
				HRESULT hr=arguments.factoryDWrite->CreateTextFormat(
					L"Segoe UI",
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					(FLOAT)42,
					L"",
					&textFormat);
				if(!FAILED(hr))
				{
					IDWriteTextLayout* textLayout;
					HRESULT hr=arguments.factoryDWrite->CreateTextLayout(
						L"Initializing IDWriteTextLayout...",
						wcslen(L"Initializing IDWriteTextLayout..."),
						textFormat,
						0,
						0,
						&textLayout);
					if(!FAILED(hr))
					{
						textLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
						message=textLayout;
					}
					textFormat->Release();
				}
			}
			{
				ID2D1SolidColorBrush* brush=0;
				HRESULT hr=arguments.rt->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &brush);
				if(!FAILED(hr))
				{
					messageBrush=brush;
				}
			}
		}

		void canvasComposition_BoundsChanged(GuiGraphicsComposition* composition, GuiEventArgs& arguments)
		{
			CalculateView();
		}
	public:
		ScriptDocumentViewDirect2D()
			:GuiScrollView(GetCurrentTheme()->CreateMultilineTextBoxStyle())
			,colorCache(false)
		{
			SetHorizontalAlwaysVisible(false);

			canvasElement=GuiDirect2DElement::Create();
			canvasElement->Rendering.AttachMethod(this, &ScriptDocumentViewDirect2D::canvasElement_Rendering);
			canvasElement->BeforeRenderTargetChanged.AttachMethod(this, &ScriptDocumentViewDirect2D::canvasElement_BeforeRenderTargetChanged);
			canvasElement->AfterRenderTargetChanged.AttachMethod(this, &ScriptDocumentViewDirect2D::canvasElement_AfterRenderTargetChanged);
			
			canvasComposition=new GuiBoundsComposition;
			canvasComposition->SetOwnedElement(canvasElement);
			canvasComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			canvasComposition->SetPreferredMinSize(Size(0, 100));
			canvasComposition->BoundsChanged.AttachMethod(this, &ScriptDocumentViewDirect2D::canvasComposition_BoundsChanged);
			GetContainerComposition()->AddChild(canvasComposition);
		}

		void SetDocument(Ptr<Document> _document)
		{
			document=new DWriteDocument;
			document->document=_document;
			document->BuildTextLayout();
			colorCache=false;
			CalculateView();
		}
	};

/***********************************************************************
TestWindow
***********************************************************************/

#ifdef GUI_GRAPHICS_RENDERER_GDI
	typedef ScriptDocumentViewGDI ScriptDocumentView;
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	typedef ScriptDocumentViewDirect2D ScriptDocumentView;
#endif

	class TestWindow : public GuiWindow
	{
	protected:
		ScriptDocumentView*				scriptDocumentView;

		void element_Rendering(GuiGraphicsComposition* composition, GuiGDIElementEventArgs& arguments)
		{
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
				scriptDocumentView=new ScriptDocumentView();
				scriptDocumentView->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				GetContainerComposition()->AddChild(scriptDocumentView->GetBoundsComposition());
			}
			GetApplication()->InvokeAsync([=]()
			{
				Ptr<Document> document=BuildDocumentFragments(L"..\\GacUISrcCodepackedTest\\Resources\\document2.txt");
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					scriptDocumentView->SetDocument(document);
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