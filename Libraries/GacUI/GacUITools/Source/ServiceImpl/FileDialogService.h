/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Service Default Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_FILEDIALOGSERVICE
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_FILEDIALOGSERVICE

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class FileDialogService : public Object, public IFileDialogService
		{
		private:
			struct DialogDescription
			{
				WString										id;
				WString										text;
				List<Pair<vint, WString>>					filters;
			};
			friend class ArrayBase<Ptr<DialogDescription>>;

			Dictionary<WString, Ptr<DialogDescription>>		dialogDescriptions;

		protected:

			WString						BuildFilter(Ptr<DialogDescription> dialog, const WString& preferredExtension);
		public:
			FileDialogService();
			~FileDialogService();

			bool						CreateFileDialog(const WString& dialogId, const WString& text)override;
			bool						AddDialogItem(const WString& dialogId, const WString& text, const WString& filter, vint priority)override;
			WString						OpenDialogForSingleFile(const WString& dialogId)override;
			WString						SaveDialogForSingleFile(const WString& dialogId, const WString& preferredExtension)override;
		};
	}
}

#endif