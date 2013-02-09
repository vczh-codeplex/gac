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

/***********************************************************************
Operations
***********************************************************************/

		class IEditorSelectionOperation;

		class IEditorSelectionCallback : public Interface
		{
		public:
			virtual void					OnStateUpdated(IEditorSelectionOperation* sender)=0;
		};

		class IEditorSelectionOperation : public IDocumentEditorOperation
		{
		public:
			static const wchar_t*			OperationTypeId;

			WString							GetOperationTypeId();

			virtual bool					Initialize(IEditorSelectionCallback* callback)=0;
			virtual bool					IsInitialized()=0;

			virtual bool					CanUndo()=0;
			virtual bool					CanRedo()=0;
			virtual bool					CanCut()=0;
			virtual bool					CanCopy()=0;
			virtual bool					CanPaste()=0;
			virtual bool					CanDelete()=0;
			virtual bool					CanSelectAll()=0;

			virtual bool					PerformUndo()=0;
			virtual bool					PerformRedo()=0;
			virtual bool					PerformCut()=0;
			virtual bool					PerformCopy()=0;
			virtual bool					PerformPaste()=0;
			virtual bool					PerformDelete()=0;
			virtual bool					PerformSelectAll()=0;
		};

/***********************************************************************
Package
***********************************************************************/

		class EditPackage : public Object, public IDocumentPackage, protected IEditorSelectionCallback
		{
		protected:
			DocumentToolstripCommand*		commandEditUndo;
			DocumentToolstripCommand*		commandEditRedo;
			DocumentToolstripCommand*		commandEditCut;
			DocumentToolstripCommand*		commandEditCopy;
			DocumentToolstripCommand*		commandEditPaste;
			DocumentToolstripCommand*		commandEditDelete;
			DocumentToolstripCommand*		commandEditSelectAll;
			IDocumentEditor*				currentEditor;

			void							EditUndo(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditRedo(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditCut(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditCopy(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditPaste(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditDelete(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							EditSelectAll(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void							OnStateUpdated(IEditorSelectionOperation* sender);
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