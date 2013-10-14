/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Resource

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUIRESOURCE
#define VCZH_PRESENTATION_GUIRESOURCE

#include "GuiDocument.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

		class GuiResourceItem;
		class GuiResourceFolder;
		class GuiResource;

/***********************************************************************
Resource Image
***********************************************************************/
			
		/// <summary>
		/// Represnets an image to display.
		/// </summary>
		class GuiImageData : public Object, public Description<GuiImageData>
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
			/// <param name="path">The path.</param>
			Ptr<Object>								GetValueByPath(const WString& path);
		};

/***********************************************************************
Resource Loader
***********************************************************************/
		
		/// <summary>Resource. A resource is a root resource folder that does not have a name.</summary>
		class GuiResource : public GuiResourceFolder, public Description<GuiResource>
		{
		public:
			/// <summary>Create a resource.</summary>
			GuiResource();
			~GuiResource();

			/// <summary>Load a resource from an xml file. If the xml file refers other files, they will be loaded as well.</summary>
			/// <returns>The loaded resource.</returns>
			/// <param name="filepath">The file path of the xml file.</param>
			static Ptr<GuiResource>					LoadFromXml(const WString& filePath);
			
			/// <summary>Get a contained document model using a path like "Packages\Application\Name". If the path does not exists or the type does not match, an exception will be thrown.</summary>
			/// <returns>The containd resource object.</returns>
			/// <param name="path">The path.</param>
			Ptr<DocumentModel>						GetDocumentByPath(const WString& path);
			/// <summary>Get a contained image using a path like "Packages\Application\Name". If the path does not exists or the type does not match, an exception will be thrown.</summary>
			/// <returns>The containd resource object.</returns>
			/// <param name="path">The path.</param>
			Ptr<GuiImageData>						GetImageByPath(const WString& path);
			/// <summary>Get a contained xml using a path like "Packages\Application\Name". If the path does not exists or the type does not match, an exception will be thrown.</summary>
			/// <returns>The containd resource object.</returns>
			/// <param name="path">The path.</param>
			Ptr<parsing::xml::XmlDocument>			GetXmlByPath(const WString& path);
			/// <summary>Get a contained string object using a path like "Packages\Application\Name". If the path does not exists or the type does not match, an exception will be thrown.</summary>
			/// <returns>The containd resource object.</returns>
			/// <param name="path">The path.</param>
			WString									GetStringByPath(const WString& path);
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

/***********************************************************************
Parser Loader
***********************************************************************/

		/// <summary>Parser manager for caching parsing table globally.</summary>
		class IParserManager : public IDescriptable, public Description<IParserManager>
		{
		protected:
			typedef parsing::tabling::ParsingTable			Table;

		public:
			/// <summary>Get a parsing table by name.</summary>
			/// <returns>The parsing table.</returns>
			/// <param name="name">The name.</param>
			virtual Ptr<Table>						GetParsingTable(const WString& name)=0;
			/// <summary>Set a parsing table loader by name.</summary>
			/// <param name="name">The name.</param>
			/// <param name="loader">The parsing table loader.</param>
			virtual bool							SetParsingTable(const WString& name, Func<Ptr<Table>()> loader)=0;
		};

		/// <summary>Get the global <see cref="IParserManager"/> object.</summary>
		extern IParserManager*						GetParserManager();
	}
}

#endif