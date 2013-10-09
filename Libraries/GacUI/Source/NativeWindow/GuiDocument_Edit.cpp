#include "GuiResource.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

		typedef DocumentModel::RunRange			RunRange;
		typedef DocumentModel::RunRangeMap		RunRangeMap;

/***********************************************************************
document_serialization_visitors::GetRunRangeVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class GetRunRangeVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				RunRangeMap&					runRanges;
				vint							start;

				GetRunRangeVisitor(RunRangeMap& _runRanges)
					:runRanges(_runRanges)
					,start(0)
				{
				}

				void VisitContainer(DocumentContainerRun* run)
				{
					RunRange range;
					range.start=start;
					FOREACH(Ptr<DocumentRun>, subRun, run->runs)
					{
						subRun->Accept(this);
					}
					range.end=start;
					runRanges.Add(run, range);
				}

				void VisitContent(DocumentContentRun* run)
				{
					RunRange range;
					range.start=start;
					start+=run->GetRepresentationText().Length();
					range.end=start;
					runRanges.Add(run, range);
				}

				void Visit(DocumentTextRun* run)override
				{
					VisitContent(run);
				}

				void Visit(DocumentStylePropertiesRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentImageRun* run)override
				{
					VisitContent(run);
				}

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentParagraphRun* run)override
				{
					VisitContainer(run);
				}

				static void GetRunRange(DocumentRun* run, RunRangeMap& runRanges)
				{
					GetRunRangeVisitor visitor(runRanges);
					run->Accept(&visitor);
				}
			};
		}
		using namespace document_serialization_visitors;

/***********************************************************************
document_serialization_visitors::LocateStyleVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class LocateStyleVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				List<DocumentContainerRun*>&	locatedRuns;
				RunRangeMap&					runRanges;
				vint							position;
				bool							frontSide;

				LocateStyleVisitor(List<DocumentContainerRun*>& _locatedRuns, RunRangeMap& _runRanges, vint _position, bool _frontSide)
					:locatedRuns(_locatedRuns)
					,runRanges(_runRanges)
					,position(_position)
					,frontSide(_frontSide)
				{
				}

				void VisitContainer(DocumentContainerRun* run)
				{
					locatedRuns.Add(run);
					Ptr<DocumentRun> selectedRun;
					FOREACH(Ptr<DocumentRun>, subRun, run->runs)
					{
						RunRange range=runRanges[subRun.Obj()];
						if(position==range.start)
						{
							if(!frontSide)
							{
								selectedRun=subRun;
								break;
							}
						}
						else if(position==range.end)
						{
							if(frontSide)
							{
								selectedRun=subRun;
								break;
							}
						}
						else if(range.start<position && position<range.end)
						{
							selectedRun=subRun;
							break;
						}
					}

					if(selectedRun)
					{
						selectedRun->Accept(this);
					}
				}

				void Visit(DocumentTextRun* run)override
				{
				}

				void Visit(DocumentStylePropertiesRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentImageRun* run)override
				{
				}

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentParagraphRun* run)override
				{
					VisitContainer(run);
				}

				static void LocateStyle(DocumentRun* run, RunRangeMap& runRanges, vint position, bool frontSide, List<DocumentContainerRun*>& locatedRuns)
				{
					LocateStyleVisitor visitor(locatedRuns, runRanges, position, frontSide);
					run->Accept(&visitor);
				}
			};
		}
		using namespace document_serialization_visitors;

/***********************************************************************
document_serialization_visitors::CloneRunVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class CloneRunVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				Ptr<DocumentRun>				clonedRun;

				CloneRunVisitor(Ptr<DocumentRun> subRun)
					:clonedRun(subRun)
				{
				}

				void VisitContainer(Ptr<DocumentContainerRun> cloned)
				{
					if(clonedRun)
					{
						cloned->runs.Add(clonedRun);
					}
					clonedRun=cloned;
				}

				void Visit(DocumentTextRun* run)override
				{
					Ptr<DocumentTextRun> cloned=new DocumentTextRun;
					cloned->text=run->text;
					clonedRun=cloned;
				}

				void Visit(DocumentStylePropertiesRun* run)override
				{
					Ptr<DocumentStylePropertiesRun> cloned=new DocumentStylePropertiesRun;
					cloned->style=new DocumentStyleProperties;
					cloned->style->face					=run->style->face;
					cloned->style->size					=run->style->size;
					cloned->style->color				=run->style->color;
					cloned->style->backgroundColor		=run->style->backgroundColor;
					cloned->style->bold					=run->style->bold;
					cloned->style->italic				=run->style->italic;
					cloned->style->underline			=run->style->underline;
					cloned->style->strikeline			=run->style->strikeline;
					cloned->style->antialias			=run->style->antialias;
					cloned->style->verticalAntialias	=run->style->verticalAntialias;

					VisitContainer(cloned);
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					Ptr<DocumentStyleApplicationRun> cloned=new DocumentStyleApplicationRun;
					cloned->styleName=run->styleName;
					
					VisitContainer(cloned);
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					Ptr<DocumentHyperlinkTextRun> cloned=new DocumentHyperlinkTextRun;
					cloned->normalStyleName=run->normalStyleName;
					cloned->activeStyleName=run->activeStyleName;
					cloned->hyperlinkId=run->hyperlinkId;
					
					VisitContainer(cloned);
				}

				void Visit(DocumentImageRun* run)override
				{
					Ptr<DocumentImageRun> cloned=new DocumentImageRun;
					cloned->size=run->size;
					cloned->baseline=run->baseline;
					cloned->image=run->image;
					cloned->frameIndex=run->frameIndex;
					cloned->source=run->source;
					clonedRun=cloned;
				}

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					Ptr<DocumentTemplateApplicationRun> cloned=new DocumentTemplateApplicationRun;
					cloned->templateName=run->templateName;
					CopyFrom(cloned->attributes, run->attributes);
					
					VisitContainer(cloned);
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					Ptr<DocumentTemplateContentRun> cloned=new DocumentTemplateContentRun;
					
					VisitContainer(cloned);
				}

				void Visit(DocumentParagraphRun* run)override
				{
					Ptr<DocumentParagraphRun> cloned=new DocumentParagraphRun;
					cloned->alignment=run->alignment;
						
					VisitContainer(cloned);
				}

				static Ptr<DocumentRun> CopyRun(DocumentRun* run)
				{
					CloneRunVisitor visitor(0);
					run->Accept(&visitor);
					return visitor.clonedRun;
				}

				static Ptr<DocumentRun> CopyStyledText(List<DocumentContainerRun*>& styleRuns, const WString& text)
				{
					Ptr<DocumentTextRun> textRun=new DocumentTextRun;
					textRun->text=text;

					CloneRunVisitor visitor(textRun);
					for(vint i=styleRuns.Count()-1;i>=0;i--)
					{
						styleRuns[i]->Accept(&visitor);
					}

					return visitor.clonedRun;
				}
			};
		}
		using namespace document_serialization_visitors;

/***********************************************************************
document_serialization_visitors::RemoveRunVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class RemoveRunVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				RunRangeMap&					runRanges;
				vint							start;
				vint							end;
				List<Ptr<DocumentRun>>			replacedRuns;

				RemoveRunVisitor(RunRangeMap& _runRanges, vint _start, vint _end)
					:runRanges(_runRanges)
					,start(_start)
					,end(_end)
				{
				}

				void VisitContainer(DocumentContainerRun* run)
				{
					if(start==end) return;
					for(vint i=run->runs.Count()-1;i>=0;i--)
					{
						Ptr<DocumentRun> subRun=run->runs[i];
						RunRange range=runRanges[subRun.Obj()];

						if(range.start<=end && start<=range.end)
						{
							subRun->Accept(this);
							if(replacedRuns.Count()==0 || subRun!=replacedRuns[0])
							{
								run->runs.RemoveAt(i);
								for(vint j=0;j<replacedRuns.Count();j++)
								{
									run->runs.Insert(i+j, replacedRuns[j]);
								}
							}
						}
					}
					replacedRuns.Clear();
					replacedRuns.Add(run);
				}

				void Visit(DocumentTextRun* run)override
				{
					replacedRuns.Clear();
					RunRange range=runRanges[run];

					if(start<=range.start)
					{
						if(end<range.end)
						{
							run->text=run->text.Sub(end-range.start, range.end-end);
							replacedRuns.Add(run);
						}
					}
					else
					{
						if(end<range.end)
						{
							DocumentTextRun* firstRun=new DocumentTextRun;
							DocumentTextRun* secondRun=new DocumentTextRun;

							firstRun->text=run->text.Sub(0, start-range.start);
							secondRun->text=run->text.Sub(end-range.start, range.end-end);

							replacedRuns.Add(firstRun);
							replacedRuns.Add(secondRun);
						}
						else
						{
							run->text=run->text.Sub(0, start-range.start);
							replacedRuns.Add(run);
						}
					}
				}

				void Visit(DocumentStylePropertiesRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentImageRun* run)override
				{
					replacedRuns.Clear();
				}

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentParagraphRun* run)override
				{
					VisitContainer(run);
				}

				static void RemoveRun(DocumentRun* run, RunRangeMap& runRanges, vint start, vint end)
				{
					RemoveRunVisitor visitor(runRanges, start, end);
					run->Accept(&visitor);
				}
			};
		}
		using namespace document_serialization_visitors;

/***********************************************************************
document_serialization_visitors::CutRunVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class CutRunVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				RunRangeMap&					runRanges;
				vint							position;
				Ptr<DocumentRun>				leftRun;
				Ptr<DocumentRun>				rightRun;

				CutRunVisitor(RunRangeMap& _runRanges, vint _position)
					:runRanges(_runRanges)
					,position(_position)
				{
				}

				void VisitContainer(DocumentContainerRun* run)
				{
					vint leftCount=0;
					Ptr<DocumentRun> selectedRun;

					FOREACH(Ptr<DocumentRun>, subRun, run->runs)
					{
						RunRange range=runRanges[subRun.Obj()];
						if(range.start<position)
						{
							leftCount++;
							if (position<range.end)
							{
								selectedRun=subRun;
							}
						}
						else
						{
							break;
						}
					}

					if(selectedRun)
					{
						selectedRun->Accept(this);
						if(leftRun && rightRun)
						{
							run->runs.RemoveAt(leftCount-1);
							run->runs.Insert(leftCount-1, leftRun);
							run->runs.Insert(leftCount, rightRun);
						}
					}
					
					Ptr<DocumentContainerRun> leftContainer=CloneRunVisitor::CopyRun(run).Cast<DocumentContainerRun>();
					Ptr<DocumentContainerRun> rightContainer=CloneRunVisitor::CopyRun(run).Cast<DocumentContainerRun>();
					for(vint i=0;i<run->runs.Count();i++)
					{
						(i<leftCount?leftContainer:rightContainer)->runs.Add(run->runs[i]);
					}
					leftRun=leftContainer;
					rightRun=rightContainer;
				}

				void VisitContent(DocumentContentRun* run)
				{
					leftRun=0;
					rightRun=0;
				}

				void Visit(DocumentTextRun* run)override
				{
					VisitContent(run);
				}

				void Visit(DocumentStylePropertiesRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentImageRun* run)override
				{
					VisitContent(run);
				}

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentParagraphRun* run)override
				{
					VisitContainer(run);
				}

				static void CutRun(DocumentRun* run, RunRangeMap& runRanges, vint position, Ptr<DocumentRun>& leftRun, Ptr<DocumentRun>& rightRun)
				{
					CutRunVisitor visitor(runRanges, position);
					run->Accept(&visitor);
					leftRun=visitor.leftRun;
					rightRun=visitor.rightRun;
				}
			};
		}
		using namespace document_serialization_visitors;

/***********************************************************************
document_serialization_visitors::ClearRunVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class ClearRunVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				vint							start;

				ClearRunVisitor()
					:start(0)
				{
				}

				void VisitContainer(DocumentContainerRun* run)
				{
					for(vint i=run->runs.Count()-1;i>=0;i--)
					{
						vint oldStart=start;
						run->runs[i]->Accept(this);
						if(oldStart==start)
						{
							run->runs.RemoveAt(i);
						}
					}
				}

				void VisitContent(DocumentContentRun* run)
				{
					start+=run->GetRepresentationText().Length();
				}

				void Visit(DocumentTextRun* run)override
				{
					VisitContent(run);
				}

				void Visit(DocumentStylePropertiesRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentImageRun* run)override
				{
					VisitContent(run);
				}

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentParagraphRun* run)override
				{
					VisitContainer(run);
				}

				static void ClearRun(DocumentRun* run)
				{
					ClearRunVisitor visitor;
					run->Accept(&visitor);
				}
			};
		}
		using namespace document_serialization_visitors;

/***********************************************************************
DocumentModel
***********************************************************************/

		bool DocumentModel::CheckEditRange(TextPos begin, TextPos end, RunRangeMap& relatedRanges)
		{
			// check caret range
			if(begin>end) return false;
			if(begin.row<0 || begin.row>=paragraphs.Count()) return false;
			if(end.row<0 || end.row>=paragraphs.Count()) return false;

			// determine run ranges
			GetRunRangeVisitor::GetRunRange(paragraphs[begin.row].Obj(), relatedRanges);
			if(begin.row!=end.row)
			{
				GetRunRangeVisitor::GetRunRange(paragraphs[end.row].Obj(), relatedRanges);
			}
			
			// check caret range
			RunRange beginRange=relatedRanges[paragraphs[begin.row].Obj()];
			RunRange endRange=relatedRanges[paragraphs[end.row].Obj()];
			if(begin.column<0 || begin.column>beginRange.end) return false;
			if(end.column<0 || end.column>endRange.end) return false;

			return true;
		}

		vint DocumentModel::EditRun(TextPos begin, TextPos end, const collections::Array<Ptr<DocumentParagraphRun>>& runs)
		{
			// check caret range
			RunRangeMap runRanges;
			if(!CheckEditRange(begin, end, runRanges)) return -1;

			// remove unnecessary paragraphs
			if(begin.row!=end.row)
			{
				for(vint i=end.row-1;i>begin.row;i--)
				{
					paragraphs.RemoveAt(i);
				}
				end.row=begin.row+1;
			}

			// remove unnecessary runs and ensure begin.row!=end.row
			if(begin.row==end.row)
			{
				RemoveRunVisitor::RemoveRun(paragraphs[begin.row].Obj(), runRanges, begin.column, end.column);

				Ptr<DocumentRun> leftRun, rightRun;
				CutRunVisitor::CutRun(paragraphs[begin.row].Obj(), runRanges, begin.column, leftRun, rightRun);

				paragraphs.RemoveAt(paragraphs[begin.row]);
				paragraphs.Insert(begin.row, leftRun.Cast<DocumentParagraphRun>());
				paragraphs.Insert(begin.row+1, rightRun.Cast<DocumentParagraphRun>());
				end.row=begin.row+1;
			}
			else
			{
				RemoveRunVisitor::RemoveRun(paragraphs[begin.row].Obj(), runRanges, 0, begin.column);
				RemoveRunVisitor::RemoveRun(paragraphs[end.row].Obj(), runRanges, end.column, runRanges[paragraphs[end.row].Obj()].end);
			}

			// insert new paragraphs
			Ptr<DocumentParagraphRun> beginParagraph=paragraphs[begin.row];
			Ptr<DocumentParagraphRun> endParagraph=paragraphs[end.row];
			if(runs.Count()==0)
			{
				CopyFrom(beginParagraph->runs, endParagraph->runs, true);
				paragraphs.RemoveAt(end.row);
			}
			else if(runs.Count()==1)
			{
				CopyFrom(beginParagraph->runs, runs[0]->runs, true);
				CopyFrom(beginParagraph->runs, endParagraph->runs, true);
				paragraphs.RemoveAt(end.row);
			}
			else
			{
				Ptr<DocumentParagraphRun> newBeginRuns=runs[0];
				CopyFrom(beginParagraph->runs, newBeginRuns->runs, true);
				
				Ptr<DocumentParagraphRun> newEndRuns=runs[runs.Count()-1];
				for(vint i=0;i<newEndRuns->runs.Count();i++)
				{
					endParagraph->runs.Insert(i, newEndRuns->runs[i]);
				}

				for(vint i=1;i<runs.Count()-1;i++)
				{
					paragraphs.Insert(begin.row+i, runs[i]);
				}
			}

			// clear unnecessary runs
			vint rows=runs.Count()==0?1:runs.Count();
			for(vint i=0;i<rows;i++)
			{
				ClearRunVisitor::ClearRun(paragraphs[begin.row+i].Obj());
			}
			return rows;
		}

		vint DocumentModel::EditText(TextPos begin, TextPos end, bool frontSide, const collections::Array<WString>& text)
		{
			// check caret range
			RunRangeMap runRanges;
			if(!CheckEditRange(begin, end, runRanges)) return -1;

			// calcuate the position to get the text style
			TextPos stylePosition;
			if(frontSide)
			{
				stylePosition=begin;
				if(stylePosition.column==0)
				{
					frontSide=false;
				}
			}
			else
			{
				stylePosition=end;
				if(stylePosition.column==runRanges[paragraphs[end.row].Obj()].end)
				{
					frontSide=true;
				}
			}

			// copy runs that contains the target style for new text
			List<DocumentContainerRun*> styleRuns;
			LocateStyleVisitor::LocateStyle(paragraphs[stylePosition.row].Obj(), runRanges, stylePosition.column, frontSide, styleRuns);

			// create paragraphs
			Array<Ptr<DocumentParagraphRun>> runs(text.Count());
			for(vint i=0;i<text.Count();i++)
			{
				Ptr<DocumentRun> paragraph=CloneRunVisitor::CopyStyledText(styleRuns, text[i]);
				runs[i]=paragraph.Cast<DocumentParagraphRun>();
			}

			// replace the paragraphs
			return EditRun(begin, end, runs);
		}

		bool DocumentModel::EditStyle(TextPos begin, TextPos end, Ptr<DocumentStyleProperties> style)
		{
			return false;
		}
	}
}