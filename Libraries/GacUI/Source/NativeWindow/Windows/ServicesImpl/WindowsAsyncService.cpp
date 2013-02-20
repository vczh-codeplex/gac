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
WindowsAsyncService::DelayItem
***********************************************************************/

			WindowsAsyncService::DelayItem::DelayItem(WindowsAsyncService* _service, const Func<void()>& _proc, bool _executeInMainThread, vint milliseconds)
				:service(_service)
				,proc(_proc)
				,status(INativeDelay::Pending)
				,executeTime(DateTime::LocalTime().Forward(milliseconds))
				,executeInMainThread(_executeInMainThread)
			{
			}

			WindowsAsyncService::DelayItem::~DelayItem()
			{
			}

			INativeDelay::ExecuteStatus WindowsAsyncService::DelayItem::GetStatus()
			{
				return status;
			}

			bool WindowsAsyncService::DelayItem::Delay(vint milliseconds)
			{
				SpinLock::Scope scope(service->taskListLock);
				if(status==INativeDelay::Pending)
				{
					executeTime=DateTime::LocalTime().Forward(milliseconds);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool WindowsAsyncService::DelayItem::Cancel()
			{
				SpinLock::Scope scope(service->taskListLock);
				if(status==INativeDelay::Pending)
				{
					service->delayItems.Remove(this);
					return true;
				}
				else
				{
					return false;
				}
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
				DateTime now=DateTime::LocalTime();
				Array<TaskItem> items;
				List<Ptr<DelayItem>> executableDelayItems;
				{
					SpinLock::Scope scope(taskListLock);
					CopyFrom(items, taskItems);
					taskItems.RemoveRange(0, items.Count());
					for(vint i=delayItems.Count()-1;i>=0;i--)
					{
						Ptr<DelayItem> item=delayItems[i];
						if(now.filetime<=item->executeTime.filetime)
						{
							item->status=INativeDelay::Executing;
							executableDelayItems.Add(item);
							delayItems.RemoveAt(i);
						}
					}
				}
				FOREACH(TaskItem, item, items)
				{
					item.proc();
					if(item.semaphore)
					{
						item.semaphore->Release();
					}
				}
				FOREACH(Ptr<DelayItem>, item, executableDelayItems)
				{
					if(item->executeInMainThread)
					{
						item->proc();
						item->status=INativeDelay::Executed;
					}
					else
					{
						InvokeAsync([=]()
						{
							item->proc();
							item->status=INativeDelay::Executed;
						});
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

			Ptr<INativeDelay> WindowsAsyncService::DelayExecute(const Func<void()>& proc, vint milliseconds)
			{
				SpinLock::Scope scope(taskListLock);
				Ptr<DelayItem> delay=new DelayItem(this, proc, false, milliseconds);
				delayItems.Add(delay);
				return delay;
			}

			Ptr<INativeDelay> WindowsAsyncService::DelayExecuteInMainThread(const Func<void()>& proc, vint milliseconds)
			{
				SpinLock::Scope scope(taskListLock);
				Ptr<DelayItem> delay=new DelayItem(this, proc, true, milliseconds);
				delayItems.Add(delay);
				return delay;
			}
		}
	}
}