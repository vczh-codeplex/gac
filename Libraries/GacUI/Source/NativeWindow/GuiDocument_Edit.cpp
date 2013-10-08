#include "GuiResource.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

/***********************************************************************
document_serialization_visitors::GetRunRangeVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			struct RunRange
			{
				vint			start;
				vint			length;
			};

			typedef Dictionary<DocumentRun*, RunRange>		RunRangeMap;

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
					range.length=start-range.start;
					runRanges.Add(run, range);
				}

				void VisitContent(DocumentContentRun* run)
				{
					RunRange range;
					range.start=start;
					range.length=run->GetRepresentationText().Length();
					start+=range.length;
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

				LocateStyleVisitor(List<DocumentContainerRun*>& _locateRuns, RunRangeMap& _runRanges, vint _position, bool _frontSide)
					:locatedRuns(locatedRuns)
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
						vint start=range.start;
						vint end=range.start+range.length;
						if(position==start)
						{
							if(!frontSide)
							{
								selectedRun=subRun;
								break;
							}
						}
						else if(position==end)
						{
							if(frontSide)
							{
								selectedRun=subRun;
								break;
							}
						}
						else if(start<position && position<end)
						{
							selectedRun=subRun;
							break;
						}
					}
					selectedRun->Accept(this);
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
				bool							copyParagraph;
				bool							copyRecursively;

				CloneRunVisitor(Ptr<DocumentRun> _initialRun, bool _copyParagraph, bool _copyRecursively)
					:clonedRun(_initialRun)
					,copyParagraph(_copyParagraph)
					,copyRecursively(_copyRecursively)
				{
				}

				void VisitContainer(DocumentContainerRun* run, Ptr<DocumentContainerRun> cloned)
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

					VisitContainer(run, cloned);
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					Ptr<DocumentStyleApplicationRun> cloned=new DocumentStyleApplicationRun;
					cloned->styleName=run->styleName;
					
					VisitContainer(run, cloned);
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					Ptr<DocumentHyperlinkTextRun> cloned=new DocumentHyperlinkTextRun;
					cloned->normalStyleName=run->normalStyleName;
					cloned->activeStyleName=run->activeStyleName;
					cloned->hyperlinkId=run->hyperlinkId;
					
					VisitContainer(run, cloned);
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
					
					VisitContainer(run, cloned);
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					Ptr<DocumentTemplateContentRun> cloned=new DocumentTemplateContentRun;
					
					VisitContainer(run, cloned);
				}

				void Visit(DocumentParagraphRun* run)override
				{
					if(copyParagraph)
					{
						Ptr<DocumentParagraphRun> cloned=new DocumentParagraphRun;
						cloned->alignment=run->alignment;

						if(clonedRun)
						{
							cloned->runs.Add(clonedRun);
						}
						clonedRun=cloned;
					}
				}

				static Ptr<DocumentRun> CopyStyledText(List<DocumentContainerRun*>& styleRuns, const WString& text, bool copyParagraph)
				{
					if(text==L"")
					{
						if(copyParagraph)
						{
							CloneRunVisitor visitor(0, copyParagraph, false);
							styleRuns[0]->Accept(&visitor);
							return visitor.clonedRun;
						}
						else
						{
							return 0;
						}
					}
					else
					{
						Ptr<DocumentTextRun> textRun=new DocumentTextRun;
						textRun->text=text;

						CloneRunVisitor visitor(textRun, copyParagraph, false);
						for(vint i=styleRuns.Count()-1;i>=0;i--)
						{
							styleRuns[i]->Accept(&visitor);
						}

						return visitor.clonedRun;
					}
				}

				static Ptr<DocumentRun> CopyRunRecursively(Ptr<DocumentRun> run)
				{
					CloneRunVisitor visitor(0, true, true);
					run->Accept(&visitor);
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
				List<DocumentRun*>				replacedRuns;

				RemoveRunVisitor(RunRangeMap& _runRanges, vint _start, vint _end)
					:runRanges(_runRanges)
					,start(_start)
					,end(_end)
				{
				}

				void VisitContainer(DocumentContainerRun* run)
				{
					for(vint i=run->runs.Count()-1;i>=0;i--)
					{
						Ptr<DocumentRun> subRun=run->runs[i];
						RunRange range=runRanges[subRun.Obj()];
						vint runStart=range.start;
						vint runEnd=range.start+range.length;

						if(runStart<end && runEnd<start)
						{
							subRun->Accept(this);
							if(subRun.Obj()!=replacedRuns[0])
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
					vint textStart=range.start;
					vint textEnd=range.start+range.length;

					if(start<=textStart)
					{
						if(textEnd>end)
						{
							run->text=run->text.Sub(end-textStart, textEnd-end);
						}
					}
					else
					{
						if(textEnd>end)
						{
							DocumentTextRun* firstRun=new DocumentTextRun;
							DocumentTextRun* secondRun=new DocumentTextRun;

							firstRun->text=run->text.Sub(0, start-textStart);
							secondRun->text=run->text.Sub(end-textStart, textEnd-end);

							replacedRuns.Add(firstRun);
							replacedRuns.Add(secondRun);
						}
						else
						{
							run->text=run->text.Sub(0, start-textStart);
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
			};
		}
		using namespace document_serialization_visitors;

/***********************************************************************
DocumentModel
***********************************************************************/

		vint DocumentModel::EditText(TextPos begin, TextPos end, bool frontSide, const collections::Array<WString>& text)
		{
			// check caret range
			if(begin>end) return -1;
			if(begin.row<0 || begin.row>=paragraphs.Count()) return -1;
			if(end.row<0 || end.row>=paragraphs.Count()) return -1;

			// break the first updated paragraph if necessary
			if(begin.row==end.row)
			{
				Ptr<DocumentRun> clonedParagraph=CloneRunVisitor::CopyRunRecursively(paragraphs[begin.row]);
				paragraphs.Insert(begin.row, clonedParagraph.Cast<DocumentParagraphRun>());
				end.row++;
			}

			// determine run ranges
			RunRangeMap runRanges;
			{
				Ptr<DocumentParagraphRun> paragraph=paragraphs[begin.row];
				GetRunRangeVisitor visitor(runRanges);
				paragraph->Accept(&visitor);
			}
			{
				Ptr<DocumentParagraphRun> paragraph=paragraphs[end.row];
				GetRunRangeVisitor visitor(runRanges);
				paragraph->Accept(&visitor);
			}
			
			// check caret range
			RunRange beginRange=runRanges[paragraphs[begin.row].Obj()];
			RunRange endRange=runRanges[paragraphs[end.row].Obj()];
			if(begin.column<0 || begin.column>beginRange.length) return -1;
			if(end.column<0 || end.column>endRange.length) return -1;

			// copy runs that contains the target style for new text
			List<DocumentContainerRun*> styleRuns;
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
				if(stylePosition.column==endRange.length)
				{
					frontSide=true;
				}
			}
			{
				LocateStyleVisitor visitor(styleRuns, runRanges, stylePosition.column, frontSide);
				paragraphs[stylePosition.row]->Accept(&visitor);
			}

			// copy runs with new text
			Ptr<DocumentRun> newBeginRun;
			Ptr<DocumentRun> newEndRun;
			List<Ptr<DocumentParagraphRun>> middleParagraphs;

			if(text.Count()>0)
			{
				newBeginRun=CloneRunVisitor::CopyStyledText(styleRuns, text[0], false);
			}
			if(text.Count()>1)
			{
				newEndRun=CloneRunVisitor::CopyStyledText(styleRuns, text[text.Count()-1], false);
			}
			for(vint i=1;i<text.Count()-2;i++)
			{
				Ptr<DocumentRun> clonedRun=CloneRunVisitor::CopyStyledText(styleRuns, text[i], true);
				middleParagraphs.Add(clonedRun.Cast<DocumentParagraphRun>());
			}

			// rearrange paragraphs
			{
				RemoveRunVisitor visitor(runRanges, begin.column, runRanges[paragraphs[begin.row].Obj()].length);
				paragraphs[begin.row]->Accept(&visitor);
			}
			{
				RemoveRunVisitor visitor(runRanges, 0, end.column);
				paragraphs[end.row]->Accept(&visitor);
			}
			for(vint i=end.row-1;i>begin.row;i++)
			{
				paragraphs.RemoveAt(i);
			}

			// update paragraphs
			if(newBeginRun)
			{
				paragraphs[begin.row]->runs.Add(newBeginRun);
			}
			if(newEndRun)
			{
				paragraphs[end.row]->runs.Insert(0, newEndRun);
			}
			if(text.Count()==1)
			{
				CopyFrom(paragraphs[begin.row]->runs, paragraphs[end.row]->runs, true);
				paragraphs.RemoveAt(end.row);
			}
			for(vint i=0;i<middleParagraphs.Count();i++)
			{
				paragraphs.Insert(begin.row+1+i, middleParagraphs[i]);
			}

			return text.Count();
		}

		bool DocumentModel::EditStyle(TextPos begin, TextPos end, Ptr<DocumentStyleProperties> style)
		{
			return false;
		}
	}
}