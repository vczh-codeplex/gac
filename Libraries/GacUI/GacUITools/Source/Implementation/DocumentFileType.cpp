#include "DocumentFileType.h"
#include "DocumentContainer.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentFileType
***********************************************************************/

		DocumentFileType::DocumentFileType(const WString& _extension, const WString& _id, const WString& _friendlyName)
			:extension(_extension)
			,id(_id)
			,friendlyName(_friendlyName)
		{
		}

		DocumentFileType::~DocumentFileType()
		{
		}

		bool DocumentFileType::AttachCallback(ICallback* callback)
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

		bool DocumentFileType::DetachCallback(ICallback* callback)
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

		WString DocumentFileType::GetFileExtension()
		{
			return extension;
		}

		WString DocumentFileType::GetFileTypeId()
		{
			return id;
		}

		WString DocumentFileType::GetFileTypeFriendlyName()
		{
			return friendlyName;
		}

		Ptr<IDocumentContainer> DocumentFileType::CreateDocument()
		{
			Ptr<DocumentContainer> document=new DocumentContainer();
			Ptr<IDocumentFragment> fragment=NewDocumentFragment(document.Obj());
			if(!fragment) return 0;
			document->SetRootFragment(fragment);
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnDocumentCreated(this, document.Obj());
			}
			return document;
		}

		Ptr<IDocumentContainer> DocumentFileType::CreateDocumentFromFile(const WString& filePath)
		{
			Ptr<DocumentContainer> document=new DocumentContainer();
			Ptr<IDocumentFragment> fragment=LoadDocumentFragment(filePath, document.Obj());
			if(!fragment) return 0;
			document->SetRootFragment(fragment);
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnDocumentCreated(this, document.Obj());
			}
			return document;
		}
	}
}