/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTFILETYPE
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTFILETYPE

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentFileType : public Object, public IDocumentFileType
		{
		private:
			WString								extension;
			WString								id;
			WString								friendlyName;
			List<ICallback*>					callbacks;

		protected:

			virtual Ptr<IDocumentFragment>		NewDocumentFragment(IDocumentContainer* container)=0;
			virtual Ptr<IDocumentFragment>		LoadDocumentFragment(const WString& filePath, IDocumentContainer* container)=0;
		public:
			DocumentFileType(const WString& _extension, const WString& _id, const WString& _friendlyName);
			~DocumentFileType();

			bool								AttachCallback(ICallback* callback)override;
			bool								DetachCallback(ICallback* callback)override;

			WString								GetFileExtension()override;
			WString								GetFileTypeId()override;
			WString								GetFileTypeFriendlyName()override;
			Ptr<IDocumentContainer>				CreateDocument()override;
			Ptr<IDocumentContainer>				CreateDocumentFromFile(const WString& filePath)override;
		};
	}
}

#endif