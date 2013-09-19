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
Uniscribe Operations (UniscribeFragment)
***********************************************************************/

			struct UniscribeFragment
			{
				FontProperties									fontStyle;
				Color											fontColor;
				WString											text;
				Ptr<WinFont>									fontObject;
				vint											interactionId;

				Ptr<IGuiGraphicsElement>						element;
				IGuiGraphicsParagraph::InlineObjectProperties	inlineObjectProperties;
				List<Ptr<UniscribeFragment>>					cachedTextFragment;

				UniscribeFragment()
					:interactionId(-1)
				{
				}

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

				Ptr<UniscribeFragment> Copy()
				{
					Ptr<UniscribeFragment> fragment=new UniscribeFragment;
					fragment->fontStyle=fontStyle;
					fragment->fontColor=fontColor;
					fragment->text=text;
					fragment->fontObject=fontObject;
					return fragment;
				}
			};

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

				void ClearUniscribeData(vint glyphCount, vint length)
				{
					glyphs.Resize(glyphCount);
					glyphVisattrs.Resize(glyphCount);
					glyphAdvances.Resize(glyphCount);
					glyphOffsets.Resize(glyphCount);
					charCluster.Resize(length);
					memset(&runAbc, 0, sizeof(runAbc));
					memset(&sa, 0, sizeof(sa));
				}
			
				bool BuildUniscribeData(WinDC* dc, SCRIPT_ITEM* scriptItem, SCRIPT_CACHE& scriptCache, const wchar_t* runText, vint length)
				{
					vint glyphCount=glyphs.Count();
					bool resizeGlyphData=false;
					if(glyphCount==0)
					{
						glyphCount=(vint)(1.5*length+16);
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
								(int)length,
								(int)glyphCount,
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
								(int)glyphCount,
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
					vint						start;
					vint						length;
					Rect						bounds;

					bool operator==(const RunFragmentBounds&){return false;}
					bool operator!=(const RunFragmentBounds&){return false;}
				};

				UniscribeFragment*				documentFragment;
				SCRIPT_ITEM*					scriptItem;
				vint							start;
				vint							length;
				const wchar_t*					runText;
				List<RunFragmentBounds>			fragmentBounds;

				UniscribeRun()
					:documentFragment(0)
					,scriptItem(0)
					,start(0)
					,length(0)
				{
				}

				~UniscribeRun()
				{
				}

				virtual bool					BuildUniscribeData(WinDC* dc)=0;
				virtual vint					SumWidth(vint charStart, vint charLength)=0;
				virtual vint					SumHeight()=0;
				virtual void					SearchForLineBreak(vint tempStart, vint maxWidth, bool firstRun, vint& charLength, vint& charAdvances)=0;
				virtual void					Render(WinDC* dc, vint fragmentBoundsIndex, vint offsetX, vint offsetY)=0;

				virtual bool HitTestPoint(Point point, vint& start, vint& length, vint& interactionId)
				{
					for(vint i=0;i<fragmentBounds.Count();i++)
					{
						if(fragmentBounds[i].bounds.Contains(point))
						{
							start=this->start;
							length=this->length;
							interactionId=this->documentFragment->interactionId;
							return true;
						}
					}
					return false;
				}
			};

/***********************************************************************
Uniscribe Operations (UniscribeTextRun)
***********************************************************************/

			class UniscribeTextRun : public UniscribeRun
			{
			public:
				SCRIPT_CACHE					scriptCache;
				Array<SCRIPT_LOGATTR>			charLogattrs;
				vint							advance;
				UniscribeGlyphData				wholeGlyph;

				UniscribeTextRun()
					:scriptCache(0)
					,advance(0)
				{
				}

				~UniscribeTextRun()
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

				void SearchGlyphCluster(vint charStart, vint charLength, vint& cluster, vint& nextCluster)
				{
					cluster=wholeGlyph.charCluster[charStart];
					if(scriptItem->a.fRTL)
					{
						nextCluster
							=charStart+charLength==length
							?0
							:wholeGlyph.charCluster[charStart+charLength];
					}
					else
					{
						nextCluster
							=charStart+charLength==length
							?wholeGlyph.glyphs.Count()
							:wholeGlyph.charCluster[charStart+charLength];
					}
				}

				bool BuildUniscribeData(WinDC* dc)override
				{
					ClearUniscribeData();
					{
						// generate break information
						charLogattrs.Resize(length);

						HRESULT hr=ScriptBreak(
							runText,
							(int)length,
							&scriptItem->a,
							&charLogattrs[0]
							);
						if(hr!=0)
						{
							goto BUILD_UNISCRIBE_DATA_FAILED;
						}
					}

					dc->SetFont(documentFragment->fontObject);
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

				vint SumWidth(vint charStart, vint charLength)override
				{
					vint cluster=0;
					vint nextCluster=0;
					SearchGlyphCluster(charStart, charLength, cluster, nextCluster);
					vint width=0;
					if(scriptItem->a.fRTL)
					{
						for(vint i=cluster;i>nextCluster;i--)
						{
							width+=wholeGlyph.glyphAdvances[i];
						}
					}
					else
					{
						for(vint i=cluster;i<nextCluster;i++)
						{
							width+=wholeGlyph.glyphAdvances[i];
						}
					}
					return width;
				}

				vint SumHeight()override
				{
					return documentFragment->fontStyle.size;
				}

				void SearchForLineBreak(vint tempStart, vint maxWidth, bool firstRun, vint& charLength, vint& charAdvances)override
				{
					vint width=0;
					charLength=0;
					charAdvances=0;
					for(vint i=tempStart;i<=length;)
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

						vint cluster=wholeGlyph.charCluster[i];
						vint clusterLength=1;
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

						if(scriptItem->a.fRTL)
						{
							vint nextCluster
								=i+clusterLength==length
								?0
								:wholeGlyph.charCluster[i+clusterLength];
							for(vint j=cluster;j>nextCluster;j--)
							{
								width+=wholeGlyph.glyphAdvances[j];
							}
						}
						else
						{
							vint nextCluster
								=i+clusterLength==length
								?wholeGlyph.glyphs.Count()
								:wholeGlyph.charCluster[i+clusterLength];
							for(vint j=cluster;j<nextCluster;j++)
							{
								width+=wholeGlyph.glyphAdvances[j];
							}
						}
						i+=clusterLength;
					}
				}

				void Render(WinDC* dc, vint fragmentBoundsIndex, vint offsetX, vint offsetY)override
				{
					Color fontColor=documentFragment->fontColor;
					dc->SetFont(documentFragment->fontObject);
					dc->SetTextColor(RGB(fontColor.r, fontColor.g, fontColor.b));

					RunFragmentBounds fragment=fragmentBounds[fragmentBoundsIndex];
					if(fragment.length==0) return;
					RECT rect;
					rect.left=(int)(fragment.bounds.Left()+offsetX);
					rect.top=(int)(fragment.bounds.Top()+offsetY);
					rect.right=(int)(fragment.bounds.Right()+offsetX);
					rect.bottom=(int)(fragment.bounds.Bottom()+offsetY);
			
					vint cluster=0;
					vint nextCluster=0;
					SearchGlyphCluster(fragment.start, fragment.length, cluster, nextCluster);

					vint clusterStart=0;
					vint clusterCount=0;
					if(scriptItem->a.fRTL)
					{
						clusterStart=nextCluster;
						clusterCount=cluster-nextCluster;
					}
					else
					{
						clusterStart=cluster;
						clusterCount=nextCluster-cluster;
					}

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
						&wholeGlyph.glyphs[clusterStart],
						(int)(clusterCount),
						&wholeGlyph.glyphAdvances[clusterStart],
						NULL,
						&wholeGlyph.glyphOffsets[clusterStart]
						);
				}
			};

/***********************************************************************
Uniscribe Operations (UniscribeElementRun)
***********************************************************************/

			class UniscribeElementRun : public UniscribeRun
			{
			public:
				Ptr<IGuiGraphicsElement>						element;
				IGuiGraphicsParagraph::InlineObjectProperties	properties;

				UniscribeElementRun()
				{
				}

				~UniscribeElementRun()
				{
				}

				bool BuildUniscribeData(WinDC* dc)override
				{
					return true;
				}

				vint SumWidth(vint charStart, vint charLength)override
				{
					return properties.size.x;
				}

				vint SumHeight()override
				{
					return properties.size.y;
				}

				void SearchForLineBreak(vint tempStart, vint maxWidth, bool firstRun, vint& charLength, vint& charAdvances)override
				{
					charLength=length-tempStart;
					charAdvances=properties.size.x;
				}

				void Render(WinDC* dc, vint fragmentBoundsIndex, vint offsetX, vint offsetY)override
				{
					Rect bounds=fragmentBounds[fragmentBoundsIndex].bounds;
					bounds.x1+=offsetX;
					bounds.x2+=offsetX;
					bounds.y1+=offsetY;
					bounds.y2+=offsetY;
					IGuiGraphicsRenderer* renderer=element->GetRenderer();
					if(renderer)
					{
						renderer->Render(bounds);
					}
				}
			};

/***********************************************************************
Uniscribe Operations (UniscribeLine)
***********************************************************************/

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
					vint current=0;
					FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments)
					{
						lineText+=fragment->text;
						current+=fragment->text.Length();
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
								(int)lineText.Length(),
								(int)(scriptItems.Count()-1),
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
							vint fragmentIndex=0;
							vint fragmentStart=0;
							for(vint i=0;i<scriptItems.Count()-1;i++)
							{
								SCRIPT_ITEM* scriptItem=&scriptItems[i];
								vint start=scriptItem[0].iCharPos;
								vint length=scriptItem[1].iCharPos-scriptItem[0].iCharPos;
								vint currentStart=start;

								while(currentStart<start+length)
								{
									UniscribeFragment* fragment=0;
									vint itemRemainLength=length-(currentStart-start);
									vint fragmentRemainLength=0;
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
									vint shortLength=itemRemainLength<fragmentRemainLength?itemRemainLength:fragmentRemainLength;
									bool skip=false;
									{
										vint elementCurrent=0;
										FOREACH(Ptr<UniscribeFragment>, elementFragment, documentFragments)
										{
											vint elementLength=elementFragment->text.Length();
											if(elementFragment->element)
											{
												if(elementCurrent<=currentStart && currentStart+shortLength<=elementCurrent+elementLength)
												{
													if(elementCurrent==currentStart)
													{
														Ptr<UniscribeElementRun> run=new UniscribeElementRun;
														run->documentFragment=fragment;
														run->scriptItem=scriptItem;
														run->start=currentStart;
														run->length=elementLength;
														run->runText=lineText.Buffer()+currentStart;
														run->element=elementFragment->element;
														run->properties=elementFragment->inlineObjectProperties;
														scriptRuns.Add(run);
													}
													skip=true;
													break;
												}
											}
											elementCurrent+=elementLength;
										}
									}
									if(!skip)
									{
										Ptr<UniscribeTextRun> run=new UniscribeTextRun;
										run->documentFragment=fragment;
										run->scriptItem=scriptItem;
										run->start=currentStart;
										run->length=shortLength;
										run->runText=lineText.Buffer()+currentStart;
										scriptRuns.Add(run);
									}
									currentStart+=shortLength;
								}
							}

							// for each run, generate shape information
							FOREACH(Ptr<UniscribeRun>, run, scriptRuns)
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
							for(vint i=0;i<scriptRuns.Count();i++)
							{
								levels[i]=scriptRuns[i]->scriptItem->a.s.uBidiLevel;
							}
							ScriptLayout((int)levels.Count(), &levels[0], &runVisualToLogical[0], &runLogicalToVisual[0]);
						}
					}
					return true;
		BUILD_UNISCRIBE_DATA_FAILED:
					CLearUniscribeData();
					return false;
				}

				void Render(WinDC* dc, vint offsetX, vint offsetY)
				{
					FOREACH(Ptr<UniscribeRun>, run, scriptRuns)
					{
						for(vint i=0;i<run->fragmentBounds.Count();i++)
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
				vint							lastAvailableWidth;
				Alignment						paragraphAlignment;
				Rect							bounds;

				UniscribeParagraph()
					:lastAvailableWidth(-1)
					,paragraphAlignment(Alignment::Left)
					,built(false)
				{
				}

				~UniscribeParagraph()
				{
					ClearUniscribeData();
				}

				void ClearUniscribeData()
				{
					FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments)
					{
						GetWindowsGDIResourceManager()->DestroyGdiFont(fragment->fontStyle);
						fragment->fontObject=0;
					}
					lines.Clear();
					lastAvailableWidth=-1;
				}

				void BuildUniscribeData(WinDC* dc)
				{
					if(!built)
					{
						built=true;
						ClearUniscribeData();
						Dictionary<WString, Ptr<WinFont>> fonts;
						FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments)
						{
							if(!fragment->fontObject)
							{
								WString fragmentFingerPrint=fragment->GetFingerprint();
								vint index=fonts.Keys().IndexOf(fragmentFingerPrint);
								if(index==-1)
								{
									fragment->fontObject=GetWindowsGDIResourceManager()->CreateGdiFont(fragment->fontStyle);
									fonts.Add(fragmentFingerPrint, fragment->fontObject);
								}
								else
								{
									fragment->fontObject=fonts.Values().Get(index);
								}
							}
						}
						{
							Regex regexLine(L"\r\n");
							Ptr<UniscribeLine> line;
							FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments)
							{
								if(fragment->element)
								{
									if(!line)
									{
										line=new UniscribeLine;
										lines.Add(line);
									}
									line->documentFragments.Add(fragment);
								}
								else
								{
									RegexMatch::List textLines;
									regexLine.Split(fragment->text, true, textLines);

									for(vint i=0;i<textLines.Count();i++)
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

										if(textLines.Count()==1)
										{
											line->documentFragments.Add(fragment);
										}
										else
										{
											Ptr<UniscribeFragment> runFragment=fragment->Copy();
											runFragment->text=text;
											line->documentFragments.Add(runFragment);
										}
									}
								}
							}
						}

						FOREACH(Ptr<UniscribeLine>, line, lines)
						{
							line->BuildUniscribeData(dc);
						}
					}
				}

				void Layout(vint availableWidth, Alignment alignment)
				{
					if(lastAvailableWidth==availableWidth && paragraphAlignment==alignment)
					{
						return;
					}
					lastAvailableWidth=availableWidth;
					paragraphAlignment=alignment;

					vint cx=0;
					vint cy=0;
					FOREACH(Ptr<UniscribeLine>, line, lines)
					{
						if(line->scriptRuns.Count()==0)
						{
							// if this line doesn't contains any run, skip and render a blank line
							vint height=line->documentFragments[0]->fontStyle.size;
							line->bounds=Rect(Point(cx, cy), Size(0, height));
							cy+=height;
						}
						else
						{
							FOREACH(Ptr<UniscribeRun>, run, line->scriptRuns)
							{
								run->fragmentBounds.Clear();
							}

							// render this line into lines with auto line wrapping
							vint startRun=0;
							vint startRunOffset=0;
							vint lastRun=0;
							vint lastRunOffset=0;
							vint currentWidth=0;

							while(startRun<line->scriptRuns.Count())
							{
								vint currentWidth=0;
								bool firstRun=true;
								// search for a range to fit in the given width
								for(vint i=startRun;i<line->scriptRuns.Count();i++)
								{
									vint charLength=0;
									vint charAdvances=0;
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
									vint maxHeight=0;
									for(vint i=startRun;i<=lastRun && i<line->scriptRuns.Count();i++)
									{
										if(i==lastRun && lastRunOffset==0)
										{
											break;
										}
										vint size=line->scriptRuns[line->runVisualToLogical[i]]->SumHeight();
										if(maxHeight<size)
										{
											maxHeight=size;
										}
									}

									// render all runs inside this range
									vint startRunFragmentCount=-1;
									for(vint i=startRun;i<=lastRun && i<line->scriptRuns.Count();i++)
									{
										UniscribeRun* run=line->scriptRuns[line->runVisualToLogical[i]].Obj();
										vint start=i==startRun?startRunOffset:0;
										vint end=i==lastRun?lastRunOffset:run->length;
										vint length=end-start;

										if(startRunFragmentCount==-1)
										{
											startRunFragmentCount=run->fragmentBounds.Count();
										}

										UniscribeRun::RunFragmentBounds fragmentBounds;
										if(run->scriptItem->a.fRTL)
										{
											fragmentBounds.start=run->length-start-length;
										}
										else
										{
											fragmentBounds.start=start;
										}
										fragmentBounds.length=length;
										fragmentBounds.bounds=Rect(
											Point(cx, cy+maxHeight-run->SumHeight()),
											Size(run->SumWidth(start, length), run->SumHeight())
											);
										run->fragmentBounds.Add(fragmentBounds);

										cx+=run->SumWidth(start, length);
									}

									vint cxOffset=0;
									switch(alignment)
									{
									case Alignment::Center:
										cxOffset=(availableWidth-cx)/2;
										break;
									case Alignment::Right:
										cxOffset=availableWidth-cx;
										break;
									}
									if(cxOffset!=0)
									{
										for(vint i=startRun;i<=lastRun && i<line->scriptRuns.Count();i++)
										{
											UniscribeRun* run=line->scriptRuns[line->runVisualToLogical[i]].Obj();
											for(vint j=(i==startRun?startRunFragmentCount:0);j<run->fragmentBounds.Count();j++)
											{
												UniscribeRun::RunFragmentBounds& fragmentBounds=run->fragmentBounds[j];
												fragmentBounds.bounds.x1+=cxOffset;
												fragmentBounds.bounds.x2+=cxOffset;
											}
										}
									}

									cx=0;
									cy+=(vint)(maxHeight*1.5);
								}

								startRun=lastRun;
								startRunOffset=lastRunOffset;
							}

							// calculate line bounds
							vint minX=0;
							vint minY=0;
							vint maxX=0;
							vint maxY=0;
							FOREACH(Ptr<UniscribeRun>, run, line->scriptRuns)
							{
								FOREACH(UniscribeRun::RunFragmentBounds, fragmentBounds, run->fragmentBounds)
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
					vint minX=0;
					vint minY=0;
					vint maxX=0;
					vint maxY=0;
					FOREACH(Ptr<UniscribeLine>, line, lines)
					{
						Rect bounds=line->bounds;
						if(minX>bounds.Left()) minX=bounds.Left();
						if(minY>bounds.Top()) minX=bounds.Top();
						if(maxX<bounds.Right()) maxX=bounds.Right();
						if(maxY<bounds.Bottom()) maxY=bounds.Bottom();
					}
					bounds=Rect(minX, minY, maxX, maxY);
				}

				void Render(WinDC* dc, vint offsetX, vint offsetY)
				{
					FOREACH(Ptr<UniscribeLine>, line, lines)
					{
						line->Render(dc, offsetX, offsetY);
					}
				}

				//-------------------------------------------------------------------------

				void SearchFragment(vint start, vint length, vint& fs, vint& ss, vint& fe, vint& se)
				{
					fs=-1;
					ss=-1;
					fe=-1;
					se=-1;
					vint current=0;
					for(vint i=0;i<documentFragments.Count();i++)
					{
						vint fragmentLength=documentFragments[i]->text.Length();
						if(current<=start && start<current+fragmentLength)
						{
							fs=i;
							ss=start-current;
						}
						if(current<=start+length && start+length<=current+fragmentLength)
						{
							fe=i;
							se=start+length-current;
						}
						if(fs!=-1 && fe!=-1)
						{
							break;
						}
						current+=fragmentLength;
					}
				}

				bool CutFragment(vint fs, vint ss, vint fe, vint se, vint& f1, vint& f2)
				{
					f1=-1;
					f2=-1;
					if(fs==fe)
					{
						Ptr<UniscribeFragment> fragment=documentFragments[fs];
						if(fragment->element)
						{
							if(ss==0 && se==fragment->text.Length())
							{
								f1=f2=fs;
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					for(vint i=fs;i<=fe;i++)
					{
						if(documentFragments[i]->element)
						{
							return false;
						}
					}
					if(fs==fe)
					{
						Ptr<UniscribeFragment> fragment=documentFragments[fs];
						vint length=fragment->text.Length();
						if(ss==0)
						{
							if(se==length)
							{
								f1=f2=fs;
							}
							else
							{
								f1=f2=fs;
								Ptr<UniscribeFragment> rightFragment=fragment->Copy();

								fragment->text=fragment->text.Sub(0, se);

								rightFragment->text=rightFragment->text.Sub(se, length-se);
								documentFragments.Insert(fs+1, rightFragment);
							}
						}
						else
						{
							if(se==length)
							{
								f1=fs+1;
								f2=fs+1;
								Ptr<UniscribeFragment> leftFragment=fragment->Copy();

								leftFragment->text=leftFragment->text.Sub(0, ss);
								documentFragments.Insert(fs, leftFragment);
								
								fragment->text=fragment->text.Sub(ss, length-ss);
							}
							else
							{
								f1=fs+1;
								f2=fs+1;
								Ptr<UniscribeFragment> leftFragment=fragment->Copy();
								Ptr<UniscribeFragment> rightFragment=fragment->Copy();

								leftFragment->text=leftFragment->text.Sub(0, ss);
								documentFragments.Insert(fs, leftFragment);
								
								fragment->text=fragment->text.Sub(ss, se-ss);

								rightFragment->text=rightFragment->text.Sub(se, length-se);
								documentFragments.Insert(fs+2, rightFragment);
							}
						}
					}
					else
					{
						Ptr<UniscribeFragment> fragmentStart=documentFragments[fs];
						Ptr<UniscribeFragment> fragmentEnd=documentFragments[fe];
						if(ss==0)
						{
							f1=fs;
						}
						else
						{
							f1=fs+1;
							fe++;
							vint length=fragmentStart->text.Length();
							Ptr<UniscribeFragment> leftFragment=fragmentStart->Copy();

							leftFragment->text=leftFragment->text.Sub(0, ss);
							documentFragments.Insert(fs, leftFragment);
								
							fragmentStart->text=fragmentStart->text.Sub(ss, length-ss);
						}
						if(se==fragmentEnd->text.Length())
						{
							f2=fe;
						}
						else
						{
							f2=fe;
							fe++;
							vint length=fragmentEnd->text.Length();
							Ptr<UniscribeFragment> rightFragment=fragmentEnd->Copy();

							fragmentEnd->text=fragmentEnd->text.Sub(0, se);

							rightFragment->text=rightFragment->text.Sub(se, length-se);
							documentFragments.Insert(fe, rightFragment);
						}
					}
					return true;
				}

				bool SetFont(vint start, vint length, const WString& value)
				{
					vint fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					if(CutFragment(fs, ss, fe, se, f1, f2))
					{
						for(vint i=f1;i<=f2;i++)
						{
							documentFragments[i]->fontStyle.fontFamily=value;
						}
						built=false;
						return true;
					}
					else
					{
						return false;
					}
				}

				bool SetSize(vint start, vint length, vint value)
				{
					vint fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					if(CutFragment(fs, ss, fe, se, f1, f2))
					{
						for(vint i=f1;i<=f2;i++)
						{
							documentFragments[i]->fontStyle.size=value;
						}
						built=false;
						return true;
					}
					else
					{
						return false;
					}
				}

				bool SetStyle(vint start, vint length, bool bold, bool italic, bool underline, bool strikeline)
				{
					vint fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					if(CutFragment(fs, ss, fe, se, f1, f2))
					{
						for(vint i=f1;i<=f2;i++)
						{
							documentFragments[i]->fontStyle.bold=bold;
							documentFragments[i]->fontStyle.italic=italic;
							documentFragments[i]->fontStyle.underline=underline;
							documentFragments[i]->fontStyle.strikeline=strikeline;
						}
						built=false;
						return true;
					}
					else
					{
						return false;
					}
				}

				bool SetColor(vint start, vint length, Color value)
				{
					vint fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					if(CutFragment(fs, ss, fe, se, f1, f2))
					{
						for(vint i=f1;i<=f2;i++)
						{
							documentFragments[i]->fontColor=value;
						}
						built=false;
						return true;
					}
					else
					{
						return false;
					}
				}

				bool SetInlineObject(vint start, vint length, const IGuiGraphicsParagraph::InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value)
				{
					vint fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					if(CutFragment(fs, ss, fe, se, f1, f2))
					{
						Ptr<UniscribeFragment> elementFragment=new UniscribeFragment;
						for(vint i=f1;i<=f2;i++)
						{
							elementFragment->text+=documentFragments[f1]->text;
							elementFragment->cachedTextFragment.Add(documentFragments[f1]);
							documentFragments.RemoveAt(f1);
						}
						elementFragment->inlineObjectProperties=properties;
						elementFragment->element=value;
						documentFragments.Insert(f1, elementFragment);
						built=false;
						return true;
					}
					else
					{
						return false;
					}
				}

				Ptr<IGuiGraphicsElement> ResetInlineObject(vint start, vint length)
				{
					vint fs, ss, fe, se;
					SearchFragment(start, length, fs, ss, fe, se);
					Ptr<UniscribeFragment> fragment=documentFragments[fs];
					if(fs==fe && ss==0 && se==fragment->text.Length() && fragment->element)
					{
						documentFragments.RemoveAt(fs);
						for(vint i=0;i<fragment->cachedTextFragment.Count();i++)
						{
							documentFragments.Insert(fs+i, fragment->cachedTextFragment[i]);
						}
						built=false;
						return fragment->element;
					}
					return 0;
				}

				bool SetInteractionId(vint start, vint length, vint value)
				{
					vint fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					if(CutFragment(fs, ss, fe, se, f1, f2))
					{
						for(vint i=f1;i<=f2;i++)
						{
							documentFragments[i]->interactionId=value;
						}
						built=false;
						return true;
					}
					else
					{
						return false;
					}
				}

				bool HitTestPoint(Point point, vint& start, vint& length, vint& interactionId)
				{
					start=-1;
					length=0;
					interactionId=IGuiGraphicsParagraph::NullInteractionId;

					if(bounds.Contains(point))
					{
						FOREACH(Ptr<UniscribeLine>, line, lines)
						{
							if(point.y<line->bounds.y1)
							{
								continue;
							}
							else if(line->bounds.y2<=point.y)
							{
								break;
							}
							else if(line->bounds.Contains(point))
							{
								FOREACH(Ptr<UniscribeRun>, run, line->scriptRuns)
								{
									if(run->HitTestPoint(point, start, length, interactionId))
									{
										return true;
									}
								}
							}
						}
					}
					return false;
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
				WString								text;
				IWindowsGDIRenderTarget*			renderTarget;

			public:
				WindowsGDIParagraph(IGuiGraphicsLayoutProvider* _provider, const WString& _text, IGuiGraphicsRenderTarget* _renderTarget)
					:provider(_provider)
					,text(_text)
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

				IGuiGraphicsRenderTarget* GetRenderTarget()override
				{
					return renderTarget;
				}

				bool GetWrapLine()override
				{
					return true;
				}

				void SetWrapLine(bool value)override
				{
				}

				vint GetMaxWidth()override
				{
					return paragraph->lastAvailableWidth;
				}

				void SetMaxWidth(vint value)override
				{
					paragraph->BuildUniscribeData(renderTarget->GetDC());
					paragraph->Layout(value, paragraph->paragraphAlignment);
				}

				Alignment GetParagraphAlignment()override
				{
					return paragraph->paragraphAlignment;
				}

				void SetParagraphAlignment(Alignment value)override
				{
					paragraph->BuildUniscribeData(renderTarget->GetDC());
					paragraph->Layout(paragraph->lastAvailableWidth, value);
				}

				bool SetFont(vint start, vint length, const WString& value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetFont(start, length, value);
					}
					else
					{
						return false;
					}
				}

				bool SetSize(vint start, vint length, vint value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetSize(start, length, value);
					}
					else
					{
						return false;
					}
				}

				bool SetStyle(vint start, vint length, TextStyle value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetStyle(start, length, (value&Bold)!=0, (value&Italic)!=0, (value&Underline)!=0, (value&Strikeline)!=0);
					}
					else
					{
						return false;
					}
				}

				bool SetColor(vint start, vint length, Color value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetColor(start, length, value);
					}
					else
					{
						return false;
					}
				}

				bool SetInlineObject(vint start, vint length, const InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						if(paragraph->SetInlineObject(start, length, properties, value))
						{
							IGuiGraphicsRenderer* renderer=value->GetRenderer();
							if(renderer)
							{
								renderer->SetRenderTarget(renderTarget);
							}
							return true;
						}
					}
					return false;
				}

				bool ResetInlineObject(vint start, vint length)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						if(Ptr<IGuiGraphicsElement> element=paragraph->ResetInlineObject(start, length))
						{
							IGuiGraphicsRenderer* renderer=element->GetRenderer();
							if(renderer)
							{
								renderer->SetRenderTarget(0);
							}
							return true;
						}
					}
					return false;
				}

				bool SetInteractionId(vint start, vint length, vint value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetInteractionId(start, length, value);
					}
					else
					{
						return false;
					}
				}

				bool HitTestPoint(Point point, vint& start, vint& length, vint& interactionId)override
				{
					return paragraph->HitTestPoint(point, start, length, interactionId);
				}

				vint GetHeight()override
				{
					paragraph->BuildUniscribeData(renderTarget->GetDC());
					if(paragraph->lastAvailableWidth==-1)
					{
						paragraph->Layout(65536, paragraph->paragraphAlignment);
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