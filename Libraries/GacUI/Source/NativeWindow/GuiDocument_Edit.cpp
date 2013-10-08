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
document_serialization_visitors::CopyStyleVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class CopyStyleVisitor : public Object, public DocumentRun::IVisitor
			{
			public:

				CopyStyleVisitor()
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
			if(begin.row<0 || begin.row>=paragraphs.Count()) return -1;
			if(end.row<0 || end.row>=paragraphs.Count()) return -1;

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

			if(begin.column<0 || begin.column>runRanges[paragraphs[begin.row].Obj()].length) return -1;
			if(end.column<0 || end.column>runRanges[paragraphs[end.row].Obj()].length) return -1;

			return text.Count();
		}

		bool DocumentModel::EditStyle(TextPos begin, TextPos end, Ptr<DocumentStyleProperties> style)
		{
			return false;
		}
	}
}