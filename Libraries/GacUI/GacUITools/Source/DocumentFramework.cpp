#include "DocumentFramework.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentToolstripCommand
***********************************************************************/

		DocumentToolstripCommand::DocumentToolstripCommand(const WString& _packageId, const WString& _commandId)
			:packageId(_packageId)
			,commandId(_commandId)
		{
		}

		DocumentToolstripCommand::~DocumentToolstripCommand()
		{
		}

		const WString& DocumentToolstripCommand::GetPackageId()
		{
			return packageId;
		}

		const WString& DocumentToolstripCommand::GetCommandId()
		{
			return commandId;
		}

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
Package Loader
***********************************************************************/

		DocumentPackageLoader* globalFirstDocumentPackageLoader=0;
		DocumentPackageLoader* globalLastDocumentPackageLoader=0;

		void InstallDocumentPackageLoader(DocumentPackageLoader* loader)
		{
			if(globalFirstDocumentPackageLoader==0)
			{
				globalFirstDocumentPackageLoader=loader;
				globalLastDocumentPackageLoader=0;
			}
			else
			{
				globalLastDocumentPackageLoader->nextLoader=loader;
				globalLastDocumentPackageLoader=loader;
			}
		}

		Ptr<DocumentPackageLoader> RetriveDocumentPackageLoader()
		{
			DocumentPackageLoader* loader=globalFirstDocumentPackageLoader;
			if(!loader) return 0;
			if(globalFirstDocumentPackageLoader==globalLastDocumentPackageLoader)
			{
				globalFirstDocumentPackageLoader=0;
				globalLastDocumentPackageLoader=0;
			}
			else
			{
				globalFirstDocumentPackageLoader=loader->nextLoader;
			}
			loader->nextLoader=0;
			return loader;
		}

/***********************************************************************
IDs
***********************************************************************/
		
		const wchar_t* IFileDialogService::ServiceTypeId = L"vl::gactools::IFileDialogService";
		const wchar_t* IEditingDocumentService::ServiceTypeId = L"vl::gactools::IEditingDocumentService";
	}
}