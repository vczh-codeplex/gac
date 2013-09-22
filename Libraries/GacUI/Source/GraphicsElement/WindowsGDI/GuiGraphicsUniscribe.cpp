#include "GuiGraphicsUniscribe.h"

#pragma comment(lib, "usp10.lib")

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{
			using namespace regex;

/***********************************************************************
UniscribeFragment
***********************************************************************/

			UniscribeFragment::UniscribeFragment()
				:interactionId(-1)
			{
			}

			WString UniscribeFragment::GetFingerprint()
			{
				return fontStyle.fontFamily+L"#"
					+itow(fontStyle.size)+L"#"
					+(fontStyle.bold?L"B":L"N")+L"#"
					+(fontStyle.italic?L"I":L"N")+L"#"
					+(fontStyle.underline?L"U":L"N")+L"#"
					+(fontStyle.strikeline?L"S":L"N")+L"#"
					;
			}

			Ptr<UniscribeFragment> UniscribeFragment::Copy()
			{
				Ptr<UniscribeFragment> fragment=new UniscribeFragment;
				fragment->fontStyle=fontStyle;
				fragment->fontColor=fontColor;
				fragment->text=text;
				fragment->fontObject=fontObject;
				return fragment;
			}

/***********************************************************************
UniscribeGlyphData
***********************************************************************/

			UniscribeGlyphData::UniscribeGlyphData()
			{
				ClearUniscribeData(0, 0);
			}

			void UniscribeGlyphData::ClearUniscribeData(vint glyphCount, vint length)
			{
				glyphs.Resize(glyphCount);
				glyphVisattrs.Resize(glyphCount);
				glyphAdvances.Resize(glyphCount);
				glyphOffsets.Resize(glyphCount);
				charCluster.Resize(length);
				memset(&runAbc, 0, sizeof(runAbc));
				memset(&sa, 0, sizeof(sa));
			}
			
			bool UniscribeGlyphData::BuildUniscribeData(WinDC* dc, SCRIPT_ITEM* scriptItem, SCRIPT_CACHE& scriptCache, const wchar_t* runText, vint length, List<vint>& breakings, List<bool>& breakingAvailabilities)
			{
				vint glyphCount=glyphs.Count();
				bool resizeGlyphData=false;
				if(glyphCount==0)
				{
					glyphCount=(vint)(1.5*length+16);
					resizeGlyphData=true;
				}
				sa=scriptItem->a;
				WinDC* dcParameter=0;
				{
					// generate shape information
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
					SCRIPT_FONTPROPERTIES fp;
					memset(&fp, 0, sizeof(fp));
					fp.cBytes=sizeof(fp);
					HRESULT hr=ScriptGetFontProperties(
							(dcParameter?dcParameter->GetHandle():NULL),
							&scriptCache,
							&fp
							);
					WORD invalidGlyph=fp.wgDefault;
					if(hr!=S_OK)
					{
						invalidGlyph=0;
					}

					// generate breaking information
					breakings.Add(0);
					vint charIndex=0;
					bool lastGlyphAvailable=false;
					while(charIndex<length)
					{
						vint glyphIndex=charCluster[charIndex];
						vint nextCharIndex=charIndex;
						while(nextCharIndex<length && charCluster[nextCharIndex]==glyphIndex)
						{
							nextCharIndex++;
						}

						vint glyphCount=0;
						if(nextCharIndex==length)
						{
							glyphCount=glyphs.Count()-glyphIndex;
						}
						else
						{
							glyphCount=charCluster[nextCharIndex]-glyphIndex;
						}

						if(scriptItem->a.fRTL)
						{
							glyphCount=-glyphCount;
							glyphIndex-=glyphCount-1;
						}

						bool available=true;
						for(vint i=0;i<glyphCount;i++)
						{
							if(glyphs[i+glyphIndex]==invalidGlyph)
							{
								available=false;
							}
						}

						if(charIndex==0)
						{
							lastGlyphAvailable=available;
							breakingAvailabilities.Add(available);
						}
						else if(lastGlyphAvailable!=available)
						{
							breakings.Add(charIndex);
							lastGlyphAvailable=available;
							breakingAvailabilities.Add(available);
						}

						charIndex=nextCharIndex;
					}
				}

				if(breakings.Count()==1)
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

			void UniscribeGlyphData::BuildUniscribeData(SCRIPT_STRING_ANALYSIS ssa, SCRIPT_ITEM* scriptItem)
			{
				int length=*ScriptString_pcOutChars(ssa);
				ClearUniscribeData(length, length);
				sa=scriptItem->a;
				{
					const SIZE* size=ScriptString_pSize(ssa);
					runAbc.abcA=0;
					runAbc.abcB=size->cx;
					runAbc.abcC=0;
				}
				memset(&glyphs[0], 0, sizeof(glyphs[0])*glyphs.Count());
				memset(&glyphVisattrs[0], 0, sizeof(glyphVisattrs[0])*glyphVisattrs.Count());
				memset(&glyphAdvances[0], 0, sizeof(glyphAdvances[0])*glyphAdvances.Count());
				memset(&glyphOffsets[0], 0, sizeof(glyphOffsets[0])*glyphOffsets.Count());
				memset(&charCluster[0], 0, sizeof(charCluster[0])*charCluster.Count());

				for(vint i=0;i<charCluster.Count();i++)
				{
					charCluster[i]=i;
				}
				ScriptStringGetLogicalWidths(ssa, &glyphAdvances[0]);
				if(sa.fRTL)
				{
					for(vint i=0;i<charCluster.Count()/2;i++)
					{
						vint j=charCluster.Count()-1-i;
						{
							WORD t=charCluster[i];
							charCluster[i]=charCluster[j];
							charCluster[j]=t;
						}
						{
							int t=glyphAdvances[i];
							glyphAdvances[i]=glyphAdvances[j];
							glyphAdvances[j]=t;
						}
					}
				}
			}

/***********************************************************************
UniscribeItem
***********************************************************************/

			UniscribeItem::UniscribeItem()
				:start(0)
				,length(0)
				,itemText(0)
			{
			}

			UniscribeItem::~UniscribeItem()
			{
			}

			void UniscribeItem::ClearUniscribeData()
			{
				charLogattrs.Resize(0);
			}

			bool UniscribeItem::BuildUniscribeData()
			{
				// generate break information
				charLogattrs.Resize(length);

				HRESULT hr=ScriptBreak(
					itemText,
					(int)length,
					&scriptItem.a,
					&charLogattrs[0]
					);
				if(hr!=0)
				{
					goto BUILD_UNISCRIBE_DATA_FAILED;
				}

				return true;
	BUILD_UNISCRIBE_DATA_FAILED:
				ClearUniscribeData();
				return false;
			}

			bool UniscribeItem::IsRightToLeft()
			{
				return scriptItem.a.fRTL;
			}

/***********************************************************************
UniscribeRun
***********************************************************************/

			UniscribeRun::UniscribeRun()
				:documentFragment(0)
				,scriptItem(0)
				,start(0)
				,length(0)
				,runText(0)
			{
			}

			UniscribeRun::~UniscribeRun()
			{
			}

			bool UniscribeRun::HitTestPoint(Point point, vint& start, vint& length, vint& interactionId)
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

/***********************************************************************
UniscribeTextRun
***********************************************************************/

			UniscribeTextRun::UniscribeTextRun()
				:scriptCache(0)
				,advance(0)
				,ssa(0)
			{
			}

			UniscribeTextRun::~UniscribeTextRun()
			{
				ClearUniscribeData();
			}

			void UniscribeTextRun::ClearUniscribeData()
			{
				if(scriptCache)
				{
					ScriptFreeCache(&scriptCache);
					scriptCache=0;
				}
				if(ssa)
				{
					ScriptStringFree(&ssa);
					ssa=0;
				}
				advance=0;
				wholeGlyph.ClearUniscribeData(0, 0);
			}

			void UniscribeTextRun::SearchGlyphCluster(vint charStart, vint charLength, vint& cluster, vint& nextCluster)
			{
				cluster=wholeGlyph.charCluster[charStart];
				if(scriptItem->IsRightToLeft())
				{
					nextCluster
						=charStart+charLength==length
						?-1
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

			bool UniscribeTextRun::BuildUniscribeData(WinDC* dc, List<vint>& breakings)
			{
				ClearUniscribeData();

				dc->SetFont(documentFragment->fontObject);
				List<bool> breakingAvailabilities;
				if(!wholeGlyph.BuildUniscribeData(dc, &scriptItem->scriptItem, scriptCache, runText, length, breakings, breakingAvailabilities))
				{
					goto BUILD_UNISCRIBE_DATA_FAILED;
				}

				if(breakings.Count()==1 && !breakingAvailabilities[0])
				{
					BYTE charClass=0;
					HRESULT hr=ScriptStringAnalyse(
						dc->GetHandle(),
						runText,
						length,
						(int)(1.5*length+16),
						-1,
						SSA_FALLBACK|SSA_GLYPHS|SSA_LINK|(scriptItem->IsRightToLeft()?SSA_RTL:0),
						0,
						NULL,
						NULL,
						NULL,
						NULL,
						&charClass,
						&ssa
						);
					hr=E_FAIL;
					if(hr==S_OK)
					{
						wholeGlyph.BuildUniscribeData(ssa, &scriptItem->scriptItem);
					}
					else if(ssa)
					{
						ScriptStringFree(&ssa);
						ssa=0;
					}
				}
				advance=wholeGlyph.runAbc.abcA+wholeGlyph.runAbc.abcB+wholeGlyph.runAbc.abcC;

				return true;
	BUILD_UNISCRIBE_DATA_FAILED:
				ClearUniscribeData();
				return false;
			}

			vint UniscribeTextRun::SumWidth(vint charStart, vint charLength)
			{
				vint cluster=0;
				vint nextCluster=0;
				SearchGlyphCluster(charStart, charLength, cluster, nextCluster);
				vint width=0;
				if(scriptItem->IsRightToLeft())
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

			vint UniscribeTextRun::SumHeight()
			{
				return documentFragment->fontStyle.size;
			}

			void UniscribeTextRun::SearchForLineBreak(vint tempStart, vint maxWidth, bool firstRun, vint& charLength, vint& charAdvances)
			{
				vint width=0;
				charLength=0;
				charAdvances=0;
				for(vint i=tempStart;i<=length;)
				{
					if(i==length || scriptItem->charLogattrs[i+(start-scriptItem->start)].fSoftBreak==TRUE)
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

					if(scriptItem->IsRightToLeft())
					{
						vint nextCluster
							=i+clusterLength==length
							?-1
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

			void UniscribeTextRun::Render(WinDC* dc, vint fragmentBoundsIndex, vint offsetX, vint offsetY)
			{
				Color fontColor=documentFragment->fontColor;
				HFONT oldFont=0;
				dc->SetFont(documentFragment->fontObject);
				dc->SetTextColor(RGB(fontColor.r, fontColor.g, fontColor.b));

				RunFragmentBounds fragment=fragmentBounds[fragmentBoundsIndex];
				if(fragment.length==0) return;
				RECT rect;
				rect.left=(int)(fragment.bounds.Left()+offsetX);
				rect.top=(int)(fragment.bounds.Top()+offsetY);
				rect.right=(int)(fragment.bounds.Right()+offsetX);
				rect.bottom=(int)(fragment.bounds.Bottom()+offsetY);

				if(ssa)
				{
					SCRIPT_STRING_ANALYSIS tempSsa=0;
					BYTE charClass=0;
					HRESULT hr=ScriptStringAnalyse(
						dc->GetHandle(),
						runText,
						length,
						(int)(1.5*length+16),
						-1,
						SSA_FALLBACK|SSA_GLYPHS|SSA_LINK|(scriptItem->IsRightToLeft()?SSA_RTL:0),
						0,
						NULL,
						NULL,
						NULL,
						NULL,
						&charClass,
						&tempSsa
						);
					if(hr=S_OK)
					{
						hr=ScriptStringOut(
							tempSsa,
							rect.left,
							rect.top,
							0,
							NULL,
							1,
							0,
							FALSE
							);
					}
					if(tempSsa)
					{
						ScriptStringFree(&tempSsa);
					}
				}
				else
				{
					vint cluster=0;
					vint nextCluster=0;
					SearchGlyphCluster(fragment.start, fragment.length, cluster, nextCluster);

					vint clusterStart=0;
					vint clusterCount=0;
					if(scriptItem->IsRightToLeft())
					{
						clusterStart=nextCluster+1;
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
			}

/***********************************************************************
UniscribeElementRun
***********************************************************************/

			UniscribeElementRun::UniscribeElementRun()
			{
			}

			UniscribeElementRun::~UniscribeElementRun()
			{
			}

			bool UniscribeElementRun::BuildUniscribeData(WinDC* dc, List<vint>& breakings)
			{
				breakings.Add(0);
				return true;
			}

			vint UniscribeElementRun::SumWidth(vint charStart, vint charLength)
			{
				return properties.size.x;
			}

			vint UniscribeElementRun::SumHeight()
			{
				return properties.size.y;
			}

			void UniscribeElementRun::SearchForLineBreak(vint tempStart, vint maxWidth, bool firstRun, vint& charLength, vint& charAdvances)
			{
				charLength=length-tempStart;
				charAdvances=properties.size.x;
			}

			void UniscribeElementRun::Render(WinDC* dc, vint fragmentBoundsIndex, vint offsetX, vint offsetY)
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

/***********************************************************************
UniscribeVirtualLine
***********************************************************************/

			UniscribeVirtualLine::UniscribeVirtualLine()
				:start(0)
				,length(0)
				,runText(0)
				,firstRunIndex(-1)
				,firstRunBoundsIndex(-1)
				,lastRunIndex(-1)
				,lastRunBoundsIndex(-1)
			{
			}

/***********************************************************************
UniscribeLine
***********************************************************************/

			UniscribeLine::UniscribeLine()
				:start(0)
			{
			}

			void UniscribeLine::ClearUniscribeData()
			{
				scriptItems.Clear();
				scriptRuns.Clear();
				virtualLines.Clear();
			}

			bool UniscribeLine::BuildUniscribeData(WinDC* dc)
			{
				lineText=L"";
				ClearUniscribeData();
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
						Array<SCRIPT_ITEM> items(lineText.Length()+2);
						int scriptItemCount=0;
						HRESULT hr=ScriptItemize(
							lineText.Buffer(),
							(int)lineText.Length(),
							(int)(items.Count()-1),
							&sc,
							&ss,
							&items[0],
							&scriptItemCount
							);
						if(hr!=0)
						{
							goto BUILD_UNISCRIBE_DATA_FAILED;
						}

						items.Resize(scriptItemCount+1);
						for(vint i=0;i<scriptItemCount;i++)
						{
							SCRIPT_ITEM item=items[i];
							Ptr<UniscribeItem> scriptItem=new UniscribeItem;
							scriptItem->start=item.iCharPos;
							scriptItem->length=items[i+1].iCharPos-item.iCharPos;
							scriptItem->itemText=lineText.Buffer()+item.iCharPos;
							scriptItem->scriptItem=item;

							if(!scriptItem->BuildUniscribeData())
							{
								goto BUILD_UNISCRIBE_DATA_FAILED;
							}
							scriptItems.Add(scriptItem);
						}
					}
					{
						// use item and document fragment information to produce runs
						// one item is constructed by one or more runs
						// characters in each run contains the same style
						vint fragmentIndex=0;
						vint fragmentStart=0;
						for(vint i=0;i<scriptItems.Count();i++)
						{
							Ptr<UniscribeItem> scriptItem=scriptItems[i];
							vint currentStart=scriptItem->start;

							while(currentStart<scriptItem->start+scriptItem->length)
							{
								UniscribeFragment* fragment=0;
								vint itemRemainLength=scriptItem->length-(currentStart-scriptItem->start);
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
													run->scriptItem=scriptItem.Obj();
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
									run->scriptItem=scriptItem.Obj();
									run->start=currentStart;
									run->length=shortLength;
									run->runText=lineText.Buffer()+currentStart;
									scriptRuns.Add(run);
								}
								currentStart+=shortLength;
							}
						}

						// for each run, generate shape information
						vint runIndex=0;
						while(runIndex<scriptRuns.Count())
						{
							Ptr<UniscribeRun> run=scriptRuns[runIndex];
							List<vint> breakings;
							if(!run->BuildUniscribeData(dc, breakings))
							{
								goto BUILD_UNISCRIBE_DATA_FAILED;
							}
							else if(breakings.Count()>1)
							{
								if(Ptr<UniscribeTextRun> textRun=run.Cast<UniscribeTextRun>())
								{
									scriptRuns.RemoveAt(runIndex);
									for(vint i=0;i<breakings.Count();i++)
									{
										vint start=breakings[i];
										vint length=i==breakings.Count()-1?textRun->length-start:breakings[i+1]-start;

										Ptr<UniscribeTextRun> newRun=new UniscribeTextRun;
										newRun->documentFragment=run->documentFragment;
										newRun->scriptItem=run->scriptItem;
										newRun->start=start+run->start;
										newRun->length=length;
										newRun->runText=run->runText+newRun->start-run->start;
										scriptRuns.Insert(runIndex+i, newRun);
									}
									continue;
								}
							}
							runIndex++;
						}
					}
				}
				return true;
	BUILD_UNISCRIBE_DATA_FAILED:
				ClearUniscribeData();
				return false;
			}

			void UniscribeLine::Layout(vint availableWidth, Alignment alignment, vint top, vint& totalHeight)
			{
				vint cx=0;
				vint cy=top;
				virtualLines.Clear();
				if(scriptRuns.Count()==0)
				{
					// if this line doesn't contains any run, skip and render a blank line
					vint height=documentFragments[0]->fontStyle.size;
					bounds=Rect(Point(cx, cy), Size(0, height));
					cy+=height;
				}
				else
				{
					FOREACH(Ptr<UniscribeRun>, run, scriptRuns)
					{
						run->fragmentBounds.Clear();
					}

					// render this line into lines with auto line wrapping
					vint startRun=0;
					vint startRunOffset=0;
					vint lastRun=0;
					vint lastRunOffset=0;
					vint currentWidth=0;

					while(startRun<scriptRuns.Count())
					{
						vint currentWidth=0;
						bool firstRun=true;
						// search for a range to fit in the given width
						for(vint i=startRun;i<scriptRuns.Count();i++)
						{
							vint charLength=0;
							vint charAdvances=0;
							UniscribeRun* run=scriptRuns[i].Obj();
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
							vint availableLastRun=lastRun<scriptRuns.Count()-1?lastRun:scriptRuns.Count()-1;
							vint maxHeight=0;
							for(vint i=startRun;i<=availableLastRun;i++)
							{
								if(i==lastRun && lastRunOffset==0)
								{
									break;
								}
								vint size=scriptRuns[i]->SumHeight();
								if(maxHeight<size)
								{
									maxHeight=size;
								}
							}

							// determine the rendering order for all runs inside this range
							Array<BYTE> levels(availableLastRun-startRun+1);
							Array<vint> runVisualToLogical(levels.Count());
							Array<vint> runLogicalToVisual(levels.Count());
							for(vint i=startRun;i<=availableLastRun;i++)
							{
								levels[i-startRun]=scriptRuns[i]->scriptItem->scriptItem.a.s.uBidiLevel;
							}
							ScriptLayout((int)levels.Count(), &levels[0], &runVisualToLogical[0], &runLogicalToVisual[0]);

							// render all runs inside this range
							vint startRunFragmentCount=-1;
							for(vint i=startRun;i<=availableLastRun;i++)
							{
								vint runIndex=runVisualToLogical[i-startRun]+startRun;
								UniscribeRun* run=scriptRuns[runIndex].Obj();
								vint start=runIndex==startRun?startRunOffset:0;
								vint end=runIndex==lastRun?lastRunOffset:run->length;
								vint length=end-start;

								if(startRunFragmentCount==-1)
								{
									startRunFragmentCount=run->fragmentBounds.Count();
								}

								UniscribeRun::RunFragmentBounds fragmentBounds;
								fragmentBounds.start=start;
								fragmentBounds.length=length;
								fragmentBounds.bounds=Rect(
									Point(cx, cy+maxHeight-run->SumHeight()),
									Size(run->SumWidth(start, length), run->SumHeight())
									);
								run->fragmentBounds.Add(fragmentBounds);

								cx+=run->SumWidth(start, length);
							}

							// adjust alignment
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

							// shift all bounds using alignment
							if(cxOffset!=0)
							{
								for(vint i=startRun;i<=availableLastRun;i++)
								{
									UniscribeRun* run=scriptRuns[i].Obj();
									for(vint j=(i==startRun?startRunFragmentCount:0);j<run->fragmentBounds.Count();j++)
									{
										UniscribeRun::RunFragmentBounds& fragmentBounds=run->fragmentBounds[j];
										fragmentBounds.bounds.x1+=cxOffset;
										fragmentBounds.bounds.x2+=cxOffset;
									}
								}
							}

							// create a virtual line
							{
								Ptr<UniscribeVirtualLine> virtualLine=new UniscribeVirtualLine;
								virtualLine->firstRunIndex=startRun;
								virtualLine->firstRunBoundsIndex=startRunFragmentCount;
								virtualLine->lastRunIndex=availableLastRun;
								virtualLine->lastRunBoundsIndex=scriptRuns[availableLastRun]->fragmentBounds.Count()-1;

								UniscribeRun* firstRun=scriptRuns[virtualLine->firstRunIndex].Obj();
								UniscribeRun* lastRun=scriptRuns[virtualLine->lastRunIndex].Obj();
								UniscribeRun::RunFragmentBounds& firstBounds=firstRun->fragmentBounds[virtualLine->firstRunBoundsIndex];
								UniscribeRun::RunFragmentBounds& lastBounds=lastRun->fragmentBounds[virtualLine->lastRunBoundsIndex];
								
								virtualLine->start=firstRun->start+firstBounds.start;
								virtualLine->length=lastRun->start+lastBounds.start+lastBounds.length-virtualLine->start;
								virtualLine->runText=lineText.Buffer()+virtualLine->start;

								bool updateBounds=false;
								for(vint i=startRun;i<=availableLastRun;i++)
								{
									UniscribeRun* run=scriptRuns[i].Obj();
									for(vint j=(i==startRun?startRunFragmentCount:0);j<run->fragmentBounds.Count();j++)
									{
										UniscribeRun::RunFragmentBounds& fragmentBounds=run->fragmentBounds[j];
										if(updateBounds)
										{
											if(virtualLine->bounds.x1>fragmentBounds.bounds.x1) virtualLine->bounds.x1=fragmentBounds.bounds.x1;
											if(virtualLine->bounds.x2<fragmentBounds.bounds.x2) virtualLine->bounds.x2=fragmentBounds.bounds.x2;
											if(virtualLine->bounds.y1>fragmentBounds.bounds.y1) virtualLine->bounds.y1=fragmentBounds.bounds.y1;
											if(virtualLine->bounds.y2<fragmentBounds.bounds.y2) virtualLine->bounds.y2=fragmentBounds.bounds.y2;
										}
										else
										{
											virtualLine->bounds=fragmentBounds.bounds;
											updateBounds=true;
										}
									}
								}
								virtualLines.Add(virtualLine);
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
					FOREACH(Ptr<UniscribeRun>, run, scriptRuns)
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
					bounds=Rect(minX, minY, maxX, maxY);
				}
				totalHeight=cy;
			}

			void UniscribeLine::Render(WinDC* dc, vint offsetX, vint offsetY)
			{
				FOREACH(Ptr<UniscribeRun>, run, scriptRuns)
				{
					for(vint i=0;i<run->fragmentBounds.Count();i++)
					{
						run->Render(dc, i, offsetX, offsetY);
					}
				}
			}

/***********************************************************************
UniscribeParagraph
***********************************************************************/

			UniscribeParagraph::UniscribeParagraph()
				:lastAvailableWidth(-1)
				,paragraphAlignment(Alignment::Left)
				,built(false)
			{
			}

			UniscribeParagraph::~UniscribeParagraph()
			{
				ClearUniscribeData();
			}

			void UniscribeParagraph::ClearUniscribeData()
			{
				FOREACH(Ptr<UniscribeFragment>, fragment, documentFragments)
				{
					GetWindowsGDIResourceManager()->DestroyGdiFont(fragment->fontStyle);
					fragment->fontObject=0;
				}
				lines.Clear();
				lastAvailableWidth=-1;
			}

			void UniscribeParagraph::BuildUniscribeData(WinDC* dc)
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

					vint lineStart=0;
					FOREACH(Ptr<UniscribeLine>, line, lines)
					{
						line->start=lineStart;
						lineStart+=line->lineText.Length()+2;
					}
				}
			}

			void UniscribeParagraph::Layout(vint availableWidth, Alignment alignment)
			{
				if(lastAvailableWidth==availableWidth && paragraphAlignment==alignment)
				{
					return;
				}
				lastAvailableWidth=availableWidth;
				paragraphAlignment=alignment;

				vint cy=0;
				FOREACH(Ptr<UniscribeLine>, line, lines)
				{
					vint totalHeight=0;
					line->Layout(availableWidth, alignment, cy, totalHeight);
					cy+=totalHeight;
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

				vint offsetY=0;
				FOREACH(Ptr<UniscribeLine>, line, lines)
				{
					FOREACH(Ptr<UniscribeFragment>, fragment, line->documentFragments)
					{
						vint size=fragment->fontStyle.size/3;
						if(size>offsetY)
						{
							offsetY=size;
						}
					}
				}
				bounds=Rect(minX, minY, maxX, maxY+offsetY);
			}

			void UniscribeParagraph::Render(WinDC* dc, vint offsetX, vint offsetY)
			{
				FOREACH(Ptr<UniscribeLine>, line, lines)
				{
					line->Render(dc, offsetX, offsetY);
				}
			}

			void UniscribeParagraph::SearchFragment(vint start, vint length, vint& fs, vint& ss, vint& fe, vint& se)
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

			bool UniscribeParagraph::CutFragment(vint fs, vint ss, vint fe, vint se, vint& f1, vint& f2)
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

			bool UniscribeParagraph::SetFont(vint start, vint length, const WString& value)
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

			bool UniscribeParagraph::SetSize(vint start, vint length, vint value)
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

			bool UniscribeParagraph::SetStyle(vint start, vint length, bool bold, bool italic, bool underline, bool strikeline)
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

			bool UniscribeParagraph::SetColor(vint start, vint length, Color value)
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

			bool UniscribeParagraph::SetInlineObject(vint start, vint length, const IGuiGraphicsParagraph::InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value)
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

			Ptr<IGuiGraphicsElement> UniscribeParagraph::ResetInlineObject(vint start, vint length)
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

			bool UniscribeParagraph::SetInteractionId(vint start, vint length, vint value)
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

			bool UniscribeParagraph::HitTestPoint(Point point, vint& start, vint& length, vint& interactionId)
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

			void UniscribeParagraph::GetLineIndexFromTextPos(vint textPos, vint& frontLine, vint& backLine)
			{
				frontLine=-1;
				backLine=-1;
				if(!IsValidTextPos(textPos)) return;

				vint start=0;
				vint end=lines.Count()-1;
				while(start<=end)
				{
					vint middle=(start+end)/2;
					Ptr<UniscribeLine> line=lines[middle];
					vint lineStart=line->start;
					vint lineEnd=line->start+line->lineText.Length();
					if(textPos<lineStart)
					{
						if(textPos==lineStart-1)
						{
							frontLine=middle-1;
							backLine=middle;
							return;
						}
						else
						{
							end=middle-1;
						}
					}
					else if(textPos>lineEnd)
					{
						if(textPos==lineEnd+1)
						{
							frontLine=middle;
							backLine=middle+1;
							return;
						}
						else
						{
							start=middle+1;
						}
					}
					else
					{
						frontLine=middle;
						backLine=middle;
						return;
					}
				}
			}

			void UniscribeParagraph::GetVirtualLineIndexFromTextPos(vint textPos, vint lineIndex, vint& frontLine, vint& backLine)
			{
				frontLine=-1;
				backLine=-1;
				if(!IsValidTextPos(textPos)) return;
				if(lineIndex<0 || lineIndex>=lines.Count()) return;

				Ptr<UniscribeLine> line=lines[lineIndex];
				vint start=0;
				vint end=line->virtualLines.Count()-1;
				while(start<=end)
				{
					vint middle=(start+end)/2;
					Ptr<UniscribeVirtualLine> vline=line->virtualLines[middle];
					vint lineStart=vline->start;
					vint lineEnd=vline->start+vline->length;
					if(textPos<lineStart)
					{
						end=middle-1;
					}
					else if(textPos>lineEnd)
					{
						start=middle+1;
					}
					else if(textPos==lineStart)
					{
						frontLine=middle==0?0:middle-1;
						backLine=middle;
						return;
					}
					else if(textPos==lineEnd)
					{
						frontLine=middle;
						backLine=middle==line->virtualLines.Count()-1?middle:middle+1;
						return;
					}
					else
					{
						frontLine=middle;
						backLine=middle;
						return;
					}
				}
			}

			vint UniscribeParagraph::GetCaret(vint comparingCaret, IGuiGraphicsParagraph::CaretRelativePosition position)
			{
				throw 0;
			}

			Rect UniscribeParagraph::GetCaretBounds(vint caret, bool frontSide)
			{
				throw 0;
			}

			vint UniscribeParagraph::GetCaretFromPoint(Point point)
			{
				throw 0;
			}

			vint UniscribeParagraph::GetNearestCaretFromTextPos(vint textPos, bool frontSide)
			{
				throw 0;
			}

			bool UniscribeParagraph::IsValidCaret(vint caret)
			{
				return IsValidTextPos(caret)
					&& GetNearestCaretFromTextPos(caret, true)==caret
					&& GetNearestCaretFromTextPos(caret, false)==caret;
			}

			bool UniscribeParagraph::IsValidTextPos(vint textPos)
			{
				return 0<=textPos && textPos<=paragraphText.Length();
			}
		}
	}
}