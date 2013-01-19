#include "DocumentFramework.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
Helper Functions
***********************************************************************/

		IDocumentManager* globalDocumentManagerObject=0;

		IDocumentManager* GetDocumentManager()
		{
			return globalDocumentManagerObject;
		}

		void SetDocumentManager(IDocumentManager* documentManager)
		{
			globalDocumentManagerObject=documentManager;
		}

/***********************************************************************
IDs
***********************************************************************/
		
		const wchar_t* IFileDialogService::ServiceTypeId = L"vl::gactools::IFileDialogService";
		const wchar_t* IEditingDocumentService::ServiceTypeId = L"vl::gactools::IEditingDocumentService";
	}
}