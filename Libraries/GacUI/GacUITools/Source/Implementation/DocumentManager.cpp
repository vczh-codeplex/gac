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

		void DocumentManager::AttachCallback(ICallback* callback)
		{
			callbacks.Add(callback);
		}

		void DocumentManager::DetachCallback(ICallback* callback)
		{
			callbacks.Remove(callback);
		}

		bool DocumentManager::RegisterFileType(Ptr<IDocumentFileType> fileType)
		{
			if(!editorFileTypesById.Keys().Contains(fileType->GetFileTypeId()) && !editorFileTypesByExt.Keys().Contains(fileType->GetFileExtension()))
			{
				editorFileTypesById.Add(fileType->GetFileTypeId(), fileType);
				editorFileTypesByExt.Add(fileType->GetFileExtension(), fileType);
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
			vint index=editorFileTypesByExt.Keys().IndexOf(fileExtension);
			return index==-1?0:GetFileType(index);
		}

		bool DocumentManager::RegisterEditorFactory(Ptr<IDocumentEditorFactory> editorFactory)
		{
			if(!editorFactoriesById.Keys().Contains(editorFactory->GetEditorTypeId()))
			{
				editorFactoriesById.Add(editorFactory->GetEditorTypeId(), editorFactory);
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

		IDocumentEditorFactory* DocumentManager::GetDefaultEditor(const WString& viewTypeId)
		{
			vint index=defaultEditors.Keys().IndexOf(viewTypeId);
			return index==-1?0:defaultEditors.Values().Get(index).Obj();
		}
	}
}