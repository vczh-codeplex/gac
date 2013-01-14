#include "DocumentFileType.h"
#include "DocumentContainer.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentFileType
***********************************************************************/

		Ptr<IDocumentContainer> DocumentFileType::CreateDocumentInternal(Ptr<IDocumentFragment> fragment)
		{
			if(fragment)
			{
				Ptr<DocumentContainer> document=new DocumentContainer(fragment);
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnDocumentCreated(this, document.Obj());
				}
				return document;
			}
			else
			{
				return 0;
			}
		}

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
			return CreateDocumentInternal(NewDocumentFragment());
		}

		Ptr<IDocumentContainer> DocumentFileType::CreateDocumentFromFile(const WString& filePath)
		{
			return CreateDocumentInternal(LoadDocumentFragment(filePath));
		}
	}
}