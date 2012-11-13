#include "GuiGraphicsLayoutProviderWindowsGDI.h"
#include "GuiGraphicsWindowsGDI.h"
#include <usp10.h>
#include <math.h>

#pragma comment(lib, "usp10.lib")

bool operator==(const SCRIPT_ITEM&, const SCRIPT_ITEM&){return false;}
bool operator!=(const SCRIPT_ITEM&, const SCRIPT_ITEM&){return false;}

bool operator==(const SCRIPT_VISATTR&, const SCRIPT_VISATTR&){return false;}
bool operator!=(const SCRIPT_VISATTR&, const SCRIPT_VISATTR&){return false;}

bool operator==(const GOFFSET&, const GOFFSET&){return false;}
bool operator!=(const GOFFSET&, const GOFFSET&){return false;}

bool operator==(const SCRIPT_LOGATTR&, const SCRIPT_LOGATTR&){return false;}
bool operator!=(const SCRIPT_LOGATTR&, const SCRIPT_LOGATTR&){return false;}

namespace vl
{
	namespace presentation
	{
		using namespace elements;
		using namespace collections;
		using namespace windows;
		using namespace regex;

		namespace elements_windows_gdi
		{

/***********************************************************************
Uniscribe Operations (UniscribeGlyphData)
***********************************************************************/

			struct UniscribeGlyphData
			{
				Array<WORD>					glyphs;
				Array<SCRIPT_VISATTR>		glyphVisattrs;
				Array<int>					glyphAdvances;
				Array<GOFFSET>				glyphOffsets;
				Array<WORD>					charCluster;
				ABC							runAbc;
				SCRIPT_ANALYSIS				sa;

				UniscribeGlyphData()
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
			
				bool BuildUniscribeData(WinDC* dc, SCRIPT_ITEM* scriptItem, SCRIPT_CACHE& scriptCache, const wchar_t* runText, int length)
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

/***********************************************************************
Uniscribe Operations (UniscribeRun)
***********************************************************************/

			class UniscribeRun : public Object
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

				FontProperties					fontStyle;
				Color							fontColor;
				Ptr<WinFont>					fontObject;

				SCRIPT_ITEM*					scriptItem;
				int								start;
				int								length;
				const wchar_t*					runText;

				SCRIPT_CACHE					scriptCache;
				Array<SCRIPT_LOGATTR>			charLogattrs;
				int								advance;
				UniscribeGlyphData				wholeGlyph;
				List<RunFragmentBounds>			fragmentBounds;

				UniscribeRun()
					:scriptItem(0)
					,start(0)
					,length(0)
					,scriptCache(0)
					,advance(0)
				{
				}

				~UniscribeRun()
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

					dc->SetFont(fontObject);
					if(!wholeGlyph.BuildUniscribeData(dc, scriptItem, scriptCache, runText, length))
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
					dc->SetFont(fontObject);
					dc->SetTextColor(RGB(fontColor.r, fontColor.g, fontColor.b));

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

/***********************************************************************
Uniscribe Operations (UniscribeLine)
***********************************************************************/

			struct UniscribeFragment
			{
				FontProperties					fontStyle;
				Color							fontColor;
				WString							text;
				Ptr<WinFont>					fontObject;

				WString GetFingerprint()
				{
					return fontStyle.fontFamily+L"#"
						+itow(fontStyle.size)+L"#"
						+(fontStyle.bold?L"B":L"N")+L"#"
						+(fontStyle.italic?L"I":L"N")+L"#"
						+(fontStyle.underline?L"U":L"N")+L"#"
						+(fontStyle.strikeline?L"S":L"N")+L"#"
						;
				}
			};

			class UniscribeLine : public Object
			{
			public:
				List<Ptr<UniscribeFragment>>	documentFragments;

				WString							lineText;
				Array<SCRIPT_ITEM>				scriptItems;
				List<Ptr<UniscribeRun>>			scriptRuns;
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

					FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments.Wrap())
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
									UniscribeFragment* fragment=0;
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

									Ptr<UniscribeRun> run=new UniscribeRun;
									run->fontStyle=fragment->fontStyle;
									run->fontColor=fragment->fontColor;
									run->fontObject=fragment->fontObject;
									run->scriptItem=scriptItem;
									run->start=currentStart;
									run->length=shortLength;
									run->runText=lineText.Buffer()+currentStart;
									scriptRuns.Add(run);
									currentStart+=shortLength;
								}
							}

							// for each run, generate shape information
							FOREACH(Ptr<UniscribeRun>, run, scriptRuns.Wrap())
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

				void Render(WinDC* dc, int offsetX, int offsetY)
				{
					FOREACH(Ptr<UniscribeRun>, run, scriptRuns.Wrap())
					{
						for(int i=0;i<run->fragmentBounds.Count();i++)
						{
							run->Render(dc, i, offsetX, offsetY);
						}
					}
				}
			};

/***********************************************************************
Uniscribe Operations (UniscribeParagraph)
***********************************************************************/

			class UniscribeParagraph : public Object
			{
			public:
				List<Ptr<UniscribeFragment>>	documentFragments;
				bool							built;

				List<Ptr<UniscribeLine>>		lines;
				int								lastAvailableWidth;
				Rect							bounds;

				UniscribeParagraph()
					:lastAvailableWidth(-1)
					,built(false)
				{
				}

				void BuildUniscribeData()
				{
					if(!built)
					{
						built=true;
						FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments.Wrap())
						{
							fragment->fontObject=0;
						}

						Dictionary<WString, Ptr<WinFont>> fonts;
						FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments.Wrap())
						{
							if(!fragment->fontObject)
							{
								WString fragmentFingerPrint=fragment->GetFingerprint();
								int index=fonts.Keys().IndexOf(fragmentFingerPrint);
								if(index==-1)
								{
									fragment->fontObject=new WinFont(fragment->fontStyle.fontFamily, -fragment->fontStyle.size, 0, 0, 0, (fragment->fontStyle.bold?FW_BOLD:FW_NORMAL), fragment->fontStyle.italic, fragment->fontStyle.underline, fragment->fontStyle.strikeline, true);
									fonts.Add(fragmentFingerPrint, fragment->fontObject);
								}
								else
								{
									fragment->fontObject=fonts.Values()[index];
								}
							}
						}

						lines.Clear();
						lastAvailableWidth=-1;
						{
							Regex regexLine(L"\r\n");
							Ptr<UniscribeLine> line;
							FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments.Wrap())
							{
								RegexMatch::List textLines;
								regexLine.Split(fragment->text, true, textLines);

								for(int i=0;i<textLines.Count();i++)
								{
									WString text=textLines[i]->Result().Value();
									if(i>0)
									{
										line=0;
									}
									if(!line)
									{
										line=new UniscribeLine;
										lines.Add(line);
									}

									Ptr<UniscribeFragment> runFragment=new UniscribeFragment;
									runFragment->fontColor=fragment->fontColor;
									runFragment->fontStyle=fragment->fontStyle;
									runFragment->text=text;
									runFragment->fontObject=fragment->fontObject;
									line->documentFragments.Add(runFragment);
								}
							}
						}

						HDC hdc=CreateCompatibleDC(NULL);
						WinProxyDC dc;
						dc.Initialize(hdc);
						int counter=0;
						FOREACH(Ptr<UniscribeLine>, line, lines.Wrap())
						{
							line->BuildUniscribeData(&dc);
						}
						DeleteDC(hdc);
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
					FOREACH(Ptr<UniscribeLine>, line, lines.Wrap())
					{
						if(line->scriptRuns.Count()==0)
						{
							// if this line doesn't contains any run, skip and render a blank line
							int height=line->documentFragments[0]->fontStyle.size;
							line->bounds=Rect(Point(cx, cy), Size(0, height));
							cy+=height;
						}
						else
						{
							FOREACH(Ptr<UniscribeRun>, run, line->scriptRuns.Wrap())
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
									UniscribeRun* run=line->scriptRuns[line->runVisualToLogical[i]].Obj();
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
										int size=line->scriptRuns[line->runVisualToLogical[i]]->fontStyle.size;
										if(maxHeight<size)
										{
											maxHeight=size;
										}
									}

									// render all runs inside this range
									for(int i=startRun;i<=lastRun && i<line->scriptRuns.Count();i++)
									{
										UniscribeRun* run=line->scriptRuns[line->runVisualToLogical[i]].Obj();
										int start=i==startRun?startRunOffset:0;
										int end=i==lastRun?lastRunOffset:run->length;
										int length=end-start;

										UniscribeRun::RunFragmentBounds fragmentBounds;
										fragmentBounds.start=start;
										fragmentBounds.length=length;
										fragmentBounds.bounds=Rect(
											Point(cx, cy+(maxHeight-run->fontStyle.size)), 
											Size(run->SumWidth(start, length), run->fontStyle.size)
											);
										run->fragmentBounds.Add(fragmentBounds);

										cx+=run->SumWidth(start, length);
									}

									cx=0;
									cy+=maxHeight;
								}

								startRun=lastRun;
								startRunOffset=lastRunOffset;
							}

							// calculate line bounds
							int minX=0;
							int minY=0;
							int maxX=0;
							int maxY=0;
							FOREACH(Ptr<UniscribeRun>, run, line->scriptRuns.Wrap())
							{
								FOREACH(UniscribeRun::RunFragmentBounds, fragmentBounds, run->fragmentBounds.Wrap())
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

					// calculate paragraph bounds
					int minX=0;
					int minY=0;
					int maxX=0;
					int maxY=0;
					FOREACH(Ptr<UniscribeLine>, line, lines.Wrap())
					{
						Rect bounds=line->bounds;
						if(minX>bounds.Left()) minX=bounds.Left();
						if(minY>bounds.Top()) minX=bounds.Top();
						if(maxX<bounds.Right()) maxX=bounds.Right();
						if(maxY<bounds.Bottom()) maxY=bounds.Bottom();
					}
					bounds=Rect(minX, minY, maxX, maxY);
				}

				void Render(WinDC* dc, int offsetX, int offsetY)
				{
					FOREACH(Ptr<UniscribeLine>, line, lines.Wrap())
					{
						line->Render(dc, offsetX, offsetY);
					}
				}
			};

/***********************************************************************
WindowsGDIParagraph
***********************************************************************/

			class WindowsGDIParagraph : public Object, public IGuiGraphicsParagraph
			{
			protected:
				IGuiGraphicsLayoutProvider*			provider;
				Ptr<UniscribeParagraph>				paragraph;
				IWindowsGDIRenderTarget*			renderTarget;

			public:
				WindowsGDIParagraph(IGuiGraphicsLayoutProvider* _provider, const WString& _text, IGuiGraphicsRenderTarget* _renderTarget)
					:provider(_provider)
					,renderTarget(dynamic_cast<IWindowsGDIRenderTarget*>(_renderTarget))
				{
					paragraph=new UniscribeParagraph;
					{
						Ptr<UniscribeFragment> fragment=new UniscribeFragment;
						fragment->text=_text;
						fragment->fontColor=Color(0, 0, 0);
						fragment->fontStyle=GetCurrentController()->ResourceService()->GetDefaultFont();
						paragraph->documentFragments.Add(fragment);
					}
				}

				~WindowsGDIParagraph()
				{
				}

				IGuiGraphicsLayoutProvider* GetProvider()override
				{
					return provider;
				}

				bool GetWrapLine()override
				{
					return true;
				}

				void SetWrapLine(bool value)override
				{
				}

				int GetMaxWidth()override
				{
					return paragraph->lastAvailableWidth;
				}

				void SetMaxWidth(int value)override
				{
					paragraph->BuildUniscribeData();
					paragraph->Layout(value);
				}

				bool SetFont(int start, int length, const WString& value)override
				{
					return true;
				}

				bool SetSize(int start, int length, int size)override
				{
					return true;
				}

				bool SetStyle(int start, int length, TextStyle value)override
				{
					return true;
				}

				bool SetColor(int start, int length, Color value)override
				{
					return true;
				}

				int GetHeight()override
				{
					paragraph->BuildUniscribeData();
					if(paragraph->lastAvailableWidth==-1)
					{
						paragraph->Layout(65536);
					}
					return paragraph->bounds.Height();
				}

				void Render(Rect bounds)override
				{
					paragraph->Render(renderTarget->GetDC(), bounds.Left(), bounds.Top());
				}
			};

/***********************************************************************
WindowsGDILayoutProvider
***********************************************************************/

			Ptr<IGuiGraphicsParagraph> WindowsGDILayoutProvider::CreateParagraph(const WString& text, IGuiGraphicsRenderTarget* renderTarget)
			{
				return new WindowsGDIParagraph(this, text, renderTarget);
			}
		}
	}
}