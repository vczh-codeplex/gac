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
					Func<void()>						proc;

					TaskItem();
					TaskItem(Semaphore* _semaphore, const Func<void()>& _proc);
					~TaskItem();

					bool operator==(const TaskItem& item)const{return false;}
					bool operator!=(const TaskItem& item)const{return true;}
				};
			protected:
				vint							mainThreadId;
				SpinLock						taskListLock;
				collections::List<TaskItem>		taskItems;
			public:
				WindowsAsyncService();
				~WindowsAsyncService();

				void							ExecuteAsyncTasks();
				bool							IsInMainThread()override;
				void							InvokeAsync(const Func<void()>& proc)override;
				void							InvokeInMainThread(const Func<void()>& proc)override;
				bool							InvokeInMainThreadAndWait(const Func<void()>& proc, vint milliseconds)override;
				Ptr<INativeDelay>				DelayExecute(const Func<void()>& proc)override;
				Ptr<INativeDelay>				DelayExecuteInMainThread(const Func<void()>& proc)override;
			};
		}
	}
}

#endif