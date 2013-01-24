#include "FileDialogService.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
EditingDocumentService
***********************************************************************/

		WString FileDialogService::BuildFilter(Ptr<DialogDescription> dialog)
		{
			return From(dialog->filters)
				.OrderBy([](Pair<vint, WString> a, Pair<vint, WString> b)->vint
				{
					return a.key-b.key;
				})
				.Select([](Pair<vint, WString> input)
				{
					return input.value;
				})
				.Aggregate(WString::Empty, [](WString a, WString b)
				{
					return a==L""?b:a+L"|"+b;
				});
		}

		FileDialogService::FileDialogService()
		{
		}

		FileDialogService::~FileDialogService()
		{
		}

		bool FileDialogService::CreateFileDialog(const WString& dialogId, const WString& text)
		{
			if(dialogDescriptions.Keys().Contains(dialogId)) return false;
			Ptr<DialogDescription> dialog=new DialogDescription;
			dialog->id=dialogId;
			dialog->text=text;
			dialogDescriptions.Add(dialogId, dialog);
			return true;
		}

		bool FileDialogService::AddDialogItem(const WString& dialogId, const WString& text, const WString& filter, vint priority)
		{
			vint index=dialogDescriptions.Keys().IndexOf(dialogId);
			if(index==-1) return false;
			Ptr<DialogDescription> dialog=dialogDescriptions.Values().Get(index);
			dialog->filters.Add(Pair<vint, WString>(priority, text+L"|"+filter));
			return true;
		}

		WString FileDialogService::OpenDialogForSingleFile(const WString& dialogId)
		{
			vint index=dialogDescriptions.Keys().IndexOf(dialogId);
			if(index==-1) return L"";
			Ptr<DialogDescription> dialog=dialogDescriptions.Values().Get(index);
			WString filter=BuildFilter(dialog);
			if(filter==L"") return L"";

			List<WString> selectionFileNames;
			vint selectionFileIndex=0;
			if(!GetCurrentController()->DialogService()->ShowFileDialog(
				GetApplication()->GetWindows().Get(0)->GetNativeWindow(),
				selectionFileNames,
				selectionFileIndex,
				INativeDialogService::FileDialogOpen,
				dialog->text,
				L"",
				L"",
				L"",
				filter,
				(INativeDialogService::FileDialogOptions)
				( INativeDialogService::FileDialogDereferenceLinks
				| INativeDialogService::FileDialogFileMustExist
				)))
			{
				return L"";
			}
			return selectionFileNames[0];
		}

		WString FileDialogService::SaveDialogForSingleFile(const WString& dialogId)
		{
			vint index=dialogDescriptions.Keys().IndexOf(dialogId);
			if(index==-1) return L"";
			Ptr<DialogDescription> dialog=dialogDescriptions.Values().Get(index);
			WString filter=BuildFilter(dialog);
			if(filter==L"") return L"";

			List<WString> selectionFileNames;
			vint selectionFileIndex=0;
			if(!GetCurrentController()->DialogService()->ShowFileDialog(
				GetApplication()->GetWindows().Get(0)->GetNativeWindow(),
				selectionFileNames,
				selectionFileIndex,
				INativeDialogService::FileDialogSave,
				dialog->text,
				L"",
				L"",
				L"",
				filter,
				(INativeDialogService::FileDialogOptions)
				( INativeDialogService::FileDialogDereferenceLinks
				| INativeDialogService::FileDialogDirectoryMustExist
				| INativeDialogService::FileDialogPromptOverwriteFile
				)))
			{
				return L"";
			}
			return selectionFileNames[0];
		}
	}
}