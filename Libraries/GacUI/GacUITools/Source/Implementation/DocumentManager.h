/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTMANAGER
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTMANAGER

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentManager : public Object, public IDocumentManager
		{
		private:
			Dictionary<WString, Ptr<IDocumentEditorFactory>>		editorFactoriesById;
			Dictionary<WString, Ptr<IDocumentFileType>>				editorFileTypesById;
			Dictionary<WString, Ptr<IDocumentFileType>>				editorFileTypesByExt;
			Dictionary<WString, Ptr<IDocumentEditorFactory>>		defaultEditors;
			List<ICallback*>										callbacks;

		public:
			DocumentManager();
			~DocumentManager();

			bool						AttachCallback(ICallback* callback)override;
			bool						DetachCallback(ICallback* callback)override;

			bool						RegisterFileType(Ptr<IDocumentFileType> fileType)override;
			vint						GetFileTypeCount()override;
			IDocumentFileType*			GetFileType(vint index)override;
			IDocumentFileType*			FindFileTypeById(const WString& fileTypeId)override;
			IDocumentFileType*			FindFileTypeByExtension(const WString& fileExtension)override;

			bool						RegisterEditorFactory(Ptr<IDocumentEditorFactory> editorFactory)override;
			vint						GetEditorFactoryCount()override;
			IDocumentEditorFactory*		GetEditorFactory(vint index)override;
			IDocumentEditorFactory*		FindEditorFactoryById(const WString& editorTypeId)override;

			bool						BindDefaultEditor(const WString& viewTypeId, IDocumentEditorFactory* editorFactory)override;
			bool						UnbindDefaultEditor(const WString& viewTypeId)override;
			IDocumentEditorFactory*		GetDefaultEditor(const WString& viewTypeId)override;
		};
	}
}

#endif