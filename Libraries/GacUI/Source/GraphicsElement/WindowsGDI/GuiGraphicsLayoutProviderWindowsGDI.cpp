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
Uniscribe Operations (UniscribeFragment)
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

				UniscribeFragment*				documentFragment;

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
					:documentFragment(0)
					,scriptItem(0)
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
					Color fontColor=documentFragment->fontColor;
					dc->SetFont(documentFragment->fontObject);
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

				~UniscribeParagraph()
				{
					ClearUniscribeData();
				}

				void ClearUniscribeData()
				{
					FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments.Wrap())
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
						FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments.Wrap())
						{
							if(!fragment->fontObject)
							{
								WString fragmentFingerPrint=fragment->GetFingerprint();
								int index=fonts.Keys().IndexOf(fragmentFingerPrint);
								if(index==-1)
								{
									fragment->fontObject=GetWindowsGDIResourceManager()->CreateGdiFont(fragment->fontStyle);
									fonts.Add(fragmentFingerPrint, fragment->fontObject);
								}
								else
								{
									fragment->fontObject=fonts.Values()[index];
								}
							}
						}
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

									Ptr<UniscribeFragment> runFragment=fragment->Copy();
									line->documentFragments.Add(runFragment);
								}
							}
						}

						FOREACH(Ptr<UniscribeLine>, line, lines.Wrap())
						{
							line->BuildUniscribeData(dc);
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
										int size=line->scriptRuns[line->runVisualToLogical[i]]->documentFragment->fontStyle.size;
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
											Point(cx, cy+(maxHeight-run->documentFragment->fontStyle.size)), 
											Size(run->SumWidth(start, length), run->documentFragment->fontStyle.size)
											);
										run->fragmentBounds.Add(fragmentBounds);

										cx+=run->SumWidth(start, length);
									}

									cx=0;
									cy+=(int)(maxHeight*1.5);
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

				//-------------------------------------------------------------------------

				void SearchFragment(int start, int length, int& fs, int& ss, int& fe, int& se)
				{
					fs=-1;
					ss=-1;
					fe=-1;
					se=-1;
					int current=0;
					for(int i=0;i<documentFragments.Count();i++)
					{
						int fragmentLength=documentFragments[i]->text.Length();
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

				void CutFragment(int fs, int ss, int fe, int se, int& f1, int& f2)
				{
					f1=-1;
					f2=-1;
					if(fs==fe)
					{
						Ptr<UniscribeFragment> fragment=documentFragments[fs];
						int length=fragment->text.Length();
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
							int length=fragmentStart->text.Length();
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
							int length=fragmentEnd->text.Length();
							Ptr<UniscribeFragment> rightFragment=fragmentEnd->Copy();

							fragmentEnd->text=fragmentEnd->text.Sub(0, se);

							rightFragment->text=rightFragment->text.Sub(se, length-se);
							documentFragments.Insert(fe, rightFragment);
						}
					}
				}

				void SetFont(int start, int length, const WString& value)
				{
					int fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					CutFragment(fs, ss, fe, se, f1, f2);
					for(int i=f1;i<=f2;i++)
					{
						documentFragments[i]->fontStyle.fontFamily=value;
					}
				}

				void SetSize(int start, int length, int value)
				{
					int fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					CutFragment(fs, ss, fe, se, f1, f2);
					for(int i=f1;i<=f2;i++)
					{
						documentFragments[i]->fontStyle.size=value;
					}
				}

				void SetStyle(int start, int length, bool bold, bool italic, bool underline, bool strikeline)
				{
					int fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					CutFragment(fs, ss, fe, se, f1, f2);
					for(int i=f1;i<=f2;i++)
					{
						documentFragments[i]->fontStyle.bold=bold;
						documentFragments[i]->fontStyle.italic=italic;
						documentFragments[i]->fontStyle.underline=underline;
						documentFragments[i]->fontStyle.strikeline=strikeline;
					}
				}

				void SetColor(int start, int length, Color value)
				{
					int fs, ss, fe, se, f1, f2;
					SearchFragment(start, length, fs, ss, fe, se);
					CutFragment(fs, ss, fe, se, f1, f2);
					for(int i=f1;i<=f2;i++)
					{
						documentFragments[i]->fontColor=value;
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

				int GetMaxWidth()override
				{
					return paragraph->lastAvailableWidth;
				}

				void SetMaxWidth(int value)override
				{
					paragraph->BuildUniscribeData(renderTarget->GetDC());
					paragraph->Layout(value);
				}

				bool SetFont(int start, int length, const WString& value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						paragraph->SetFont(start, length, value);
						return true;
					}
					else
					{
						return false;
					}
				}

				bool SetSize(int start, int length, int value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						paragraph->SetSize(start, length, value);
						return true;
					}
					else
					{
						return false;
					}
				}

				bool SetStyle(int start, int length, TextStyle value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						paragraph->SetStyle(start, length, (value&Bold)!=0, (value&Italic)!=0, (value&Underline)!=0, (value&Strikeline)!=0);
						return true;
					}
					else
					{
						return false;
					}
				}

				bool SetColor(int start, int length, Color value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						paragraph->SetColor(start, length, value);
						return true;
					}
					else
					{
						return false;
					}
				}

				bool SetInlineObject(int start, int length, const InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value)override
				{
					return false;
				}

				bool ResetInlineObject(int start, int length)override
				{
					return false;
				}

				int GetHeight()override
				{
					paragraph->BuildUniscribeData(renderTarget->GetDC());
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