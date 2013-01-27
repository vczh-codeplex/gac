/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTVIEW
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTVIEW

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentFragment;

		class DocumentView : public Object, public IDocumentView
		{
		private:
			List<ICallback*>								callbacks;
			DocumentFragment*								ownedFragment;
			WString											id;
			WString											friendlyName;
			IDocumentEditor*								currentEditor;

		protected:
			const List<ICallback*>&							GetCallbacks();
		public:
			DocumentView(DocumentFragment* _ownedFragment, const WString& _id, const WString& _friendlyName);
			~DocumentView();

			virtual void									NotifyUpdateView();
			bool											AttachCallback(ICallback* callback)override;
			bool											DetachCallback(ICallback* callback)override;

			IDocumentFragment*								GetOwnedFragment()override;
			WString											GetViewTypeId()override;
			WString											GetViewTypeFriendlyName()override;
			bool											IsReadOnlyView()override;

			bool											BeginEdit(IDocumentEditor* editor)override;
			bool											FinishEdit(IDocumentEditor* editor)override;
			IDocumentEditor*								GetEditor()override;
		};
	}
}

#endif