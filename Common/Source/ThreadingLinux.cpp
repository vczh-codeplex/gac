#include "Threading.h"
#include "Collections/List.h"
#ifdef VCZH_GCC
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

namespace vl
{
	using namespace threading_internal;
	using namespace collections;


/***********************************************************************
Thread
***********************************************************************/

	namespace threading_internal
	{
		struct ThreadData
		{
			pthread_t					id;
			pthread_mutex_t				mutex;
			pthread_cond_t				cond;
		};

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
		pthread_mutex_lock(&thread->internalData->mutex);
		thread->Run();
		thread->threadState=Thread::Stopped;
		pthread_cond_broadcast(&thread->internalData->cond);
		pthread_mutex_unlock(&thread->internalData->mutex);
	}

	void* InternalThreadProcWrapper(void* lpParameter)
	{
		InternalThreadProc((Thread*)lpParameter);
		return 0;
	}

	Thread::Thread()
	{
		internalData=new ThreadData;
		pthread_mutex_init(&internalData->mutex, nullptr);
		pthread_cond_init(&internalData->cond, nullptr);
		threadState=Thread::NotStarted;
	}

	Thread::~Thread()
	{
		if (internalData)
		{
			Stop();
			pthread_mutex_destroy(&internalData->mutex);
			pthread_cond_destroy(&internalData->cond);

			if (threadState!=Thread::NotStarted)
			{
				pthread_detach(internalData->id);
			}
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

	void Thread::Sleep(vint ms)
	{
		sleep((ms+999)/1000);
	}
	
	vint Thread::GetCPUCount()
	{
		return (vint)sysconf(_SC_NPROCESSORS_ONLN);
	}

	vint Thread::GetCurrentThreadId()
	{
		return (vint)::pthread_self();
	}

	bool Thread::Start()
	{
		if(threadState==Thread::NotStarted)
		{
			if(pthread_create(&internalData->id, nullptr, &InternalThreadProcWrapper, this)==0)
			{
				threadState=Thread::Running;
				return true;
			}
		}
		return false;
	}

	bool Thread::Wait()
	{
		pthread_mutex_lock(&internalData->mutex);
		switch(threadState)
		{
			case Thread::NotStarted:
			case Thread::Stopped:
				break;
			default:
				pthread_cond_wait(&internalData->cond, &internalData->mutex);
		}
		pthread_mutex_unlock(&internalData->mutex);
		return true;
	}

	bool Thread::Stop()
	{
		if (threadState==Thread::Running)
		{
			if(pthread_cancel(internalData->id)==0)
			{
				threadState=Thread::Stopped;
				pthread_cond_broadcast(&internalData->cond);
				return true;
			}
		}
		return false;
	}

	Thread::ThreadState Thread::GetState()
	{
		return threadState;
	}

/***********************************************************************
Mutex
***********************************************************************/

	namespace threading_internal
	{
		struct MutexData
		{
			Semaphore			sem;
		};
	};

	Mutex::Mutex()
	{
		internalData = new MutexData;
	}

	Mutex::~Mutex()
	{
		delete internalData;
	}

	bool Mutex::Create(bool owned, const WString& name)
	{
		return internalData->sem.Create(owned ? 0 : 1, 1, name);
	}

	bool Mutex::Open(bool inheritable, const WString& name)
	{
		return internalData->sem.Open(inheritable, name);
	}

	bool Mutex::Release()
	{
		return internalData->sem.Release();
	}

	bool Mutex::Wait()
	{
		return internalData->sem.Wait();
	}

/***********************************************************************
Semaphore
***********************************************************************/

	namespace threading_internal
	{
		struct SemaphoreData
		{
			sem_t			semUnnamed;
			sem_t*			semNamed = nullptr;
		};
	}

	Semaphore::Semaphore()
		:internalData(0)
	{
	}

	Semaphore::~Semaphore()
	{
		if (internalData)
		{
			if (internalData->semNamed)
			{
				sem_close(internalData->semNamed);
			}
			else
			{
				sem_destroy(&internalData->semUnnamed);
			}
			delete internalData;
		}
	}

	bool Semaphore::Create(vint initialCount, vint maxCount, const WString& name)
	{
		if (internalData) return false;
		if (initialCount > maxCount) return false;

		internalData = new SemaphoreData;
		if (name == L"")
		{
			if(sem_init(&internalData->semUnnamed, 0, (int)initialCount) != 0)
			{
				delete internalData;
				return false;
			}
		}
		else
		{
			if (!(internalData->semNamed = sem_open(wtoa(name).Buffer(), O_CREAT, 0777, maxCount)))
			{
				delete internalData;
				return false;
			}
		}

		Release(initialCount);
		return true;
	}

	bool Semaphore::Open(bool inheritable, const WString& name)
	{
		if (internalData) return false;
		if (inheritable) return false;

		internalData = new SemaphoreData;
		if (!(internalData->semNamed = sem_open(wtoa(name).Buffer(), 0)))
		{
			delete internalData;
			return false;
		}

		return true;
	}

	bool Semaphore::Release()
	{
		return Release(1);
	}

	vint Semaphore::Release(vint count)
	{
		for (vint i = 0; i < count; i++)
		{
			if (internalData->semNamed)
			{
				sem_post(internalData->semNamed);
			}
			else
			{
				sem_post(&internalData->semUnnamed);
			}
		}
		return true;
	}

	bool Semaphore::Wait()
	{
		if (internalData->semNamed)
		{
			return sem_wait(internalData->semNamed) == 0;
		}
		else
		{
			return sem_wait(&internalData->semUnnamed) == 0;
		}
	}

/***********************************************************************
EventObject
***********************************************************************/

	namespace threading_internal
	{
		struct EventData
		{

		};
	}

	EventObject::EventObject()
	{
		internalData = nullptr;
		throw 0;
	}

	EventObject::~EventObject()
	{
		throw 0;
	}

	bool EventObject::CreateAutoUnsignal(bool signaled, const WString& name)
	{
		if (name!=L"") return false;
		throw 0;
	}

	bool EventObject::CreateManualUnsignal(bool signaled, const WString& name)
	{
		if (name!=L"") return false;
		throw 0;
	}

	bool EventObject::Signal()
	{
		throw 0;
	}

	bool EventObject::Unsignal()
	{
		throw 0;
	}

	bool EventObject::Wait()
	{
		throw 0;
	}

/***********************************************************************
ThreadPoolLite
***********************************************************************/

/***********************************************************************
CriticalSection
***********************************************************************/

	namespace threading_internal
	{
		struct CriticalSectionData
		{
			pthread_mutex_t		mutex;
		};
	}

	CriticalSection::CriticalSection()
	{
		internalData = new CriticalSectionData;
		pthread_mutex_init(&internalData->mutex, nullptr);
	}

	CriticalSection::~CriticalSection()
	{
		pthread_mutex_destroy(&internalData->mutex);
		delete internalData;
	}

	bool CriticalSection::TryEnter()
	{
		pthread_mutex_trylock(&internalData->mutex) == 0;
	}

	void CriticalSection::Enter()
	{
		pthread_mutex_lock(&internalData->mutex);
	}

	void CriticalSection::Leave()
	{
		pthread_mutex_unlock(&internalData->mutex);
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

/***********************************************************************
ReaderWriterLock
***********************************************************************/

	namespace threading_internal
	{
		struct ReaderWriterLockData
		{
			pthread_rwlock_t			rwlock;
		};
	}

	ReaderWriterLock::ReaderWriterLock()
	{
		internalData = new ReaderWriterLockData;
		pthread_rwlock_init(&internalData->rwlock, nullptr);
	}

	ReaderWriterLock::~ReaderWriterLock()
	{
		pthread_rwlock_destroy(&internalData->rwlock);
		delete internalData;
	}

	bool ReaderWriterLock::TryEnterReader()
	{
		return pthread_rwlock_tryrdlock(&internalData->rwlock) == 0;
	}

	void ReaderWriterLock::EnterReader()
	{
		pthread_rwlock_rdlock(&internalData->rwlock);
	}

	void ReaderWriterLock::LeaveReader()
	{
		pthread_rwlock_unlock(&internalData->rwlock);
	}

	bool ReaderWriterLock::TryEnterWriter()
	{
		return pthread_rwlock_trywrlock(&internalData->rwlock) == 0;
	}

	void ReaderWriterLock::EnterWriter()
	{
		pthread_rwlock_wrlock(&internalData->rwlock);
	}

	void ReaderWriterLock::LeaveWriter()
	{
		pthread_rwlock_unlock(&internalData->rwlock);
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
		lock->LeaveReader();
	}

/***********************************************************************
ConditionVariable
***********************************************************************/

	namespace threading_internal
	{
		struct ConditionVariableData
		{
			pthread_cond_t			cond;
		};
	}

	ConditionVariable::ConditionVariable()
	{
		internalData = new ConditionVariableData;
		pthread_cond_init(&internalData->cond, nullptr);
	}

	ConditionVariable::~ConditionVariable()
	{
		pthread_cond_destroy(&internalData->cond);
		delete internalData;
	}

	bool ConditionVariable::SleepWith(CriticalSection& cs)
	{
		return pthread_cond_wait(&internalData->cond, &cs.internalData->mutex) == 0;
	}

	void ConditionVariable::WakeOnePending()
	{
		pthread_cond_signal(&internalData->cond);
	}

	void ConditionVariable::WakeAllPendings()
	{
		pthread_cond_broadcast(&internalData->cond);
	}

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
		return __sync_lock_test_and_set(&token, 1)==0;
	}

	void SpinLock::Enter()
	{
		while(__sync_val_compare_and_swap(&token, 0, 1)!=0)
		{
			while(token!=0) _mm_pause();
		}
	}

	void SpinLock::Leave()
	{
		__sync_lock_test_and_set(&token, 0);
	}
}
#endif
