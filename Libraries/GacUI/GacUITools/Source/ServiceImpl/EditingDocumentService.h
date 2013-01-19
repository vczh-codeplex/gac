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
		class EditingDocumentService : public Object, public IEditingDocumentService
		{
		private:
			List<IDocumentEditor*>		activeEditors;

		protected:

			virtual bool				CanInstallNewEditor()=0;
			virtual bool				InstallEditor(IDocumentEditor* editor)=0;
			virtual bool				UninstallEditor(IDocumentEditor* editor)=0;
		public:
			EditingDocumentService();
			~EditingDocumentService();

			bool						NewDocument(const WString& fileTypeId, const WString& editorTypeId)override;
			bool						LoadDocumentFromFile(const WString& filePath, const WString& editorTypeId)override;
			bool						LoadDocumentFromView(IDocumentView* view, const WString& editorTypeId)override;
			bool						LoadDocumentByDialog(const WString& dialogId, const WString& editorTypeId)override;

			vint						GetActiveEditorCount()override;
			IDocumentEditor*			GetActiveEditor(vint index)override;
			bool						SaveDocumentByDialog(IDocumentEditor* editor, const WString& dialogId)override;
			bool						CloseEditor(IDocumentEditor* editor)override;
		};
	}
}

#endif