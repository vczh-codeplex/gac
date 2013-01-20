#include "DocumentManager.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentManager
***********************************************************************/

		DocumentManager::DocumentManager()
		{
		}

		DocumentManager::~DocumentManager()
		{
		}

		bool DocumentManager::AttachCallback(ICallback* callback)
		{
			if(callbacks.Contains(callback))
			{
				return false;
			}
			else
			{
				callbacks.Add(callback);
				callback->OnAttach(this);
				return true;
			}
		}

		bool DocumentManager::DetachCallback(ICallback* callback)
		{
			if(callbacks.Contains(callback))
			{
				callback->OnDetach(this);
				callbacks.Remove(callback);
				return true;
			}
			else
			{
				return false;
			}
		}

		bool DocumentManager::RegisterFileType(Ptr<IDocumentFileType> fileType)
		{
			if(!editorFileTypesById.Keys().Contains(fileType->GetFileTypeId()) && !editorFileTypesByExt.Keys().Contains(fileType->GetFileExtension()))
			{
				editorFileTypesById.Add(fileType->GetFileTypeId(), fileType);
				editorFileTypesByExt.Add(Locale::Invariant().ToUpper(fileType->GetFileExtension()), fileType);
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnFileTypeRegistered(this, fileType.Obj());
				}
				return true;
			}
			return false;
		}

		vint DocumentManager::GetFileTypeCount()
		{
			return editorFileTypesById.Count();
		}

		IDocumentFileType* DocumentManager::GetFileType(vint index)
		{
			return editorFileTypesById.Values().Get(index).Obj();
		}

		IDocumentFileType* DocumentManager::FindFileTypeById(const WString& fileTypeId)
		{
			vint index=editorFileTypesById.Keys().IndexOf(fileTypeId);
			return index==-1?0:GetFileType(index);
		}

		IDocumentFileType* DocumentManager::FindFileTypeByExtension(const WString& fileExtension)
		{
			vint index=editorFileTypesByExt.Keys().IndexOf(Locale::Invariant().ToUpper(fileExtension));
			return index==-1?0:GetFileType(index);
		}

		bool DocumentManager::RegisterEditorFactory(Ptr<IDocumentEditorFactory> editorFactory)
		{
			if(!editorFactoriesById.Keys().Contains(editorFactory->GetEditorTypeId()))
			{
				editorFactoriesById.Add(editorFactory->GetEditorTypeId(), editorFactory);
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnEditorFactoryRegistered(this, editorFactory.Obj());
				}
				return true;
			}
			return false;
		}

		vint DocumentManager::GetEditorFactoryCount()
		{
			return editorFactoriesById.Count();
		}

		IDocumentEditorFactory* DocumentManager::GetEditorFactory(vint index)
		{
			return editorFactoriesById.Values().Get(index).Obj();
		}

		IDocumentEditorFactory* DocumentManager::FindEditorFactoryById(const WString& editorTypeId)
		{
			vint index=editorFactoriesById.Keys().IndexOf(editorTypeId);
			return index==-1?0:GetEditorFactory(index);
		}

		bool DocumentManager::BindDefaultEditor(const WString& viewTypeId, IDocumentEditorFactory* editorFactory)
		{
			vint factoryIndex=editorFactoriesById.Keys().IndexOf(editorFactory->GetEditorTypeId());
			if(factoryIndex!=-1)
			{
				Ptr<IDocumentEditorFactory> editorFactoryPtr=editorFactoriesById.Values().Get(factoryIndex);
				if(editorFactoryPtr==editorFactory)
				{
					defaultEditors.Set(viewTypeId, editorFactoryPtr);
					return true;
				}
			}
			return false;
		}

		bool DocumentManager::UnbindDefaultEditor(const WString& viewTypeId)
		{
			bool exists=defaultEditors.Keys().Contains(viewTypeId);
			if(exists)
			{
				defaultEditors.Remove(viewTypeId);
			}
			return exists;
		}

		WString DocumentManager::GetDefaultEditorTypeId(const WString& viewTypeId)
		{
			vint index=defaultEditors.Keys().IndexOf(viewTypeId);
			return index==-1?L"":defaultEditors.Values().Get(index)->GetEditorTypeId();
		}

		bool DocumentManager::RegisterPackage(Ptr<IDocumentPackage> package)
		{
			if(!packages.Keys().Contains(package->GetPackageId()))
			{
				packages.Add(package->GetPackageId(), package);
			}
			return false;
		}

		IDocumentPackage* DocumentManager::GetPackage(const WString& packageId)
		{
			vint index=packages.Keys().IndexOf(packageId);
			return index==-1?0:packages.Values().Get(index).Obj();
		}

		void DocumentManager::RunPackageBeforeInitialization()
		{
			FOREACH(Ptr<IDocumentPackage>, package, packages.Values())
			{
				package->BeforeInitialization();
			}
		}

		void DocumentManager::RunPackageAfterInitialization()
		{
			FOREACH(Ptr<IDocumentPackage>, package, packages.Values())
			{
				package->AfterInitialization();
			}
		}

		bool DocumentManager::RegisterService(Ptr<IDocumentService> service)
		{
			if(!services.Keys().Contains(service->GetServiceTypeId()))
			{
				services.Add(service->GetServiceTypeId(), service);
			}
			return false;
		}

		IDocumentService* DocumentManager::GetService(const WString& serviceTypeId)
		{
			vint index=services.Keys().IndexOf(serviceTypeId);
			return index==-1?0:services.Values().Get(index).Obj();
		}
	}
}