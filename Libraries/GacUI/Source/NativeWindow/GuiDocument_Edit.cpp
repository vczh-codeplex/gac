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
document_serialization_visitors::CloneStyleRunVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class CloneStyleRunVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				Ptr<DocumentRun>				clonedRun;
				bool							copyParagraph;

				CloneStyleRunVisitor(Ptr<DocumentRun> _initialRun, bool _copyParagraph)
					:clonedRun(_initialRun)
					,copyParagraph(_copyParagraph)
				{
				}

				void Visit(DocumentTextRun* run)override
				{
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

					if(clonedRun)
					{
						cloned->runs.Add(clonedRun);
					}
					clonedRun=cloned;
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					Ptr<DocumentStyleApplicationRun> cloned=new DocumentStyleApplicationRun;
					cloned->styleName=run->styleName;

					if(clonedRun)
					{
						cloned->runs.Add(clonedRun);
					}
					clonedRun=cloned;
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					Ptr<DocumentHyperlinkTextRun> cloned=new DocumentHyperlinkTextRun;
					cloned->normalStyleName=run->normalStyleName;
					cloned->activeStyleName=run->activeStyleName;
					cloned->hyperlinkId=run->hyperlinkId;

					if(clonedRun)
					{
						cloned->runs.Add(clonedRun);
					}
					clonedRun=cloned;
				}

				void Visit(DocumentImageRun* run)override
				{
				}

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					Ptr<DocumentTemplateApplicationRun> cloned=new DocumentTemplateApplicationRun;
					cloned->templateName=run->templateName;
					CopyFrom(cloned->attributes, run->attributes);

					if(clonedRun)
					{
						cloned->runs.Add(clonedRun);
					}
					clonedRun=cloned;
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					Ptr<DocumentTemplateContentRun> cloned=new DocumentTemplateContentRun;

					if(clonedRun)
					{
						cloned->runs.Add(clonedRun);
					}
					clonedRun=cloned;
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
							CloneStyleRunVisitor visitor(0, copyParagraph);
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

						CloneStyleRunVisitor visitor(textRun, copyParagraph);
						for(vint i=styleRuns.Count()-1;i>=0;i--)
						{
							styleRuns[i]->Accept(&visitor);
						}

						return visitor.clonedRun;
					}
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

				RemoveRunVisitor()
				{
				}

				void VisitContainer(DocumentContainerRun* run)
				{
				}

				void VisitContent(DocumentContentRun* run)
				{
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
DocumentModel
***********************************************************************/

		vint DocumentModel::EditText(TextPos begin, TextPos end, bool frontSide, const collections::Array<WString>& text)
		{
			// check caret range
			if(begin>end) return -1;
			if(begin.row<0 || begin.row>=paragraphs.Count()) return -1;
			if(end.row<0 || end.row>=paragraphs.Count()) return -1;

			// determine run ranges
			RunRangeMap runRanges;
			{
				Ptr<DocumentParagraphRun> paragraph=paragraphs[begin.row];
				GetRunRangeVisitor visitor(runRanges);
				paragraph->Accept(&visitor);
			}
			if(begin.row!=end.row)
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

			newBeginRun=CloneStyleRunVisitor::CopyStyledText(styleRuns, (text.Count()==0?L"":text[0]), false);
			if(begin.row!=end.row)
			{
				newEndRun=CloneStyleRunVisitor::CopyStyledText(styleRuns, (text.Count()==0?L"":text[text.Count()-1]), false);
			}
			for(vint i=1;i<text.Count()-2;i++)
			{
				Ptr<DocumentRun> clonedRun=CloneStyleRunVisitor::CopyStyledText(styleRuns, text[i], true);
				middleParagraphs.Add(clonedRun.Cast<DocumentParagraphRun>());
			}

			return text.Count();
		}

		bool DocumentModel::EditStyle(TextPos begin, TextPos end, Ptr<DocumentStyleProperties> style)
		{
			return false;
		}
	}
}