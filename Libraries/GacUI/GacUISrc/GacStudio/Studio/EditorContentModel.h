/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacStudio::EditorContentModel

Interfaces:
***********************************************************************/

#include "..\UI\GacStudioUI.h"

using namespace vl::collections;

namespace vm
{
	class EditorFileContentModelBase;

	class EditorContentModelBase : public Object, public virtual IEditorContentModel
	{
		friend class EditorFileContentModelBase;
	protected:
		enum ContentState
		{
			Ready,
			BaseContentChanged,
			SubContentChanged,
		};

		Ptr<IEditorContentFactoryModel>				contentFactory;
		EditorContentModelBase*						baseContent = nullptr;
		EditorContentModelBase*						subContent = nullptr;
		ContentState								state = Ready;
		description::Value							persisted;
		IEditorModel*								currentEditor = nullptr;

		virtual description::Value					FromBase(description::Value base) = 0;
		virtual description::Value					ToBase(description::Value sub) = 0;

		bool										TryGetEditingContentFromBaseContent(description::Value& content);
		bool										TryGetEditingContentFromSubContent(description::Value& content);
		virtual void								OnBasePersistedContentChanged();
		virtual void								OnSubPersistedContentChanged();
	public:
		EditorContentModelBase(Ptr<IEditorContentFactoryModel> _contentFactory, EditorContentModelBase* _baseContent);
		~EditorContentModelBase();

		Ptr<IEditorContentFactoryModel>				GetContentFactory()override;
		IEditorContentModel*						GetBaseContent()override;
		IEditorContentModel*						GetSubContent()override;
		description::Value							GetPersistedContent()override;
		description::Value							GetEditingContent()override;
		IEditorModel*								GetCurrentEditor()override;
		void										Persist(description::Value content)override;
		void										BeginEdit(IEditorModel* editor)override;
		void										EndEdit()override;
	};

#pragma warning(push)
#pragma warning(disable:4250)
	class EditorFileContentModelBase : public EditorContentModelBase, public virtual IEditorFileContentModel
	{
	protected:
		WString										fileName;

		virtual description::Value					LoadFromFile(const WString& _fileName) = 0;
		virtual void								SaveToFile(const WString& _fileName, description::Value content) = 0;

		description::Value							FromBase(description::Value base)override;
		description::Value							ToBase(description::Value sub)override;
		void										OnSubPersistedContentChanged()override;
	public:
		EditorFileContentModelBase(Ptr<IEditorContentFactoryModel> _contentFactory);
		~EditorFileContentModelBase();
		
		description::Value							GetPersistedContent()override;
		WString										GetFileName()override;
		void										LoadFile(WString _fileName)override;
		void										RenameFile(WString _fileName)override;
	};
#pragma warning(pop)

}