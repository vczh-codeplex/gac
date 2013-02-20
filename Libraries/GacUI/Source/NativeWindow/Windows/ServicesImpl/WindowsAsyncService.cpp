#include "WindowsAsyncService.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			using namespace collections;

/***********************************************************************
WindowsAsyncService::TaskItem
***********************************************************************/

			WindowsAsyncService::TaskItem::TaskItem()
				:semaphore(0)
			{
			}

			WindowsAsyncService::TaskItem::TaskItem(Semaphore* _semaphore, const Func<void()>& _proc)
				:semaphore(_semaphore)
				,proc(_proc)
			{
			}

			WindowsAsyncService::TaskItem::~TaskItem()
			{
			}

/***********************************************************************
WindowsAsyncService
***********************************************************************/

			WindowsAsyncService::WindowsAsyncService()
				:mainThreadId(Thread::GetCurrentThreadId())
			{
			}

			WindowsAsyncService::~WindowsAsyncService()
			{
			}

			void WindowsAsyncService::ExecuteAsyncTasks()
			{
				Array<TaskItem> items;
				{
					SpinLock::Scope scope(taskListLock);
					CopyFrom(items, taskItems);
					taskItems.RemoveRange(0, items.Count());
				}
				for(vint i=0;i<items.Count();i++)
				{
					TaskItem taskItem=items[i];
					taskItem.proc();
					if(taskItem.semaphore)
					{
						taskItem.semaphore->Release();
					}
				}
			}

			bool WindowsAsyncService::IsInMainThread()
			{
				return Thread::GetCurrentThreadId()==mainThreadId;
			}

			void WindowsAsyncService::InvokeAsync(const Func<void()>& proc)
			{
				ThreadPoolLite::Queue(proc);
			}

			void WindowsAsyncService::InvokeInMainThread(const Func<void()>& proc)
			{
				SpinLock::Scope scope(taskListLock);
				TaskItem item(0, proc);
				taskItems.Add(item);
			}

			bool WindowsAsyncService::InvokeInMainThreadAndWait(const Func<void()>& proc, vint milliseconds)
			{
				Semaphore semaphore;
				semaphore.Create(0, 1);
				{
					SpinLock::Scope scope(taskListLock);
					TaskItem item(&semaphore, proc);
					taskItems.Add(item);
				}
				if(milliseconds<0)
				{
					return semaphore.Wait();
				}
				else
				{
					return semaphore.WaitForTime(milliseconds);
				}
			}

			Ptr<INativeDelay> WindowsAsyncService::DelayExecute(const Func<void()>& proc)
			{
				return 0;
			}

			Ptr<INativeDelay> WindowsAsyncService::DelayExecuteInMainThread(const Func<void()>& proc)
			{
				return 0;
			}
		}
	}
}