#include "Threading.h"
#include "Collections/List.h"

#if defined VCZH_MSVC
#include <Windows.h>
#elif defined VCZH_GCC
#include <pthread.h>
#include <unistd.h>
#endif

namespace vl
{
	using namespace threading_internal;
	using namespace collections;

#ifdef VCZH_MSVC
/***********************************************************************
WaitableObject
***********************************************************************/

	namespace threading_internal
	{
		struct WaitableData
		{
			HANDLE			handle;

			WaitableData(HANDLE _handle)
				:handle(_handle)
			{
			}
		};
	}

	WaitableObject::WaitableObject()
		:waitableData(0)
	{
	}

	void WaitableObject::SetData(threading_internal::WaitableData* data)
	{
		waitableData=data;
	}

	bool WaitableObject::IsCreated()
	{
		return waitableData!=0;
	}

	bool WaitableObject::Wait()
	{
		return WaitForTime(INFINITE);
	}

	bool WaitableObject::WaitForTime(vint ms)
	{
		if(IsCreated())
		{
			if(WaitForSingleObject(waitableData->handle, (DWORD)ms)==WAIT_OBJECT_0)
			{
				return true;
			}
		}
		return false;
	}

	bool WaitableObject::WaitAll(WaitableObject** objects, vint count)
	{
		Array<HANDLE> handles(count);
		for(vint i=0;i<count;i++)
		{
			handles[i]=objects[i]->waitableData->handle;
		}
		DWORD result=WaitForMultipleObjects((DWORD)count, &handles[0], TRUE, INFINITE);
		return result==WAIT_OBJECT_0 || result==WAIT_ABANDONED_0;

	}

	bool WaitableObject::WaitAllForTime(WaitableObject** objects, vint count, vint ms)
	{
		Array<HANDLE> handles(count);
		for(vint i=0;i<count;i++)
		{
			handles[i]=objects[i]->waitableData->handle;
		}
		DWORD result=WaitForMultipleObjects((DWORD)count, &handles[0], TRUE, (DWORD)ms);
		return result==WAIT_OBJECT_0 || result==WAIT_ABANDONED_0;
	}

	vint WaitableObject::WaitAny(WaitableObject** objects, vint count, bool* abandoned)
	{
		Array<HANDLE> handles(count);
		for(vint i=0;i<count;i++)
		{
			handles[i]=objects[i]->waitableData->handle;
		}
		DWORD result=WaitForMultipleObjects((DWORD)count, &handles[0], FALSE, INFINITE);
		if(WAIT_OBJECT_0 <= result && result<WAIT_OBJECT_0+count)
		{
			*abandoned=false;
			return result-WAIT_OBJECT_0;
		}
		else if(WAIT_ABANDONED_0 <= result && result<WAIT_ABANDONED_0+count)
		{
			*abandoned=true;
			return result-WAIT_ABANDONED_0;
		}
		else
		{
			return -1;
		}
	}

	vint WaitableObject::WaitAnyForTime(WaitableObject** objects, vint count, vint ms, bool* abandoned)
	{
		Array<HANDLE> handles(count);
		for(vint i=0;i<count;i++)
		{
			handles[i]=objects[i]->waitableData->handle;
		}
		DWORD result=WaitForMultipleObjects((DWORD)count, &handles[0], FALSE, (DWORD)ms);
		if(WAIT_OBJECT_0 <= result && result<WAIT_OBJECT_0+count)
		{
			*abandoned=false;
			return result-WAIT_OBJECT_0;
		}
		else if(WAIT_ABANDONED_0 <= result && result<WAIT_ABANDONED_0+count)
		{
			*abandoned=true;
			return result-WAIT_ABANDONED_0;
		}
		else
		{
			return -1;
		}
	}
#endif

/***********************************************************************
Thread
***********************************************************************/

	namespace threading_internal
	{
#if defined VCZH_MSVC
		struct ThreadData : public WaitableData
		{
			DWORD						id;

			ThreadData()
				:WaitableData(NULL)
			{
				id=-1;
			}
		};
#elif defined VCZH_GCC
		struct ThreadData
		{
			pthread_t					id;
		};
#endif

		class ProceduredThread : public Thread
		{
		private:
			Thread::ThreadProcedure		procedure;
			void*						argument;
			bool						deleteAfterStopped;

		protected:
			void Run()
			{
				procedure(this, argument);
				if(deleteAfterStopped)
				{
					delete this;
				}
			}
		public:
			ProceduredThread(Thread::ThreadProcedure _procedure, void* _argument, bool _deleteAfterStopped)
				:procedure(_procedure)
				,argument(_argument)
				,deleteAfterStopped(_deleteAfterStopped)
			{
			}
		};

		class LambdaThread : public Thread
		{
		private:
			Func<void()>				procedure;
			bool						deleteAfterStopped;

		protected:
			void Run()
			{
				procedure();
				if(deleteAfterStopped)
				{
					delete this;
				}
			}
		public:
			LambdaThread(const Func<void()>& _procedure, bool _deleteAfterStopped)
				:procedure(_procedure)
				,deleteAfterStopped(_deleteAfterStopped)
			{
			}
		};
	}

	void InternalThreadProc(Thread* thread)
	{
		thread->Run();
		thread->threadState=Thread::Stopped;
	}

#if defined VCZH_MSVC
	DWORD WINAPI InternalThreadProcWrapper(LPVOID lpParameter)
#elif defined VCZH_GCC
	void* InternalThreadProcWrapper(void* lpParameter)
#endif
	{
		InternalThreadProc((Thread*)lpParameter);
		return 0;
	}

	Thread::Thread()
	{
#if defined VCZH_MSVC
		internalData=new ThreadData;
		internalData->handle=CreateThread(NULL, 0, InternalThreadProcWrapper, this, CREATE_SUSPENDED, &internalData->id);
		threadState=Thread::NotStarted;
		SetData(internalData);
#elif defined VCZH_GCC
		internalData=nullptr;
		threadState=Thread::NotStarted;
#endif
	}

	Thread::~Thread()
	{
		if (internalData)
		{
			Stop();
#if defined VCZH_MSVC
			CloseHandle(internalData->handle);
#elif defined VCZH_GCC
			pthread_detach(internalData->id);
#endif
			delete internalData;
		}
	}

	Thread* Thread::CreateAndStart(ThreadProcedure procedure, void* argument, bool deleteAfterStopped)
	{
		if(procedure)
		{
			Thread* thread=new ProceduredThread(procedure, argument, deleteAfterStopped);
			if(thread->Start())
			{
				return thread;
			}
			else
			{
				delete thread;
			}
		}
		return 0;
	}

	Thread* Thread::CreateAndStart(const Func<void()>& procedure, bool deleteAfterStopped)
	{
		Thread* thread=new LambdaThread(procedure, deleteAfterStopped);
		if(thread->Start())
		{
			return thread;
		}
		else
		{
			delete thread;
		}
		return 0;
	}
#ifdef VCZH_MSVC	
	void Thread::Sleep(vint ms)
	{
		::Sleep((DWORD)ms);
	}
#endif
	
	vint Thread::GetCPUCount()
	{
#if defined VCZH_MSVC
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return info.dwNumberOfProcessors;
#elif defined VCZH_GCC
		return (vint)sysconf(_SC_NPROCESSORS_ONLN);
#endif
	}

	vint Thread::GetCurrentThreadId()
	{
#if defined VCZH_MSVC
		return (vint)::GetCurrentThreadId();
#elif defined VCZH_GCC
		return (vint)::pthread_self();
#endif
	}

	bool Thread::Start()
	{
#if defined VCZH_MSVC
		if(threadState==Thread::NotStarted && internalData->handle!=NULL)
		{
			if(ResumeThread(internalData->handle)!=-1)
			{
				threadState=Thread::Running;
				return true;
			}
		}
#elif defined VCZH_GCC
		if(threadState==Thread::NotStarted && internalData==nullptr)
		{
			internalData = new ThreadData;
			if(pthread_create(&internalData->id, nullptr, &InternalThreadProcWrapper, this)==0)
			{
				threadState=Thread::Running;
				return true;
			}
			else
			{
				delete internalData;
			}
		}
#endif
		return false;
	}

#ifdef VCZH_MSVC
	bool Thread::Pause()
	{
		if(threadState==Thread::Running)
		{
			if(SuspendThread(internalData->handle)!=-1)
			{
				threadState=Thread::Paused;
				return true;
			}
		}
		return false;
	}

	bool Thread::Resume()
	{
		if(threadState==Thread::Paused)
		{
			if(ResumeThread(internalData->handle)!=-1)
			{
				threadState=Thread::Running;
				return true;
			}
		}
		return false;
	}
#endif

	bool Thread::Stop()
	{
#if defined VCZH_MSVC
		if(internalData->handle!=NULL)
		{
			Pause();
			threadState=Thread::Stopped;
			return true;
		}
#elif defined VCZH_GCC
		if (internalData)
		{
			if(pthread_cancel(internalData->id)==0)
			{
				threadState=Thread::Stopped;
				return true;
			}
		}
#endif
		return false;
	}

	Thread::ThreadState Thread::GetState()
	{
		return threadState;
	}

#ifdef VCZH_MSVC
	void Thread::SetCPU(vint index)
	{
		SetThreadAffinityMask(internalData->handle, (1<<index));
	}
#endif

#ifdef VCZH_MSVC
/***********************************************************************
Mutex
***********************************************************************/

	namespace threading_internal
	{
		struct MutexData : public WaitableData
		{
			MutexData(HANDLE _handle)
				:WaitableData(_handle)
			{
			}
		};
	}

	Mutex::Mutex()
		:internalData(0)
	{
	}

	Mutex::~Mutex()
	{
		if(internalData)
		{
			CloseHandle(internalData->handle);
			delete internalData;
		}
	}

	bool Mutex::Create(bool owned, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aOwned=owned?TRUE:FALSE;
		LPCTSTR aName=name==L""?NULL:name.Buffer();
		HANDLE handle=CreateMutex(NULL, aOwned, aName);
		if(handle)
		{
			internalData=new MutexData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool Mutex::Open(bool inheritable, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aInteritable=inheritable?TRUE:FALSE;
		HANDLE handle=OpenMutex(SYNCHRONIZE, aInteritable, name.Buffer());
		if(handle)
		{
			internalData=new MutexData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool Mutex::Release()
	{
		if(IsCreated())
		{
			return ReleaseMutex(internalData->handle)!=0;
		}
		return false;
	}

/***********************************************************************
Semaphore
***********************************************************************/

	namespace threading_internal
	{
		struct SemaphoreData : public WaitableData
		{
			SemaphoreData(HANDLE _handle)
				:WaitableData(_handle)
			{
			}
		};
	}

	Semaphore::Semaphore()
		:internalData(0)
	{
	}

	Semaphore::~Semaphore()
	{
		if(internalData)
		{
			CloseHandle(internalData->handle);
			delete internalData;
		}
	}

	bool Semaphore::Create(vint initialCount, vint maxCount, const WString& name)
	{
		if(IsCreated())return false;
		LONG aInitial=(LONG)initialCount;
		LONG aMax=(LONG)maxCount;
		LPCTSTR aName=name==L""?NULL:name.Buffer();
		HANDLE handle=CreateSemaphore(NULL, aInitial, aMax, aName);
		if(handle)
		{
			internalData=new SemaphoreData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool Semaphore::Open(bool inheritable, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aInteritable=inheritable?TRUE:FALSE;
		HANDLE handle=OpenSemaphore(SYNCHRONIZE, aInteritable, name.Buffer());
		if(handle)
		{
			internalData=new SemaphoreData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool Semaphore::Release()
	{
		if(IsCreated())
		{
			return Release(1)!=-1;
		}
		return false;
	}

	vint Semaphore::Release(vint count)
	{
		if(IsCreated())
		{
			LONG previous=-1;
			if(ReleaseSemaphore(internalData->handle, (LONG)count, &previous)!=0)
			{
				return (vint)previous;
			}
		}
		return -1;
	}

/***********************************************************************
EventObject
***********************************************************************/

	namespace threading_internal
	{
		struct EventData : public WaitableData
		{
			EventData(HANDLE _handle)
				:WaitableData(_handle)
			{
			}
		};
	}

	EventObject::EventObject()
		:internalData(0)
	{
	}

	EventObject::~EventObject()
	{
		if(internalData)
		{
			CloseHandle(internalData->handle);
			delete internalData;
		}
	}

	bool EventObject::CreateAutoUnsignal(bool signaled, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aSignaled=signaled?TRUE:FALSE;
		LPCTSTR aName=name==L""?NULL:name.Buffer();
		HANDLE handle=CreateEvent(NULL, FALSE, aSignaled, aName);
		if(handle)
		{
			internalData=new EventData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool EventObject::CreateManualUnsignal(bool signaled, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aSignaled=signaled?TRUE:FALSE;
		LPCTSTR aName=name==L""?NULL:name.Buffer();
		HANDLE handle=CreateEvent(NULL, TRUE, aSignaled, aName);
		if(handle)
		{
			internalData=new EventData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool EventObject::Open(bool inheritable, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aInteritable=inheritable?TRUE:FALSE;
		HANDLE handle=OpenEvent(SYNCHRONIZE, aInteritable, name.Buffer());
		if(handle)
		{
			internalData=new EventData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool EventObject::Signal()
	{
		if(IsCreated())
		{
			return SetEvent(internalData->handle)!=0;
		}
		return false;
	}

	bool EventObject::Unsignal()
	{
		if(IsCreated())
		{
			return ResetEvent(internalData->handle)!=0;
		}
		return false;
	}

/***********************************************************************
ThreadPoolLite
***********************************************************************/

		struct ThreadPoolQueueProcArgument
		{
			void(*proc)(void*);
			void* argument;
		};

		DWORD WINAPI ThreadPoolQueueProc(void* argument)
		{
			Ptr<ThreadPoolQueueProcArgument> proc=(ThreadPoolQueueProcArgument*)argument;
			proc->proc(proc->argument);
			return 0;
		}

		DWORD WINAPI ThreadPoolQueueFunc(void* argument)
		{
			Ptr<Func<void()>> proc=(Func<void()>*)argument;
			(*proc.Obj())();
			return 0;
		}

		ThreadPoolLite::ThreadPoolLite()
		{
		}

		ThreadPoolLite::~ThreadPoolLite()
		{
		}

		bool ThreadPoolLite::Queue(void(*proc)(void*), void* argument)
		{
			ThreadPoolQueueProcArgument* p=new ThreadPoolQueueProcArgument;
			p->proc=proc;
			p->argument=argument;
			if(QueueUserWorkItem(&ThreadPoolQueueProc, p, WT_EXECUTEDEFAULT))
			{
				return true;
			}
			else
			{
				delete p;
				return false;
			}
		}

		bool ThreadPoolLite::Queue(const Func<void()>& proc)
		{
			Func<void()>* p=new Func<void()>(proc);
			if(QueueUserWorkItem(&ThreadPoolQueueFunc, p, WT_EXECUTEDEFAULT))
			{
				return true;
			}
			else
			{
				delete p;
				return false;
			}
		}

/***********************************************************************
CriticalSection
***********************************************************************/

	namespace threading_internal
	{
		struct CriticalSectionData
		{
			CRITICAL_SECTION		criticalSection;
		};
	}

	CriticalSection::Scope::Scope(CriticalSection& _criticalSection)
		:criticalSection(&_criticalSection)
	{
		criticalSection->Enter();
	}

	CriticalSection::Scope::~Scope()
	{
		criticalSection->Leave();
	}
			
	CriticalSection::CriticalSection()
	{
		internalData=new CriticalSectionData;
		InitializeCriticalSection(&internalData->criticalSection);
	}

	CriticalSection::~CriticalSection()
	{
		DeleteCriticalSection(&internalData->criticalSection);
		delete internalData;
	}

	bool CriticalSection::TryEnter()
	{
		return TryEnterCriticalSection(&internalData->criticalSection)!=0;
	}

	void CriticalSection::Enter()
	{
		EnterCriticalSection(&internalData->criticalSection);
	}

	void CriticalSection::Leave()
	{
		LeaveCriticalSection(&internalData->criticalSection);
	}

/***********************************************************************
ReaderWriterLock
***********************************************************************/

	namespace threading_internal
	{
		struct ReaderWriterLockData
		{
			SRWLOCK			lock;
		};
	}

	ReaderWriterLock::ReaderScope::ReaderScope(ReaderWriterLock& _lock)
		:lock(&_lock)
	{
		lock->EnterReader();
	}

	ReaderWriterLock::ReaderScope::~ReaderScope()
	{
		lock->LeaveReader();
	}

	ReaderWriterLock::WriterScope::WriterScope(ReaderWriterLock& _lock)
		:lock(&_lock)
	{
		lock->EnterWriter();
	}

	ReaderWriterLock::WriterScope::~WriterScope()
	{
		lock->LeaveWriter();
	}

	ReaderWriterLock::ReaderWriterLock()
		:internalData(new threading_internal::ReaderWriterLockData)
	{
		InitializeSRWLock(&internalData->lock);
	}

	ReaderWriterLock::~ReaderWriterLock()
	{
		delete internalData;
	}

	bool ReaderWriterLock::TryEnterReader()
	{
		return TryAcquireSRWLockShared(&internalData->lock)!=0;
	}

	void ReaderWriterLock::EnterReader()
	{
		AcquireSRWLockShared(&internalData->lock);
	}

	void ReaderWriterLock::LeaveReader()
	{
		ReleaseSRWLockShared(&internalData->lock);
	}

	bool ReaderWriterLock::TryEnterWriter()
	{
		return TryAcquireSRWLockExclusive(&internalData->lock)!=0;
	}

	void ReaderWriterLock::EnterWriter()
	{
		AcquireSRWLockExclusive(&internalData->lock);
	}

	void ReaderWriterLock::LeaveWriter()
	{
		ReleaseSRWLockExclusive(&internalData->lock);
	}

/***********************************************************************
ConditionVariable
***********************************************************************/

	namespace threading_internal
	{
		struct ConditionVariableData
		{
			CONDITION_VARIABLE			variable;
		};
	}

	ConditionVariable::ConditionVariable()
		:internalData(new threading_internal::ConditionVariableData)
	{
		InitializeConditionVariable(&internalData->variable);
	}

	ConditionVariable::~ConditionVariable()
	{
		delete internalData;
	}

	bool ConditionVariable::SleepWith(CriticalSection& cs)
	{
		return SleepConditionVariableCS(&internalData->variable, &cs.internalData->criticalSection, INFINITE)!=0;
	}

	bool ConditionVariable::SleepWithForTime(CriticalSection& cs, vint ms)
	{
		return SleepConditionVariableCS(&internalData->variable, &cs.internalData->criticalSection, (DWORD)ms)!=0;
	}

	bool ConditionVariable::SleepWithReader(ReaderWriterLock& lock)
	{
		return SleepConditionVariableSRW(&internalData->variable, &lock.internalData->lock, INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED)!=0;
	}

	bool ConditionVariable::SleepWithReaderForTime(ReaderWriterLock& lock, vint ms)
	{
		return SleepConditionVariableSRW(&internalData->variable, &lock.internalData->lock, (DWORD)ms, CONDITION_VARIABLE_LOCKMODE_SHARED)!=0;
	}

	bool ConditionVariable::SleepWithWriter(ReaderWriterLock& lock)
	{
		return SleepConditionVariableSRW(&internalData->variable, &lock.internalData->lock, INFINITE, 0)!=0;
	}

	bool ConditionVariable::SleepWithWriterForTime(ReaderWriterLock& lock, vint ms)
	{
		return SleepConditionVariableSRW(&internalData->variable, &lock.internalData->lock, (DWORD)ms, 0)!=0;
	}

	void ConditionVariable::WakeOnePending()
	{
		WakeConditionVariable(&internalData->variable);
	}

	void ConditionVariable::WakeAllPendings()
	{
		WakeAllConditionVariable(&internalData->variable);
	}
#endif
/***********************************************************************
SpinLock
***********************************************************************/

	SpinLock::Scope::Scope(SpinLock& _spinLock)
		:spinLock(&_spinLock)
	{
		spinLock->Enter();
	}

	SpinLock::Scope::~Scope()
	{
		spinLock->Leave();
	}
			
	SpinLock::SpinLock()
		:token(0)
	{
	}

	SpinLock::~SpinLock()
	{
	}

	bool SpinLock::TryEnter()
	{
#if defined VCZH_MSVC
		return _InterlockedExchange(&token, 1)==0;
#elif defined VCZH_GCC
		return __sync_lock_test_and_set(&token, 1)==0;
#endif
	}

	void SpinLock::Enter()
	{
#if defined VCZH_MSVC
		while(_InterlockedCompareExchange(&token, 1, 0)!=0)
#elif defined VCZH_GCC
		while(__sync_val_compare_and_swap(&token, 1, 0)!=0)
#endif
		{
			while(token!=0) _mm_pause();
		}
	}

	void SpinLock::Leave()
	{
#if defined VCZH_MSVC
		_InterlockedExchange(&token, 0);
#elif defined VCZH_GCC
		__sync_lock_test_and_set(&token, 0);
#endif
	}
}
