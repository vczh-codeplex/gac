/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI Tools::Document Framework

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK
#define GACUI_TOOLS_DOCUMENTFRAMEWORK

#include "..\..\Public\Source\GacUI.h"

using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::xml;
using namespace vl::parsing::json;
using namespace vl::parsing::tabling;

namespace vl
{
	namespace gactools
	{
		class IDocumentOperation;
		class IDocumentView;
		class IDocumentFragment;
		class IDocumentContainer;
		class IDocumentFileType;
		class IDocumentEditor;
		class IDocumentEditorFactory;
		class IDocumentManager;

/***********************************************************************
Document Interfaces
***********************************************************************/

		class IDocumentView : public Interface
		{
		public:
			class ICallback : public Interface
			{
			public:
				virtual void					OnAttach(IDocumentView* sender)=0;
				virtual void					OnDetach(IDocumentView* sender)=0;
				virtual void					OnViewUpdated(IDocumentView* sender)=0;
				virtual void					OnViewDestroyed(IDocumentView* sender)=0;
				virtual void					OnBeginEdit(IDocumentView* sender)=0;
				virtual void					OnFinishEdit(IDocumentView* sender)=0;
			};
			virtual bool						AttachCallback(ICallback* callback)=0;
			virtual bool						DetachCallback(ICallback* callback)=0;

			virtual IDocumentFragment*			GetOwnedFragment()=0;
			virtual WString						GetViewTypeId()=0;
			virtual WString						GetViewTypeFriendlyName()=0;
			virtual bool						IsReadOnlyView()=0;

			virtual bool						BeginEdit(IDocumentEditor* editor)=0;
			virtual bool						FinishEdit(IDocumentEditor* editor)=0;
			virtual IDocumentEditor*			GetEditor()=0;
		};

		class IDocumentFragment : public Interface
		{
		public:
			class ICallback : public Interface
			{
			public:
				virtual void					OnAttach(IDocumentFragment* sender)=0;
				virtual void					OnDetach(IDocumentFragment* sender)=0;
				virtual void					OnFilePathUpdated(IDocumentFragment* sender)=0;
				virtual void					OnFragmentUpdated(IDocumentFragment* sender)=0;
				virtual void					OnFragmentDestroyed(IDocumentFragment* sender)=0;
			};
			virtual bool						AttachCallback(ICallback* callback)=0;
			virtual bool						DetachCallback(ICallback* callback)=0;

			virtual IDocumentContainer*			GetOwnedContainer()=0;
			virtual IDocumentFragment*			GetOwnedFragment()=0;
			virtual vint						GetSubFragmentCount()=0;
			virtual IDocumentFragment*			GetSubFragment(vint index)=0;

			virtual bool						IsStoredInSeparatedFile()=0;
			virtual bool						CanSaveSeparately()=0;
			virtual bool						CanSaveToAnotherFile()=0;
			virtual WString						GetFilePath()=0;
			virtual bool						ReloadDocument()=0;
			virtual bool						SaveDocument()=0;
			virtual bool						SaveDocumentAs(const WString& filePath)=0;
			virtual bool						IsModified()=0;
			virtual WString						GetFriendlyName()=0;

			virtual vint						GetSupportedViewTypeCount()=0;
			virtual WString						GetSupportedViewTypeId(vint index)=0;
			virtual bool						IsSupportedViewTypeId(const WString& viewTypeId)=0;
			virtual WString						GetDefaultViewTypeId()=0;
			virtual IDocumentView*				GetView(const WString& viewTypeId)=0;

			template<typename T>
			T* GetView()
			{
				return dynamic_cast<T*>(GetView(T::ViewTypeId));
			}
		};

		class IDocumentContainer : public Interface
		{
		public:
			class ICallback : public Interface
			{
			public:
				virtual void					OnAttach(IDocumentContainer* sender)=0;
				virtual void					OnDetach(IDocumentContainer* sender)=0;
				virtual void					OnDocumentDestroyed(IDocumentContainer* sender)=0;
			};
			virtual bool						AttachCallback(ICallback* callback)=0;
			virtual bool						DetachCallback(ICallback* callback)=0;

			virtual IDocumentFragment*			GetRootFragment()=0;
		};

		class IDocumentFileType : public Interface
		{
		public:
			class ICallback : public Interface
			{
			public:
				virtual void					OnAttach(IDocumentFileType* sender)=0;
				virtual void					OnDetach(IDocumentFileType* sender)=0;
				virtual void					OnDocumentCreated(IDocumentFileType* sender, IDocumentContainer* document)=0;
			};
			virtual bool						AttachCallback(ICallback* callback)=0;
			virtual bool						DetachCallback(ICallback* callback)=0;

			virtual WString						GetFileExtension()=0;
			virtual WString						GetFileTypeId()=0;
			virtual WString						GetFileTypeFriendlyName()=0;
			virtual Ptr<IDocumentContainer>		CreateDocument()=0;
			virtual Ptr<IDocumentContainer>		CreateDocumentFromFile(const WString& filePath)=0;
		};

/***********************************************************************
Editor Interfaces
***********************************************************************/

		class IDocumentOperation : public Interface
		{
		public:
			virtual IDocumentView*				GetOwnedView()=0;
			virtual WString						GetOperationTypeId()=0;
		};

		class IDocumentEditor : public Interface
		{
		public:
			class ICallback : public Interface
			{
			public:
				virtual void					OnAttach(IDocumentEditor* sender)=0;
				virtual void					OnDetach(IDocumentEditor* sender)=0;
				virtual void					OnEditorUpdated(IDocumentEditor* sender)=0;
				virtual void					OnEditorDestroyed(IDocumentEditor* sender)=0;
				virtual void					OnFinishEdit(IDocumentEditor* sender)=0;
				virtual void					OnLostActiveView(IDocumentEditor* sender)=0;
			};
			virtual bool						AttachCallback(ICallback* callback)=0;
			virtual bool						DetachCallback(ICallback* callback)=0;

			virtual IDocumentEditorFactory*		GetOwnedFactory()=0;
			virtual GuiControl*					GetEditorControl()=0;
			virtual IDocumentView*				GetEditingView()=0;
			virtual bool						FinishEdit()=0;
			virtual bool						IsAvailable()=0;

			virtual vint						GetSupportedOperationTypeCount()=0;
			virtual WString						GetSupportedOperationType(vint index)=0;
			virtual bool						IsSupportedOperationTypeId(const WString& operationTypeId)=0;
			virtual IDocumentOperation*			GetOperation(const WString& operationTypeId)=0;

			template<typename T>
			T* GetOperation()
			{
				return dynamic_cast<T*>(GetOperation(T::OperationTypeId));
			}
		};

		class IDocumentEditorFactory : public Interface
		{
		public:
			class ICallback : public Interface
			{
			public:
				virtual void					OnAttach(IDocumentEditorFactory* sender)=0;
				virtual void					OnDetach(IDocumentEditorFactory* sender)=0;
				virtual void					OnEditorCreated(IDocumentEditorFactory* sender, IDocumentEditor* editor)=0;
			};
			virtual bool						AttachCallback(ICallback* callback)=0;
			virtual bool						DetachCallback(ICallback* callback)=0;

			virtual WString						GetEditorTypeId()=0;
			virtual WString						GetEditorTypeFriendlyName()=0;
			virtual vint						GetSupportedViewTypeCount()=0;
			virtual WString						GetSupportedViewTypeId(vint index)=0;
			virtual bool						IsSupportedViewTypeId(const WString& viewTypeId)=0;
			virtual Ptr<IDocumentEditor>		CreateEditor(IDocumentView* view)=0;
		};

/***********************************************************************
Package Interfaces
***********************************************************************/

		class DocumentToolstripCommand : public GuiToolstripCommand
		{
		private:
			WString								packageId;
			WString								commandId;

		public:
			DocumentToolstripCommand(const WString& _packageId, const WString& _commandId);
			~DocumentToolstripCommand();

			const WString&						GetPackageId();
			const WString&						GetCommandId();
		};

		class IDocumentService : public Interface
		{
		public:
			virtual WString						GetServiceTypeId()=0;
		};

		class IDocumentPackage : public Interface
		{
		public:
			virtual WString						GetPackageId()=0;
			virtual void						OnBeforeInit(){};
			virtual void						OnAfterInit(){};
			virtual void						OnCurrentEditorUpdated(IDocumentEditor* editor){};
			virtual void						OnInstallCommand(DocumentToolstripCommand* command){};
		};

/***********************************************************************
Manager Interfaces
***********************************************************************/

		class IDocumentManager : public Interface
		{
		public:
			class ICallback : public Interface
			{
			public:
				virtual void					OnAttach(IDocumentManager* sender)=0;
				virtual void					OnDetach(IDocumentManager* sender)=0;
				virtual void					OnFileTypeRegistered(IDocumentManager* sender, IDocumentFileType* fileType)=0;
				virtual void					OnEditorFactoryRegistered(IDocumentManager* sender, IDocumentEditorFactory* editorFactory)=0;
			};
			virtual bool						AttachCallback(ICallback* callback)=0;
			virtual bool						DetachCallback(ICallback* callback)=0;

			virtual bool						RegisterFileType(Ptr<IDocumentFileType> fileType)=0;
			virtual vint						GetFileTypeCount()=0;
			virtual IDocumentFileType*			GetFileType(vint index)=0;
			virtual IDocumentFileType*			FindFileTypeById(const WString& fileTypeId)=0;
			virtual IDocumentFileType*			FindFileTypeByExtension(const WString& fileExtension)=0;

			virtual bool						RegisterEditorFactory(Ptr<IDocumentEditorFactory> editorFactory)=0;
			virtual vint						GetEditorFactoryCount()=0;
			virtual IDocumentEditorFactory*		GetEditorFactory(vint index)=0;
			virtual IDocumentEditorFactory*		FindEditorFactoryById(const WString& editorTypeId)=0;

			virtual bool						BindDefaultEditor(const WString& viewTypeId, IDocumentEditorFactory* editorFactory)=0;
			virtual bool						UnbindDefaultEditor(const WString& viewTypeId)=0;
			virtual WString						GetDefaultEditorTypeId(const WString& viewTypeId)=0;

			virtual bool						RegisterPackage(Ptr<IDocumentPackage> package)=0;
			virtual vint						GetPackageCount()=0;
			virtual IDocumentPackage*			GetPackage(vint index)=0;
			virtual IDocumentPackage*			GetPackageById(const WString& packageId)=0;
			virtual bool						RegisterService(Ptr<IDocumentService> service)=0;
			virtual IDocumentService*			GetServiceById(const WString& serviceTypeId)=0;

			template<typename T>
			T* GetPackage()
			{
				return dynamic_cast<T*>(GetPackageById(T::PackageId));
			}

			template<typename T>
			T* GetService()
			{
				return dynamic_cast<T*>(GetServiceById(T::ServiceTypeId));
			}
		};

		extern IDocumentManager*				GetDocumentManager();
		extern void								SetDocumentManager(IDocumentManager* documentManager);

/***********************************************************************
Package Loader
***********************************************************************/

		class DocumentPackageLoader : public Object
		{
		public:
			WString								packageId;
			DocumentPackageLoader*				nextLoader;

			DocumentPackageLoader()
				:nextLoader(0)
			{
			};

			virtual IDocumentPackage*			LoadPackage()=0;
		};

		template<typename T>
		class StrongTypeDocumentPackageLoader : public DocumentPackageLoader
		{
		public:
			StrongTypeDocumentPackageLoader()
			{
				packageId=T::PackageId;
			}

			IDocumentPackage* LoadPackage()override
			{
				return new T;
			};
		};

		extern void								InstallDocumentPackageLoader(DocumentPackageLoader* loader);
		extern Ptr<DocumentPackageLoader>		RetriveDocumentPackageLoader();

#define INSTALL_PACKAGE(PACKAGETYPE)\
		namespace\
		{\
			class PACKAGETYPE##Initializer\
			{\
			public:\
				PACKAGETYPE##Initializer()\
				{\
					InstallDocumentPackageLoader(new StrongTypeDocumentPackageLoader<PACKAGETYPE>);\
				}\
			} PACKAGETYPE##initializer;\
		}\

/***********************************************************************
Common Services
***********************************************************************/

		class IFileDialogService : public IDocumentService
		{
		public:
			static const wchar_t*				ServiceTypeId;
			WString								GetServiceTypeId(){return ServiceTypeId;}

			virtual bool						CreateFileDialog(const WString& dialogId, const WString& text)=0;
			virtual bool						AddDialogItem(const WString& dialogId, const WString& text, const WString& filter, vint priority)=0;
			virtual WString						OpenDialogForSingleFile(const WString& dialogId)=0;
			virtual WString						SaveDialogForSingleFile(const WString& dialogId)=0;
		};

		class IEditingDocumentService : public IDocumentService
		{
		public:
			static const wchar_t*				ServiceTypeId;
			WString								GetServiceTypeId(){return ServiceTypeId;}

			virtual IDocumentEditor*			NewDocument(const WString& fileTypeId, const WString& editorTypeId, bool promptDialog)=0;
			virtual IDocumentEditor*			LoadDocumentFromView(IDocumentView* view, const WString& editorTypeId, bool promptDialog)=0;
			virtual IDocumentEditor*			LoadDocumentFromContainer(Ptr<IDocumentContainer> document, const WString& editorTypeId, bool promptDialog)=0;
			virtual IDocumentEditor*			LoadDocumentFromFile(const WString& filePath, const WString& editorTypeId, bool promptDialog)=0;
			virtual IDocumentEditor*			LoadDocumentByDialog(const WString& dialogId, const WString& editorTypeId, bool promptDialog)=0;
			virtual bool						SaveDocumentByDialog(IDocumentEditor* editor, const WString& dialogId, bool promptDialog)=0;
			virtual bool						CloseEditor(IDocumentEditor* editor, bool promptDialog)=0;

			virtual vint						GetActiveEditorCount()=0;
			virtual IDocumentEditor*			GetActiveEditor(vint index)=0;
			virtual vint						GetActiveDocumentCount()=0;
			virtual IDocumentContainer*			GetActiveDocument(vint index)=0;
			virtual IDocumentEditor*			GetCurrentEditor()=0;
		};
	}
}

#endif