/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Service Default Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_EDITINGDOCUMENTSERVICE
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_EDITINGDOCUMENTSERVICE

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class EditingDocumentService : public Object, public IEditingDocumentService, private IDocumentEditor::ICallback
		{
		private:
			List<Ptr<IDocumentEditor>>			activeEditors;
			List<Ptr<IDocumentContainer>>		activeDocuments;

			void								OnAttach(IDocumentEditor* sender)override;
			void								OnDetach(IDocumentEditor* sender)override;
			void								OnEditorUpdated(IDocumentEditor* sender)override;
			void								OnEditorDestroyed(IDocumentEditor* sender)override;
			void								OnFinishEdit(IDocumentEditor* sender)override;
			void								OnLostActiveView(IDocumentEditor* sender)override;
		protected:

			virtual bool						CanInstallNewEditor(bool promptDialog)=0;
			virtual bool						CanUninstallEditor(IDocumentEditor* editor, bool promptDialog)=0;
			virtual bool						InstallEditor(IDocumentEditor* editor)=0;
			virtual bool						UninstallEditor(IDocumentEditor* editor)=0;
		public:
			EditingDocumentService();
			~EditingDocumentService();

			IDocumentEditor*					NewDocument(const WString& fileTypeId, const WString& editorTypeId, bool promptDialog)override;
			IDocumentEditor*					LoadDocumentFromView(IDocumentView* view, const WString& editorTypeId, bool promptDialog)override;
			IDocumentEditor*					LoadDocumentFromContainer(Ptr<IDocumentContainer> document, const WString& editorTypeId, bool promptDialog)override;
			IDocumentEditor*					LoadDocumentFromFile(const WString& filePath, const WString& editorTypeId, bool promptDialog)override;
			IDocumentEditor*					LoadDocumentByDialog(const WString& dialogId, const WString& editorTypeId, bool promptDialog)override;
			bool								SaveDocumentByDialog(IDocumentEditor* editor, const WString& dialogId, bool promptDialog)override;
			bool								CloseEditor(IDocumentEditor* editor, bool promptDialog)override;

			vint								GetActiveEditorCount()override;
			IDocumentEditor*					GetActiveEditor(vint index)override;
			vint								GetActiveDocumentCount()override;
			IDocumentContainer*					GetActiveDocument(vint index)override;
		};
	}
}

#endif