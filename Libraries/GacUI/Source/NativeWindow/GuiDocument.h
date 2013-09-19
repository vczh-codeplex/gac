/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Resource

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUIDOCUMENT
#define VCZH_PRESENTATION_GUIDOCUMENT

#include "GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

		class DocumentTextRun;
		class DocumentStylePropertiesRun;
		class DocumentStyleApplicationRun;
		class DocumentHyperlinkTextRun;
		class DocumentImageRun;
		class DocumentTemplateApplicationRun;
		class DocumentTemplateContentRun;
		class DocumentParagraphRun;

/***********************************************************************
Rich Content Document (style)
***********************************************************************/

		/// <summary>Represents a text style.</summary>
		class DocumentStyleProperties : public Object, public Description<DocumentStyleProperties>
		{
		public:
			/// <summary>Font face.</summary>
			Nullable<WString>				face;
			/// <summary>Font size.</summary>
			Nullable<vint>					size;
			/// <summary>Font color.</summary>
			Nullable<Color>					color;
			/// <summary>Bold.</summary>
			Nullable<bool>					bold;
			/// <summary>Italic.</summary>
			Nullable<bool>					italic;
			/// <summary>Underline.</summary>
			Nullable<bool>					underline;
			/// <summary>Strikeline.</summary>
			Nullable<bool>					strikeline;
			/// <summary>Antialias.</summary>
			Nullable<bool>					antialias;
			/// <summary>Vertical antialias.</summary>
			Nullable<bool>					verticalAntialias;
		};

/***********************************************************************
Rich Content Document (run)
***********************************************************************/

		/// <summary>Pepresents a logical run of a rich content document.</summary>
		class DocumentRun : public Object, public Description<DocumentRun>
		{
		public:
			static const vint				NullHyperlinkId = -1;

			/// <summary>A visitor interface for <see cref="DocumentRun"/>.</summary>
			class IVisitor : public Interface
			{
			public:
				/// <summary>Visit operation for <see cref="DocumentTextRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentTextRun* run)=0;
				/// <summary>Visit operation for <see cref="DocumentStylePropertiesRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentStylePropertiesRun* run)=0;
				/// <summary>Visit operation for <see cref="DocumentStyleApplicationRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentStyleApplicationRun* run)=0;
				/// <summary>Visit operation for <see cref="DocumentHyperlinkTextRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentHyperlinkTextRun* run)=0;
				/// <summary>Visit operation for <see cref="DocumentImageRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentImageRun* run)=0;
				/// <summary>Visit operation for <see cref="DocumentTemplateApplicationRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentTemplateApplicationRun* run)=0;
				/// <summary>Visit operation for <see cref="DocumentTemplateContentRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentTemplateContentRun* run)=0;
				/// <summary>Visit operation for <see cref="DocumentParagraphRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentParagraphRun* run)=0;
			};
			

			DocumentRun(){}

			/// <summary>Accept a <see cref="IVisitor"/> and trigger the selected visit operation.</summary>
			/// <param name="visitor">The visitor.</param>
			virtual void					Accept(IVisitor* visitor)=0;
		};
		
		/// <summary>Pepresents a container run.</summary>
		class DocumentContainerRun : public DocumentRun, public Description<DocumentContainerRun>
		{
			typedef collections::List<Ptr<DocumentRun>>			RunList;
		public:
			/// <summary>Sub runs.</summary>
			RunList							runs;
		};

		/// <summary>Pepresents a text run.</summary>
		class DocumentTextRun : public DocumentRun, public Description<DocumentTextRun>
		{
		public:
			/// <summary>Run text.</summary>
			WString							text;

			DocumentTextRun(){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};
				
		/// <summary>Pepresents a style properties run.</summary>
		class DocumentStylePropertiesRun : public DocumentContainerRun, public Description<DocumentStylePropertiesRun>
		{
		public:
			/// <summary>Style properties.</summary>
			Ptr<DocumentStyleProperties>	style;

			DocumentStylePropertiesRun(){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};
				
		/// <summary>Pepresents a style application run.</summary>
		class DocumentStyleApplicationRun : public DocumentContainerRun, public Description<DocumentStyleApplicationRun>
		{
		public:
			/// <summary>Style name.</summary>
			WString							styleName;

			DocumentStyleApplicationRun(){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};
		
		/// <summary>Pepresents a hyperlink text run.</summary>
		class DocumentHyperlinkTextRun : public DocumentStyleApplicationRun, public Description<DocumentHyperlinkTextRun>
		{
		public:
			/// <summary>Style name for normal state.</summary>
			WString							normalStyleName;
			/// <summary>Style name for active state.</summary>
			WString							activeStyleName;
			/// <summary>Id for hyperlink.</summary>
			vint							hyperlinkId;

			DocumentHyperlinkTextRun():hyperlinkId(NullHyperlinkId){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};
				
		/// <summary>Pepresents a inline object run.</summary>
		class DocumentInlineObjectRun : public DocumentRun, public Description<DocumentInlineObjectRun>
		{
		public:
			/// <summary>Size of the inline object.</summary>
			Size							size;
			/// <summary>Baseline of the inline object.</summary>
			vint							baseline;

			DocumentInlineObjectRun():baseline(-1){}
		};
				
		/// <summary>Pepresents a image run.</summary>
		class DocumentImageRun : public DocumentInlineObjectRun, public Description<DocumentImageRun>
		{
		public:
			/// <summary>The image.</summary>
			Ptr<INativeImage>				image;
			/// <summary>The frame index.</summary>
			vint							frameIndex;
			/// <summary>The image source string.</summary>
			WString							source;

			DocumentImageRun():frameIndex(0){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};
				
		/// <summary>Pepresents a template application run.</summary>
		class DocumentTemplateApplicationRun : public DocumentContainerRun, public Description<DocumentStyleApplicationRun>
		{
			typedef collections::Dictionary<WString, WString>		AttributeMap;
		public:
			/// <summary>Template name.</summary>
			WString							templateName;
			/// <summary>Template attributes.</summary>
			AttributeMap					attributes;

			DocumentTemplateApplicationRun(){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};
				
		/// <summary>Pepresents a template content run in a <see cref="DocumentTemplateApplicationRun"/>.</summary>
		class DocumentTemplateContentRun : public DocumentContainerRun, public Description<DocumentTemplateContentRun>
		{
		public:
			DocumentTemplateContentRun(){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};
				
		/// <summary>Pepresents a paragraph run.</summary>
		class DocumentParagraphRun : public DocumentContainerRun, public Description<DocumentTemplateContentRun>
		{
		public:
			/// <summary>Paragraph alignment.</summary>
			Alignment						alignment;

			DocumentParagraphRun():alignment(Alignment::Left){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

/***********************************************************************
Rich Content Document (resolver)
***********************************************************************/

		/// <summary>Represents a symbol resolver for loading a document model.</summary>
		class DocumentResolver : public Object
		{
		private:
			Ptr<DocumentResolver>			previousResolver;

		protected:

			virtual Ptr<INativeImage>		ResolveImageInternal(const WString& protocol, const WString& path)=0;
		public:
			/// <summary>Create a document resolver.</summary>
			/// <param name="_previousResolver">A previous resolver. When loading a resource failed, the resolver will try to invoke the previous resolver for loading the resource.</param>
			DocumentResolver(Ptr<DocumentResolver> _previousResolver);
			~DocumentResolver();

			/// <summary>Load an image when the [F:vl.presentation.DocumentImageRun.source] of [T:vl.presentation.DocumentImageRun] is something like a protocol-prefixed uri.</summary>
			/// <returns>The loaded image. Returns null if failed to load.</returns>
			/// <param name="protocol">The protocol.</param>
			/// <param name="path">The path.</param>
			Ptr<INativeImage>				ResolveImage(const WString& protocol, const WString& path);
		};

/***********************************************************************
Rich Content Document (model)
***********************************************************************/

		/// <summary>Represents a text style.</summary>
		class DocumentStyle : public Object, public Description<DocumentStyle>
		{
		public:
			/// <summary>Parent style name, could be #Default, #Context, #NormalLink, #ActiveLink or style name of others</summary>
			WString							parentStyleName;

			/// <summary>Properties of this style.</summary>
			Ptr<DocumentStyleProperties>	styles;

			/// <summary>Resolved properties of this style using parent styles.</summary>
			Ptr<DocumentStyleProperties>	resolvedStyles;
		};

		/// <summary>Represents a template.</summary>
		class DocumentTemplate : public Object, public Description<DocumentTemplate>
		{
		public:
			Ptr<parsing::xml::XmlElement>	templateDescription;
		};

		/// <summary>Represents a rich content document model.</summary>
		class DocumentModel : public Object, public Description<DocumentModel>
		{
		public:

			/// <summary>Store some necessary internal information for a hyperlink.</summary>
			struct HyperlinkInfo
			{
				/// <summary>The href attribute of the hyperlink.</summary>
				WString						reference;
				/// <summary>The index of the paragraph that contains this hyperlink.</summary>
				vint						paragraphIndex;

				HyperlinkInfo()
					:paragraphIndex(-1)
				{
				}
			};
		private:
			typedef collections::List<Ptr<DocumentParagraphRun>>						ParagraphList;
			typedef collections::Pair<FontProperties, Color>							RawStylePair;
			typedef collections::Dictionary<WString, Ptr<DocumentStyle>>				StyleMap;
			typedef collections::Dictionary<WString, Ptr<DocumentTemplate>>				TemplateMap;
			typedef collections::Dictionary<vint, HyperlinkInfo>						HyperlinkMap;
		public:
			/// <summary>All paragraphs.</summary>
			ParagraphList					paragraphs;
			/// <summary>All available styles. These will not be persistant.</summary>
			StyleMap						styles;
			/// <summary>All available templates. These will not be persistant.</summary>
			TemplateMap						templates;
			/// <summary>Link content. This map stores all "href" value for all hyperlinks.</summary>
			HyperlinkMap					hyperlinkInfos;
			
			DocumentModel();

			RawStylePair					GetStyle(Ptr<DocumentStyleProperties> sp, const RawStylePair& context);
			RawStylePair					GetStyle(const WString& styleName, const RawStylePair& context);
			vint							ActivateHyperlink(vint hyperlinkId, bool active);

			/// <summary>Load a document model from an xml.</summary>
			/// <returns>The loaded document model.</returns>
			/// <param name="xml">The xml document.</param>
			/// <param name="resolver">A document resolver to resolve symbols in non-embedded objects like image.</param>
			static Ptr<DocumentModel>		LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, Ptr<DocumentResolver> resolver);

			/// <summary>Load a document model from an xml.</summary>
			/// <returns>The loaded document model.</returns>
			/// <param name="xml">The xml document.</param>
			/// <param name="workingDirectory">The working directory for loading image files.</param>
			static Ptr<DocumentModel>		LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, const WString& workingDirectory);

			/// <summary>Save a document model to an xml.</summary>
			/// <returns>The saved xml document.</returns>
			Ptr<parsing::xml::XmlDocument>	SaveToXml();
		};
	}
}

#endif