#include "GuiDocument.h"
#include "../../../../Common/Source/Stream/FileStream.h"
#include "../../../../Common/Source/Stream/MemoryStream.h"
#include "../../../../Common/Source/Stream/Accessor.h"
#include "../../../../Common/Source/Stream/CharFormat.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing::tabling;
		using namespace parsing::xml;
		using namespace regex;

/***********************************************************************
DocumentImageRun
***********************************************************************/

		const wchar_t* DocumentImageRun::RepresentationText=L"[Image]";

/***********************************************************************
ExtractTextVisitor
***********************************************************************/

		namespace document_operation_visitors
		{
			class ExtractTextVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				stream::TextWriter&				writer;
				bool							skipNonTextContent;

				ExtractTextVisitor(stream::TextWriter& _writer, bool _skipNonTextContent)
					:writer(_writer)
					,skipNonTextContent(_skipNonTextContent)
				{
				}

				void VisitContainer(DocumentContainerRun* run)
				{
					FOREACH(Ptr<DocumentRun>, subRun, run->runs)
					{
						subRun->Accept(this);
					}
				}

				void VisitContent(DocumentContentRun* run)
				{
					writer.WriteString(run->GetRepresentationText());
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

				void Visit(DocumentHyperlinkRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentImageRun* run)override
				{
					if(!skipNonTextContent)
					{
						VisitContent(run);
					}
				}

				void Visit(DocumentParagraphRun* run)override
				{
					VisitContainer(run);
				}
			};
		}
		using namespace document_operation_visitors;

/***********************************************************************
DocumentParagraphRun
***********************************************************************/

		WString DocumentParagraphRun::GetText(bool skipNonTextContent)
		{
			stream::MemoryStream memoryStream;
			{
				stream::StreamWriter writer(memoryStream);
				GetText(writer, skipNonTextContent);
			}

			memoryStream.SeekFromBegin(0);
			stream::StreamReader reader(memoryStream);
			return reader.ReadToEnd();
		}

		void DocumentParagraphRun::GetText(stream::TextWriter& writer, bool skipNonTextContent)
		{
			ExtractTextVisitor visitor(writer, skipNonTextContent);
			Accept(&visitor);
		}

/***********************************************************************
DocumentModel
***********************************************************************/

		const wchar_t* DocumentModel::DefaultStyleName		= L"#Default";
		const wchar_t* DocumentModel::SelectionStyleName	= L"#Selection";
		const wchar_t* DocumentModel::ContextStyleName		= L"#Context";
		const wchar_t* DocumentModel::NormalLinkStyleName	= L"#NormalLink";
		const wchar_t* DocumentModel::ActiveLinkStyleName	= L"#ActiveLink";

		DocumentModel::DocumentModel()
		{
			{
				FontProperties font=GetCurrentController()->ResourceService()->GetDefaultFont();
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->face=font.fontFamily;
				sp->size=font.size;
				sp->color=Color();
				sp->backgroundColor=Color(0, 0, 0, 0);
				sp->bold=font.bold;
				sp->italic=font.italic;
				sp->underline=font.underline;
				sp->strikeline=font.strikeline;
				sp->antialias=font.antialias;
				sp->verticalAntialias=font.verticalAntialias;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->styles=sp;
				styles.Add(L"#Default", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->color=Color(255, 255, 255);
				sp->backgroundColor=Color(51, 153, 255);

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->styles=sp;
				styles.Add(L"#Selection", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->styles=sp;
				styles.Add(L"#Context", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->color=Color(0, 0, 255);
				sp->underline=true;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->parentStyleName=L"#Context";
				style->styles=sp;
				styles.Add(L"#NormalLink", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->color=Color(0, 0, 255);
				sp->underline=true;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->parentStyleName=L"#Context";
				style->styles=sp;
				styles.Add(L"#ActiveLink", style);
			}
		}

		void DocumentModel::MergeStyle(Ptr<DocumentStyleProperties> style, Ptr<DocumentStyleProperties> parent)
		{
			if(!style->face					&& parent->face)				style->face					=parent->face;
			if(!style->size					&& parent->size)				style->size					=parent->size;
			if(!style->color				&& parent->color)				style->color				=parent->color;
			if(!style->backgroundColor		&& parent->backgroundColor)		style->backgroundColor		=parent->backgroundColor;
			if(!style->bold					&& parent->bold)				style->bold					=parent->bold;
			if(!style->italic				&& parent->italic)				style->italic				=parent->italic;
			if(!style->underline			&& parent->underline)			style->underline			=parent->underline;
			if(!style->strikeline			&& parent->strikeline)			style->strikeline			=parent->strikeline;
			if(!style->antialias			&& parent->antialias)			style->antialias			=parent->antialias;
			if(!style->verticalAntialias	&& parent->verticalAntialias)	style->verticalAntialias	=parent->verticalAntialias;
		}

		void DocumentModel::MergeBaselineStyle(Ptr<DocumentModel> baselineDocument, const WString& styleName)
		{
			auto indexSrc = baselineDocument->styles.Keys().IndexOf(styleName);
			if (indexSrc == -1)
			{
				return;
			}

			auto indexDst = styles.Keys().IndexOf(styleName);
			auto csp = baselineDocument->styles.Values()[indexSrc]->styles;
			Ptr<DocumentStyleProperties> sp = new DocumentStyleProperties(*csp.Obj());
			if (indexDst != -1)
			{
				MergeStyle(sp, styles.Values()[indexDst]->styles);
			}

			if (indexDst == -1)
			{
				auto style = new DocumentStyle;
				style->styles = sp;
				styles.Add(styleName, style);
			}
			else
			{
				styles.Values()[indexDst]->styles = sp;
			}

			FOREACH(Ptr<DocumentStyle>, style, styles.Values())
			{
				style->resolvedStyles = nullptr;
			}
		}

		void DocumentModel::MergeBaselineStyles(Ptr<DocumentModel> baselineDocument)
		{
			MergeBaselineStyle(baselineDocument, DefaultStyleName);
			MergeBaselineStyle(baselineDocument, SelectionStyleName);
			MergeBaselineStyle(baselineDocument, ContextStyleName);
			MergeBaselineStyle(baselineDocument, NormalLinkStyleName);
			MergeBaselineStyle(baselineDocument, ActiveLinkStyleName);
		}

		DocumentModel::ResolvedStyle DocumentModel::GetStyle(Ptr<DocumentStyleProperties> sp, const ResolvedStyle& context)
		{
			FontProperties font;
			font.fontFamily			=sp->face				?sp->face.Value()				:context.style.fontFamily;
			font.size				=sp->size				?sp->size.Value()				:context.style.size;
			font.bold				=sp->bold				?sp->bold.Value()				:context.style.bold;
			font.italic				=sp->italic				?sp->italic.Value()				:context.style.italic;
			font.underline			=sp->underline			?sp->underline.Value()			:context.style.underline;
			font.strikeline			=sp->strikeline			?sp->strikeline.Value()			:context.style.strikeline;
			font.antialias			=sp->antialias			?sp->antialias.Value()			:context.style.antialias;
			font.verticalAntialias	=sp->verticalAntialias	?sp->verticalAntialias.Value()	:context.style.verticalAntialias;
			Color color				=sp->color				?sp->color.Value()				:context.color;
			Color backgroundColor	=sp->backgroundColor	?sp->backgroundColor.Value()	:context.backgroundColor;
			return ResolvedStyle(font, color, backgroundColor);
		}

		DocumentModel::ResolvedStyle DocumentModel::GetStyle(const WString& styleName, const ResolvedStyle& context)
		{
			Ptr<DocumentStyle> selectedStyle;
			{
				vint index=styles.Keys().IndexOf(styleName);
				if(index!=-1)
				{
					selectedStyle=styles.Values()[index];
				}
				else
				{
					selectedStyle=styles[L"#Default"];
				}
			}

			if(!selectedStyle->resolvedStyles)
			{
				Ptr<DocumentStyleProperties> sp = new DocumentStyleProperties;
				selectedStyle->resolvedStyles = sp;

				Ptr<DocumentStyle> currentStyle;
				WString currentName = styleName;
				while(true)
				{
					vint index = styles.Keys().IndexOf(currentName);
					if (index == -1) break;
					currentStyle = styles.Values().Get(index);
					currentName = currentStyle->parentStyleName;
					MergeStyle(sp, currentStyle->styles);
				}
			}

			Ptr<DocumentStyleProperties> sp=selectedStyle->resolvedStyles;
			return GetStyle(sp, context);
		}

		WString DocumentModel::GetText(bool skipNonTextContent)
		{
			stream::MemoryStream memoryStream;
			{
				stream::StreamWriter writer(memoryStream);
				GetText(writer, skipNonTextContent);
			}

			memoryStream.SeekFromBegin(0);
			stream::StreamReader reader(memoryStream);
			return reader.ReadToEnd();
		}

		void DocumentModel::GetText(stream::TextWriter& writer, bool skipNonTextContent)
		{
			for(vint i=0;i<paragraphs.Count();i++)
			{
				Ptr<DocumentParagraphRun> paragraph=paragraphs[i];
				paragraph->GetText(writer, skipNonTextContent);
				if(i<paragraphs.Count()-1)
				{
					writer.WriteString(L"\r\n\r\n");
				}
			}
		}
	}
}