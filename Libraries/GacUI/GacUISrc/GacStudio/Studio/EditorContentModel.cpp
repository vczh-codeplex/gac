#include "EditorContentModel.h"
#include "SolutionModel.h"

namespace vm
{

/***********************************************************************
EditorContentModelBase
***********************************************************************/

	bool EditorContentModelBase::TryGetEditingContentFromBaseContent(description::Value& content)
	{
		if (currentEditor)
		{
			content = currentEditor->GetEditingContent();
			return true;
		}
		else if (baseContent)
		{
			if (baseContent->TryGetEditingContentFromBaseContent(content))
			{
				content = FromBase(content);
				return true;
			}
		}
		return false;
	}

	bool EditorContentModelBase::TryGetEditingContentFromSubContent(description::Value& content)
	{
		if (currentEditor)
		{
			content = currentEditor->GetEditingContent();
			return true;
		}
		else if (subContent)
		{
			if (subContent->TryGetEditingContentFromSubContent(content))
			{
				content = subContent->ToBase(content);
				return true;
			}
		}
		return false;
	}

	void EditorContentModelBase::OnBasePersistedContentChanged()
	{
		state = BaseContentChanged;
		PersistedContentChanged();
		if (subContent)
		{
			subContent->OnBasePersistedContentChanged();
		}
	}

	void EditorContentModelBase::OnSubPersistedContentChanged()
	{
		state = SubContentChanged;
		PersistedContentChanged();
		if (baseContent)
		{
			baseContent->OnSubPersistedContentChanged();
		}
	}

	EditorContentModelBase::EditorContentModelBase(Ptr<IEditorContentFactoryModel> _contentFactory, EditorContentModelBase* _baseContent)
		:contentFactory(_contentFactory)
	{
		if ((baseContent = _baseContent))
		{
			baseContent->subContent = this;
		}
	}

	EditorContentModelBase::~EditorContentModelBase()
	{
	}

	Ptr<IEditorContentFactoryModel> EditorContentModelBase::GetContentFactory()
	{
		return contentFactory;
	}

	IEditorContentModel* EditorContentModelBase::GetBaseContent()
	{
		return baseContent;
	}

	IEditorContentModel* EditorContentModelBase::GetSubContent()
	{
		return subContent;
	}

	description::Value EditorContentModelBase::GetPersistedContent()
	{
		switch (state)
		{
		case BaseContentChanged:
			persisted = FromBase(baseContent->GetPersistedContent());
			state = Ready;
			break;
		case SubContentChanged:
			persisted = subContent->ToBase(subContent->GetPersistedContent());
			state = Ready;
			break;
		}
		return persisted;
	}

	description::Value EditorContentModelBase::GetEditingContent()
	{
		if (currentEditor)
		{
			return currentEditor->GetEditingContent();
		}
		else
		{
			description::Value content;
			if (TryGetEditingContentFromBaseContent(content))
			{
				return content;
			}
			else if (TryGetEditingContentFromSubContent(content))
			{
				return content;
			}
			else
			{
				return description::Value();
			}
		}
	}

	IEditorModel* EditorContentModelBase::GetCurrentEditor()
	{
		return currentEditor;
	}

	void EditorContentModelBase::Persist(description::Value content)
	{
		persisted = content;
		state = Ready;
		PersistedContentChanged();
		if (baseContent)
		{
			baseContent->OnSubPersistedContentChanged();
		}
		if (subContent)
		{
			subContent->OnBasePersistedContentChanged();
		}
	}

	void EditorContentModelBase::BeginEdit(IEditorModel* editor)
	{
		if (currentEditor)
		{
			throw StudioException(L"Internal error: \"" + currentEditor->GetEditorFactory()->GetName() + L"\" has already been opened for this item.", true);
		}
		currentEditor = editor;
		CurrentEditorChanged();
	}

	void EditorContentModelBase::EndEdit()
	{
		currentEditor = nullptr;
		CurrentEditorChanged();
	}

/***********************************************************************
EditorFileContentModelBase
***********************************************************************/

	description::Value EditorFileContentModelBase::FromBase(description::Value base)
	{
		throw StudioException(L"Internal error: \"" + contentFactory->GetName() + L"\" doesn't have a base content.", true);
	}

	description::Value EditorFileContentModelBase::ToBase(description::Value sub)
	{
		throw StudioException(L"Internal error: \"" + contentFactory->GetName() + L"\" doesn't have a base content.", true);
	}

	void EditorFileContentModelBase::OnSubPersistedContentChanged()
	{

	}

	EditorFileContentModelBase::EditorFileContentModelBase(Ptr<IEditorContentFactoryModel> _contentFactory)
		:EditorContentModelBase(_contentFactory, nullptr)
	{
	}

	EditorFileContentModelBase::~EditorFileContentModelBase()
	{
	}

	description::Value EditorFileContentModelBase::GetPersistedContent()
	{
		if (state == BaseContentChanged)
		{
			persisted = LoadFromFile(fileName);
			state = Ready;
		}
		return EditorContentModelBase::GetPersistedContent();
	}

	WString EditorFileContentModelBase::GetFileName()
	{
		return fileName;
	}

	void EditorFileContentModelBase::LoadFile(WString _fileName)
	{
		fileName = _fileName;
		state = BaseContentChanged;
		if (subContent)
		{
			subContent->OnSubPersistedContentChanged();
		}
	}

	void EditorFileContentModelBase::RenameFile(WString _fileName)
	{
		fileName = _fileName;
	}

/***********************************************************************
UnsupportedEditorContentModel
***********************************************************************/

	description::Value UnsupportedEditorContentModel::FromBase(description::Value base)
	{
		throw StudioException(contentFactory->GetName() + L" is not supported.", true);
	}

	description::Value UnsupportedEditorContentModel::ToBase(description::Value sub)
	{
		throw StudioException(contentFactory->GetName() + L" is not supported.", true);
	}

	UnsupportedEditorContentModel::UnsupportedEditorContentModel(Ptr<IEditorContentFactoryModel> _contentFactory, EditorContentModelBase* _baseContent)
		:EditorContentModelBase(_contentFactory, _baseContent)
	{
	}

	UnsupportedEditorContentModel::~UnsupportedEditorContentModel()
	{
	}

/***********************************************************************
UnsupportedEditorFileContentModel
***********************************************************************/

	description::Value UnsupportedEditorFileContentModel::LoadFromFile(const WString& _fileName)
	{
		throw StudioException(contentFactory->GetName() + L" is not supported.", true);
	}

	void UnsupportedEditorFileContentModel::SaveToFile(const WString& _fileName, description::Value content)
	{
		throw StudioException(contentFactory->GetName() + L" is not supported.", true);
	}

	UnsupportedEditorFileContentModel::UnsupportedEditorFileContentModel(Ptr<IEditorContentFactoryModel> _contentFactory)
		:EditorFileContentModelBase(_contentFactory)
	{
	}

	UnsupportedEditorFileContentModel::~UnsupportedEditorFileContentModel()
	{
	}
}