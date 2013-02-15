/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Resource

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUIRESOURCE
#define VCZH_PRESENTATION_GUIRESOURCE

#include "GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

		class GuiResourceItem;
		class GuiResourceFolder;
		class GuiResource;

		class DocumentTextRun;
		class DocumentHyperlinkTextRun;
		class DocumentImageRun;

/***********************************************************************
Resource Image
***********************************************************************/
			
		/// <summary>
		/// Represnets an image to display.
		/// </summary>
		class GuiImageData : public Object
		{
		protected:
			Ptr<INativeImage>				image;
			vint							frameIndex;

		public:
			/// <summary>Create an empty image data.</summary>
			GuiImageData();
			/// <summary>Create an image data with a specified image and a frame index.</summary>
			/// <param name="_image">The specified image.</param>
			/// <param name="_frameIndex">The specified frame index.</param>
			GuiImageData(Ptr<INativeImage> _image, vint _frameIndex);
			~GuiImageData();

			/// <summary>Get the specified image.</summary>
			/// <returns>The specified image.</returns>
			Ptr<INativeImage>				GetImage();
			/// <summary>Get the specified frame index.</summary>
			/// <returns>The specified frame index.</returns>
			vint							GetFrameIndex();
		};

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

			struct HyperlinkInfo
			{
				WString						reference;
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

/***********************************************************************
Rich Content Document (resolver)
***********************************************************************/
		
		/// <summary>Document resolver: image loader for "file" protocol.</summary>
		class DocumentFileProtocolResolver : public DocumentResolver
		{
		protected:
			WString							workingDirectory;

			Ptr<INativeImage>				ResolveImageInternal(const WString& protocol, const WString& path)override;
		public:
			/// <summary>Create a document resolver.</summary>
			/// <param name="_workingDirectory">Specify a working directory when the file path is a relative path.</param>
			/// <param name="previousResolver">The previous resolver. See <see cref="DocumentResolver"/> for details.</param>
			DocumentFileProtocolResolver(const WString& _workingDirectory, Ptr<DocumentResolver> previousResolver=0);
		};
		
		/// <summary>Document resolver: image loader for "res" protocol.</summary>
		class DocumentResProtocolResolver : public DocumentResolver
		{
		protected:
			Ptr<GuiResource>				resource;

			Ptr<INativeImage>				ResolveImageInternal(const WString& protocol, const WString& path)override;
		public:
			/// <summary>Create a document resolver.</summary>
			/// <param name="_resource">The resource that contains images for retriving by path.</param>
			/// <param name="previousResolver">The previous resolver. See <see cref="DocumentResolver"/> for details.</param>
			DocumentResProtocolResolver(Ptr<GuiResource> _resource, Ptr<DocumentResolver> previousResolver=0);
		};

/***********************************************************************
Resource Structure
***********************************************************************/

		/// <summary>Resource node base.</summary>
		class GuiResourceNodeBase : public Object
		{
			friend class GuiResourceFolder;
		protected:
			GuiResourceFolder*						parent;
			WString									name;
			
		public:
			GuiResourceNodeBase();
			~GuiResourceNodeBase();

			/// <summary>Get the containing folder. Returns null means that this is the root resource node.</summary>
			/// <returns>The containing folder.</returns>
			GuiResourceFolder*						GetParent();
			/// <summary>Get the name of this resource node.</summary>
			/// <returns>The name of this resource node .</returns>
			const WString&							GetName();
		};
		
		/// <summary>Resource item.</summary>
		class GuiResourceItem : public GuiResourceNodeBase
		{
			friend class GuiResourceFolder;
		protected:
			Ptr<Object>								content;
			
		public:
			/// <summary>Create a resource item.</summary>
			GuiResourceItem();
			~GuiResourceItem();
			
			/// <summary>Get the contained object for this resource item.</summary>
			/// <returns>The contained object.</returns>
			Ptr<Object>								GetContent();
			/// <summary>Set the containd object for this resource item.</summary>
			/// <param name="value">The contained object.</param>
			void									SetContent(Ptr<Object> value);

			/// <summary>Get the contained object as an image.</summary>
			/// <returns>The contained object.</returns>
			Ptr<GuiImageData>						AsImage();
			/// <summary>Get the contained object as an xml.</summary>
			/// <returns>The contained object.</returns>
			Ptr<parsing::xml::XmlDocument>			AsXml();
			/// <summary>Get the contained object as a string.</summary>
			/// <returns>The contained object.</returns>
			Ptr<ObjectBox<WString>>					AsString();
			/// <summary>Get the contained object as a document model.</summary>
			/// <returns>The contained object.</returns>
			Ptr<DocumentModel>						AsDocument();
		};
		
		/// <summary>Resource folder. A resource folder contains many sub folders and sub items.</summary>
		class GuiResourceFolder : public GuiResourceNodeBase
		{
		protected:
			typedef collections::Dictionary<WString, Ptr<GuiResourceItem>>		ItemMap;
			typedef collections::Dictionary<WString, Ptr<GuiResourceFolder>>	FolderMap;
			typedef collections::List<Ptr<GuiResourceItem>>						ItemList;
			typedef collections::List<Ptr<GuiResourceFolder>>					FolderList;

			struct DelayLoading
			{
				collections::Dictionary<Ptr<GuiResourceItem>, WString>			documentModelFolders;
			};

			ItemMap									items;
			FolderMap								folders;

			void									LoadResourceFolderXml(DelayLoading& delayLoading, const WString& containingFolder, Ptr<parsing::xml::XmlElement> folderXml, Ptr<parsing::tabling::ParsingTable> xmlParsingTable);
		public:
			/// <summary>Create a resource folder.</summary>
			GuiResourceFolder();
			~GuiResourceFolder();

			/// <summary>Get all sub items.</summary>
			/// <returns>All sub items.</returns>
			const ItemList&							GetItems();
			/// <summary>Get the item of a specified name.</summary>
			/// <returns>The item of a specified name.</returns>
			/// <param name="name">The specified name.</param>
			Ptr<GuiResourceItem>					GetItem(const WString& name);
			/// <summary>Add a resource item.</summary>
			/// <returns>Returns true if this operation succeeded.</returns>
			/// <param name="name">The name of this resource item.</param>
			/// <param name="item">The resource item.</param>
			bool									AddItem(const WString& name, Ptr<GuiResourceItem> item);
			/// <summary>Remove a resource item of a specified name.</summary>
			/// <returns>Returns the removed resource item if this operation succeeded.</returns>
			/// <param name="name">The name of this resource item.</param>
			Ptr<GuiResourceItem>					RemoveItem(const WString& name);
			/// <summary>Remove all resource item.</summary>
			void									ClearItems();
			
			/// <summary>Get all sub folders.</summary>
			/// <returns>All sub folders.</returns>
			const FolderList&						GetFolders();
			/// <summary>Get the folder of a specified name.</summary>
			/// <returns>The folder of a specified name.</returns>
			/// <param name="name">The specified name.</param>
			Ptr<GuiResourceFolder>					GetFolder(const WString& name);
			/// <summary>Add a resource folder.</summary>
			/// <returns>Returns true if this operation succeeded.</returns>
			/// <param name="name">The name of this resource folder.</param>
			/// <param name="folder">The resource folder.</param>
			bool									AddFolder(const WString& name, Ptr<GuiResourceFolder> folder);
			/// <summary>Remove a resource folder of a specified name.</summary>
			/// <returns>Returns the removed resource folder if this operation succeeded.</returns>
			/// <param name="name">The name of this resource folder.</param>
			Ptr<GuiResourceFolder>					RemoveFolder(const WString& name);
			/// <summary>Remove all resource folders.</summary>
			void									ClearFolders();

			/// <summary>Get a contained resource object using a path like "Packages\Application\Name".</summary>
			/// <returns>The containd resource object.</returns>
			Ptr<Object>								GetValueByPath(const WString& path);
		};

/***********************************************************************
Resource Loader
***********************************************************************/
		
		/// <summary>Resource. A resource is a root resource folder that does not have a name.</summary>
		class GuiResource : public GuiResourceFolder
		{
		public:
			/// <summary>Create a resource.</summary>
			GuiResource();
			~GuiResource();

			/// <summary>Load a resource from an xml file. If the xml file refers other files, they will be loaded as well.</summary>
			/// <returns>The loaded resource.</returns>
			/// <param name="filepath">The file path of the xml file.</param>
			static Ptr<GuiResource>					LoadFromXml(const WString& filePath);
		};

/***********************************************************************
Resource Loader
***********************************************************************/

		/// <summary>Get the folder path from a file path. The result folder path is ended with a separator.</summary>
		/// <returns>The folder path.</returns>
		/// <param name="filePath">The file path.</param>
		extern WString								GetFolderPath(const WString& filePath);
		/// <summary>Get the file name from a file path.</summary>
		/// <returns>The file name.</returns>
		/// <param name="filePath">The file path.</param>
		extern WString								GetFileName(const WString& filePath);
		/// <summary>Load a text file.</summary>
		/// <returns>Returns true if the operation succeeded.</returns>
		/// <param name="filePath">The text file path.</param>
		/// <param name="text">The text file content, if succeeded.</param>
		extern bool									LoadTextFile(const WString& filePath, WString& text);
		/// <summary>Load the text from a stream.</summary>
		/// <returns>Returns true if the operation succeeded.</returns>
		/// <param name="stream">The stream containing text.</param>
		/// <param name="text">The text file content, if succeeded.</param>
		extern bool									LoadTextFromStream(stream::IStream& stream, WString& text);
	}
}

#endif