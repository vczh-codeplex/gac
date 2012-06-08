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
				,proc(0)
				,argument(0)
			{
			}

			WindowsAsyncService::TaskItem::TaskItem(Semaphore* _semaphore, INativeAsyncService::AsyncTaskProc* _proc, void* _argument)
				:semaphore(_semaphore)
				,proc(_proc)
				,argument(_argument)
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
					CopyFrom(items.Wrap(), taskItems.Wrap());
					taskItems.RemoveRange(0, items.Count());
				}
				for(int i=0;i<items.Count();i++)
				{
					TaskItem taskItem=items[i];
					taskItem.proc(taskItem.argument);
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

			void WindowsAsyncService::InvokeAsync(INativeAsyncService::AsyncTaskProc* proc, void* argument)
			{
				ThreadPoolLite::Queue(proc, argument);
			}

			void WindowsAsyncService::InvokeInMainThread(INativeAsyncService::AsyncTaskProc* proc, void* argument)
			{
				SpinLock::Scope scope(taskListLock);
				TaskItem item(0, proc, argument);
				taskItems.Add(item);
			}

			bool WindowsAsyncService::InvokeInMainThreadAndWait(INativeAsyncService::AsyncTaskProc* proc, void* argument, int milliseconds)
			{
				Semaphore semaphore;
				semaphore.Create(0, 1);
				{
					SpinLock::Scope scope(taskListLock);
					TaskItem item(&semaphore, proc, argument);
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
		}
	}
}