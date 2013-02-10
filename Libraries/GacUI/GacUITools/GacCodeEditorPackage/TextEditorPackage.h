/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Text Editor Package

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_TEXTEDITORPACKAGE
#define GACUI_TOOLS_TEXTEDITORPACKAGE

#include "..\Source\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class TextEditorPackage : public Object, public IDocumentPackage
		{
		protected:
			IEditingDocumentService*		editingDocumentService;
			DocumentToolstripCommand*		commandGoto;

			void							NewDocument(const WString& fileTypeId);
			void							TextEditorNewText(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							TextEditorNewXml(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							TextEditorNewJson(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							TextEditorNewMoe(GuiGraphicsComposition* sender, GuiEventArgs& arguments);

			void							TextEditorQuickFind(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							TextEditorFindAll(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							TextEditorQuickReplace(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							TextEditorReplaceAll(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							TextEditorGoTo(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
		public:
			static const wchar_t*			PackageId;

			TextEditorPackage();

			WString							GetPackageId()override;
			void							OnBeforeInit()override;
			void							OnAfterInit()override;
			void							OnCurrentEditorUpdated(IDocumentEditor* editor)override;
			void							OnInstallCommand(DocumentToolstripCommand* command)override;
		};
	}
}

#endif