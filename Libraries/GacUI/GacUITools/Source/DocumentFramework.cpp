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
	}
}