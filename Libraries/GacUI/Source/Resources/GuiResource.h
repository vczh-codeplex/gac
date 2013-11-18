/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Resource

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_RESOURCES_GUIRESOURCE
#define VCZH_PRESENTATION_RESOURCES_GUIRESOURCE

#include "..\NativeWindow\GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

		class GuiResourceItem;
		class GuiResourceFolder;
		class GuiResource;

/***********************************************************************
Helper Functions
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
		/// <summary>Test is a text a resource url and extract the protocol and the path.</summary>
		/// <returns>Returns true if the text is a resource url.</returns>
		/// <param name="text">The text.</param>
		/// <param name="protocol">The extracted protocol.</param>
		/// <param name="path">The extracted path.</param>
		extern bool									IsResourceUrl(const WString& text, WString& protocol, WString& path);

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
Resource String
***********************************************************************/


		/// <summary>Represents a text resource.</summary>
		class GuiTextData : public Object, public Description<GuiTextData>
		{
		protected:
			WString							text;

		public:
			/// <summary>Create an empty text data.</summary>
			GuiTextData();
			/// <summary>Create a text data with a specified text.</summary>
			/// <param name="_text">The specified text.</param>
			GuiTextData(const WString& _text);
			
			/// <summary>Get the specified text.</summary>
			/// <returns>The specified text.</returns>
			WString							GetText();
		};

/***********************************************************************
Resource Structure
***********************************************************************/

		/// <summary>Resource node base.</summary>
		class GuiResourceNodeBase : public Object, public Description<GuiResourceNodeBase>
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

		class DocumentModel;
		
		/// <summary>Resource item.</summary>
		class GuiResourceItem : public GuiResourceNodeBase, public Description<GuiResourceItem>
		{
			friend class GuiResourceFolder;
		protected:
			Ptr<DescriptableObject>					content;
			
		public:
			/// <summary>Create a resource item.</summary>
			GuiResourceItem();
			~GuiResourceItem();
			
			/// <summary>Get the contained object for this resource item.</summary>
			/// <returns>The contained object.</returns>
			Ptr<DescriptableObject>					GetContent();
			/// <summary>Set the containd object for this resource item.</summary>
			/// <param name="value">The contained object.</param>
			void									SetContent(Ptr<DescriptableObject> value);

			/// <summary>Get the contained object as an image.</summary>
			/// <returns>The contained object.</returns>
			Ptr<GuiImageData>						AsImage();
			/// <summary>Get the contained object as an xml.</summary>
			/// <returns>The contained object.</returns>
			Ptr<parsing::xml::XmlDocument>			AsXml();
			/// <summary>Get the contained object as a string.</summary>
			/// <returns>The contained object.</returns>
			Ptr<GuiTextData>						AsString();
			/// <summary>Get the contained object as a document model.</summary>
			/// <returns>The contained object.</returns>
			Ptr<DocumentModel>						AsDocument();
		};
		
		/// <summary>Resource folder. A resource folder contains many sub folders and sub items.</summary>
		class GuiResourceFolder : public GuiResourceNodeBase, public Description<GuiResourceFolder>
		{
		protected:
			typedef collections::Dictionary<WString, Ptr<GuiResourceItem>>		ItemMap;
			typedef collections::Dictionary<WString, Ptr<GuiResourceFolder>>	FolderMap;
			typedef collections::List<Ptr<GuiResourceItem>>						ItemList;
			typedef collections::List<Ptr<GuiResourceFolder>>					FolderList;

			struct DelayLoading
			{
				WString								type;
				WString								workingDirectory;
				Ptr<GuiResourceItem>				preloadResource;
			};

			typedef collections::List<DelayLoading>								DelayLoadingList;

			ItemMap									items;
			FolderMap								folders;

			void									LoadResourceFolderXml(DelayLoadingList& delayLoadings, const WString& containingFolder, Ptr<parsing::xml::XmlElement> folderXml);
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
			Ptr<DescriptableObject>					GetValueByPath(const WString& path);
			/// <summary>Get a resource folder using a path like "Packages\Application\Name\".</summary>
			/// <returns>The resource folder.</returns>
			/// <param name="path">The path.</param>
			Ptr<GuiResourceFolder>					GetFolderByPath(const WString& path);
		};

/***********************************************************************
Resource
***********************************************************************/
		
		/// <summary>Resource. A resource is a root resource folder that does not have a name.</summary>
		class GuiResource : public GuiResourceFolder, public Description<GuiResource>
		{
		protected:
			WString									workingDirectory;
		public:
			/// <summary>Create a resource.</summary>
			GuiResource();
			~GuiResource();

			/// <summary>Get the directory where the resource is load.</summary>
			/// <returns>The directory.</returns>
			WString									GetWorkingDirectory();

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
Resource Path Resolver
***********************************************************************/

		/// <summary>Represents a symbol resolver for loading a resource of a certain protocol.</summary>
		class IGuiResourcePathResolver : public IDescriptable, public Description<IGuiResourcePathResolver>
		{
		public:
			/// <summary>Load a resource when the descriptor is something like a protocol-prefixed uri.</summary>
			/// <returns>The loaded resource. Returns null if failed to load.</returns>
			/// <param name="path">The path.</param>
			virtual Ptr<DescriptableObject>					ResolveResource(const WString& path)=0;
		};

		/// <summary>Represents an <see cref="IGuiResourcePathResolver"/> factory.</summary>
		class IGuiResourcePathResolverFactory : public IDescriptable, public Description<IGuiResourcePathResolverFactory>
		{
		public:
			/// <summary>Get the protocol for this resolver.</summary>
			/// <returns>The protocol.</returns>
			virtual WString									GetProtocol()=0;

			/// <summary>Create an <see cref="IGuiResourcePathResolver"/> object.</summary>
			/// <returns>The created resolver.</returns>
			/// <param name="resource">The resource context.</param>
			/// <param name="workingDirectory">The working directory context.</param>
			virtual Ptr<IGuiResourcePathResolver>			CreateResolver(Ptr<GuiResource> resource, const WString& workingDirectory)=0;
		};
		
		/// <summary>Represents a symbol resolver for loading a resource.</summary>
		class GuiResourcePathResolver : public Object, public Description<GuiResourcePathResolver>
		{
			typedef collections::Dictionary<WString, Ptr<IGuiResourcePathResolver>>		ResolverMap;
		protected:
			ResolverMap										resolvers;
			Ptr<GuiResource>								resource;
			WString											workingDirectory;

		public:
			/// <summary>Create a resolver.</summary>
			/// <param name="_resource">The resource context.</param>
			/// <param name="_workingDirectory">The working directory context.</param>
			GuiResourcePathResolver(Ptr<GuiResource> _resource, const WString& _workingDirectory);
			~GuiResourcePathResolver();

			/// <summary>Load a resource when the descriptor is something like a protocol-prefixed uri.</summary>
			/// <returns>The loaded resource. Returns null if failed to load.</returns>
			/// <param name="protocol">The protocol.</param>
			/// <param name="path">The path.</param>
			Ptr<DescriptableObject>							ResolveResource(const WString& protocol, const WString& path);
		};

/***********************************************************************
Resource Type Resolver
***********************************************************************/

		/// <summary>Represents a symbol type for loading a resource.</summary>
		class IGuiResourceTypeResolver : public IDescriptable, public Description<IGuiResourceTypeResolver>
		{
		public:
			/// <summary>Get the type of the resource that load by this resolver.</summary>
			/// <returns>The type.</returns>
			virtual WString									GetType()=0;
			/// <summary>Get the preload type to load the resource before loading itself.</summary>
			/// <returns>The preload type. Returns an empty string to indicate that there is no preload type for this resolver.</returns>
			virtual WString									GetPreloadType()=0;
			/// <summary>Get the delay load feature for this resolver.</summary>
			/// <returns>Returns true if this type need to delay load.</returns>
			virtual bool									IsDelayLoad()=0;

			/// <summary>Load a resource for a type inside an xml element.</summary>
			/// <returns>The resource.</returns>
			/// <param name="element">The xml element.</param>
			virtual Ptr<DescriptableObject>					ResolveResource(Ptr<parsing::xml::XmlElement> element)=0;

			/// <summary>Load a resource for a type from a file.</summary>
			/// <returns>The resource.</returns>
			/// <param name="path">The file path.</param>
			virtual Ptr<DescriptableObject>					ResolveResource(const WString& path)=0;

			/// <summary>Load a resource for a type from a resource loaded by the preload type resolver.</summary>
			/// <returns>The resource.</returns>
			/// <param name="resource">The resource.</param>
			/// <param name="resolver">The path resolver. This is only for delay load resource.</param>
			virtual Ptr<DescriptableObject>					ResolveResource(Ptr<DescriptableObject> resource, Ptr<GuiResourcePathResolver> resolver)=0;
		};

/***********************************************************************
Resource Resolver Manager
***********************************************************************/

		/// <summary>A resource resolver manager.</summary>
		class IGuiResourceResolverManager : public IDescriptable, public Description<IGuiResourceResolverManager>
		{
		public:
			/// <summary>Get the <see cref="IGuiResourcePathResolverFactory"/> for a protocol.</summary>
			/// <returns>The factory.</returns>
			/// <param name="protocol">The protocol.</param>
			virtual IGuiResourcePathResolverFactory*		GetPathResolverFactory(const WString& protocol)=0;
			/// <summary>Set the <see cref="IGuiResourcePathResolverFactory"/> for a protocol.</summary>
			/// <returns>Returns true if this operation succeeded.</returns>
			/// <param name="factory">The factory.</param>
			virtual bool									SetPathResolverFactory(Ptr<IGuiResourcePathResolverFactory> factory)=0;
			/// <summary>Get the <see cref="IGuiResourceTypeResolver"/> for a resource type.</summary>
			/// <returns>The resolver.</returns>
			/// <param name="type">The resource type.</param>
			virtual IGuiResourceTypeResolver*				GetTypeResolver(const WString& type)=0;
			/// <summary>Set the <see cref="IGuiResourceTypeResolver"/> for a resource type.</summary>
			/// <returns>Returns true if this operation succeeded.</returns>
			/// <param name="resolver">The resolver.</param>
			virtual bool									SetTypeResolver(Ptr<IGuiResourceTypeResolver> resolver)=0;
		};
		
		extern IGuiResourceResolverManager*					GetResourceResolverManager();
	}
}

#endif