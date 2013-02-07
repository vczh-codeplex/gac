/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Service Default Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_MESSAGEDIALOGSERVICE
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_MESSAGEDIALOGSERVICE

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class MessageDialogService : public Object, public IMessageDialogService
		{
		public:
			void						ShowMessage(const WString& message)override;
			MessageResult				ShowRequest(const WString& message)override;
			MessageResult				ShowQuestion(const WString& message)override;
			MessageResult				ShowQuestionWithCancel(const WString& message)override;
		};
	}
}

#endif