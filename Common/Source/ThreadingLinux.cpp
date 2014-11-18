#include "Threading.h"
#include "Collections/List.h"
#ifdef VCZH_GCC
#include <pthread.h>
#include <unistd.h>

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
		thread->Run();
		thread->threadState=Thread::Stopped;
	}

	void* InternalThreadProcWrapper(void* lpParameter)
	{
		InternalThreadProc((Thread*)lpParameter);
		return 0;
	}

	Thread::Thread()
	{
		internalData=nullptr;
		threadState=Thread::NotStarted;
	}

	Thread::~Thread()
	{
		if (internalData)
		{
			Stop();
			pthread_detach(internalData->id);
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
		return false;
	}


	bool Thread::Stop()
	{
		if (internalData)
		{
			if(pthread_cancel(internalData->id)==0)
			{
				threadState=Thread::Stopped;
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
		while(__sync_val_compare_and_swap(&token, 1, 0)!=0)
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