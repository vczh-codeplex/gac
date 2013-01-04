/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSASYNCSERVICE
#define VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSASYNCSERVICE

#include "..\..\GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WindowsAsyncService : public INativeAsyncService
			{
			public:
				struct TaskItem
				{
					Semaphore*							semaphore;
					INativeAsyncService::AsyncTaskProc*	proc;
					void*								argument;

					TaskItem();
					TaskItem(Semaphore* _semaphore, INativeAsyncService::AsyncTaskProc* _proc, void* _argument);
					~TaskItem();

					bool operator==(const TaskItem& item)const{return false;}
					bool operator!=(const TaskItem& item)const{return true;}
				};
			protected:
				vint								mainThreadId;
				SpinLock						taskListLock;
				collections::List<TaskItem>		taskItems;
			public:
				WindowsAsyncService();
				~WindowsAsyncService();

				void							ExecuteAsyncTasks();
				bool							IsInMainThread()override;
				void							InvokeAsync(INativeAsyncService::AsyncTaskProc* proc, void* argument)override;
				void							InvokeInMainThread(INativeAsyncService::AsyncTaskProc* proc, void* argument)override;
				bool							InvokeInMainThreadAndWait(INativeAsyncService::AsyncTaskProc* proc, void* argument, vint milliseconds)override;
			};
		}
	}
}

#endif