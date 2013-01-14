/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTEDITORFACTORY
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTEDITORFACTORY

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentEditorFactory : public Object, public IDocumentEditorFactory
		{
		private:
			WString								id;
			WString								friendlyName;
			SortedList<WString>					supportedViewTypeIds;
			List<ICallback*>					callbacks;

		protected:

			bool								AddSupportedViewTypeId(const WString& viewTypeId);
			virtual Ptr<IDocumentEditor>		CreateEditorInternal(IDocumentView* view)=0;
		public:
			DocumentEditorFactory(const WString& _id, const WString& _friendlyName);
			~DocumentEditorFactory();

			bool								AttachCallback(ICallback* callback)override;
			bool								DetachCallback(ICallback* callback)override;

			WString								GetEditorTypeId()override;
			WString								GetEditorTypeFriendlyName()override;
			vint								GetSupportedViewTypeCount()override;
			WString								GetSupportedViewTypeId(vint index)override;
			bool								IsSupportedViewTypeId(const WString& viewTypeId)override;
			Ptr<IDocumentEditor>				CreateEditor(IDocumentView* view)override;
		};
	}
}

#endif