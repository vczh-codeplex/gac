/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Edit Package

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_EDITPACKAGE
#define GACUI_TOOLS_EDITPACKAGE

#include "..\Source\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class EditPackage : public Object, public IDocumentPackage
		{
		protected:
			DocumentToolstripCommand*		commandEditUndo;
			DocumentToolstripCommand*		commandEditRedo;
			DocumentToolstripCommand*		commandEditCut;
			DocumentToolstripCommand*		commandEditCopy;
			DocumentToolstripCommand*		commandEditPaste;
			DocumentToolstripCommand*		commandEditDelete;
			DocumentToolstripCommand*		commandEditSelectAll;

			void							EditUndo(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditRedo(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditCut(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditCopy(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditPaste(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditDelete(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditSelectAll(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
		public:
			static const wchar_t*			PackageId;

			EditPackage::EditPackage();

			WString							GetPackageId()override;
			void							OnCurrentEditorUpdated(IDocumentEditor* editor)override;
			void							OnInstallCommand(DocumentToolstripCommand* command)override;
		};
	}
}

#endif