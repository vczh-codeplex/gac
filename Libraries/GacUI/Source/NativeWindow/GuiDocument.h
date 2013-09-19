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
		class DocumentHyperlinkTextRun;
		class DocumentImageRun;

/***********************************************************************
Rich Content Document (model)
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
				/// <summary>Visit operation for <see cref="DocumentHyperlinkTextRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentHyperlinkTextRun* run)=0;
				/// <summary>Visit operation for <see cref="DocumentImageRun"/>.</summary>
				/// <param name="run">The run object.</param>
				virtual void				Visit(DocumentImageRun* run)=0;
			};
			
			/// <summary>Id for hyperlink. Set to -1 to make this run not a hyperlink</summary>
			vint							hyperlinkId;

			DocumentRun():hyperlinkId(NullHyperlinkId){}

			/// <summary>Accept a <see cref="IVisitor"/> and trigger the selected visit operation.</summary>
			/// <param name="visitor">The visitor.</param>
			virtual void					Accept(IVisitor* visitor)=0;
		};
				
		/// <summary>Pepresents a text run.</summary>
		class DocumentTextRun : public DocumentRun, public Description<DocumentTextRun>
		{
		public:
			/// <summary>Run font and style.</summary>
			FontProperties					style;
			/// <summary>Run color.</summary>
			Color							color;
			/// <summary>Run text.</summary>
			WString							text;

			DocumentTextRun(){}

			void							Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};
		
		/// <summary>Pepresents a hyperlink text run.</summary>
		class DocumentHyperlinkTextRun : public DocumentTextRun, public Description<DocumentHyperlinkTextRun>
		{
		public:
			/// <summary>Run font and style.</summary>
			FontProperties					normalStyle;
			/// <summary>Run color.</summary>
			Color							normalColor;
			/// <summary>Run font and style.</summary>
			FontProperties					activeStyle;
			/// <summary>Run color.</summary>
			Color							activeColor;

			DocumentHyperlinkTextRun(){}

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

		//--------------------------------------------------------------------------

		/// <summary>Represents a logical line of a rich content document.</summary>
		class DocumentLine : public Object, public Description<DocumentLine>
		{
			typedef collections::List<Ptr<DocumentRun>>			RunList;
		public:
			/// <summary>All runs in this paragraph.</summary>
			RunList							runs;
		};

		/// <summary>Represents a logical paragraph of a rich content document.</summary>
		class DocumentParagraph : public Object, public Description<DocumentParagraph>
		{
			typedef collections::List<Ptr<DocumentLine>>		LineList;
		public:
			/// <summary>All lines in this paragraph.</summary>
			LineList						lines;

			/// <summary>Horizontal alignment of this paragraph.</summary>
			Alignment					alignment;

			DocumentParagraph():alignment(Alignment::Left){}
		};

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

		//--------------------------------------------------------------------------

		/// <summary>Represents a text style.</summary>
		class DocumentStyle : public Object
		{
		public:
			/// <summary>Parent style name, could be #Default, #Context, #NormalLink, #ActiveLink or style name of others</summary>
			WString							parentStyleName;

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
			typedef collections::List<Ptr<DocumentParagraph>>							ParagraphList;
			typedef collections::Dictionary<WString, Ptr<DocumentStyle>>				StyleMap;
			typedef collections::Dictionary<WString, Ptr<parsing::xml::XmlElement>>		TemplateMap;
			typedef collections::Pair<FontProperties, Color>							RawStylePair;
			typedef collections::Dictionary<vint, HyperlinkInfo>						HyperlinkMap;
		public:

			/// <summary>All paragraphs in this document.</summary>
			ParagraphList					paragraphs;
			/// <summary>All available styles. These will not be persistant.</summary>
			StyleMap						styles;
			/// <summary>All available templates. These will not be persistant.</summary>
			TemplateMap						templates;
			/// <summary>Link content. This map stores all "href" value for all hyperlinks.</summary>
			HyperlinkMap					hyperlinkInfos;
			
			DocumentModel();

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
			Ptr<parsing::xml::XmlDocument>	SaveToXml(bool persistMetadata);
		};
	}
}

#endif