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
			List<ICallback*>									callbacks;
			DocumentFragment*									ownedFragment;
			WString												id;
			WString												friendlyName;
			IDocumentEditor*									currentEditor;
			Dictionary<WString, Ptr<IDocumentViewOperation>>	supportedOperations;

		protected:
			const List<ICallback*>&				GetCallbacks();
			bool								AddSupportedOperation(Ptr<IDocumentViewOperation> operation);
		public:
			DocumentView(DocumentFragment* _ownedFragment, const WString& _id, const WString& _friendlyName);
			~DocumentView();

			virtual void						NotifyUpdateView(bool alsoNotifyOwnedFragment);
			virtual void						NotifySaveFragment();
			bool								AttachCallback(ICallback* callback)override;
			bool								DetachCallback(ICallback* callback)override;

			IDocumentFragment*					GetOwnedFragment()override;
			WString								GetViewTypeId()override;
			WString								GetViewTypeFriendlyName()override;
			bool								IsReadOnlyView()override;

			bool								BeginEdit(IDocumentEditor* editor)override;
			bool								FinishEdit(IDocumentEditor* editor)override;
			IDocumentEditor*					GetEditor()override;

			vint								GetSupportedOperationTypeCount()override;
			WString								GetSupportedOperationType(vint index)override;
			bool								IsSupportedOperationTypeId(const WString& operationTypeId)override;
			IDocumentViewOperation*				GetOperation(const WString& operationTypeId)override;
		};
	}
}

#endif