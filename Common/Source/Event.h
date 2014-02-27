/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Event

Classes:
	Event<function-type>									：事件对象
	
本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/
#ifndef VCZH_EVENT
#define VCZH_EVENT
#include "Function.h"
#include "Collections\List.h"
namespace vl
{
	template<typename T>
	class Event
	{
	};
 
	class EventHandler : public Object
	{
	public:
		virtual bool							IsAttached() = 0;
	};
 
/***********************************************************************
vl::Event<void()>
***********************************************************************/
	template< >
	class Event<void()> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void()>		function;
			EventHandlerImpl(const Func<void()>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void()>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)())
		{
			return Add(Func<void()>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)())
		{
			return Add(Func<void()>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()()const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function();
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0)>
***********************************************************************/
	template< typename T0>
	class Event<void(T0)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0)>		function;
			EventHandlerImpl(const Func<void(T0)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0))
		{
			return Add(Func<void(T0)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0))
		{
			return Add(Func<void(T0)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1)>
***********************************************************************/
	template< typename T0,typename T1>
	class Event<void(T0,T1)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1)>		function;
			EventHandlerImpl(const Func<void(T0,T1)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1))
		{
			return Add(Func<void(T0,T1)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1))
		{
			return Add(Func<void(T0,T1)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2)>
***********************************************************************/
	template< typename T0,typename T1,typename T2>
	class Event<void(T0,T1,T2)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1,T2)>		function;
			EventHandlerImpl(const Func<void(T0,T1,T2)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1,T2)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1,T2))
		{
			return Add(Func<void(T0,T1,T2)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1,T2))
		{
			return Add(Func<void(T0,T1,T2)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1,T2 p2)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1,p2);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3>
	class Event<void(T0,T1,T2,T3)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1,T2,T3)>		function;
			EventHandlerImpl(const Func<void(T0,T1,T2,T3)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1,T2,T3)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1,T2,T3))
		{
			return Add(Func<void(T0,T1,T2,T3)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1,T2,T3))
		{
			return Add(Func<void(T0,T1,T2,T3)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1,p2,p3);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4>
	class Event<void(T0,T1,T2,T3,T4)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1,T2,T3,T4)>		function;
			EventHandlerImpl(const Func<void(T0,T1,T2,T3,T4)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1,T2,T3,T4)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1,T2,T3,T4))
		{
			return Add(Func<void(T0,T1,T2,T3,T4)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1,T2,T3,T4))
		{
			return Add(Func<void(T0,T1,T2,T3,T4)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1,p2,p3,p4);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
	class Event<void(T0,T1,T2,T3,T4,T5)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1,T2,T3,T4,T5)>		function;
			EventHandlerImpl(const Func<void(T0,T1,T2,T3,T4,T5)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1,T2,T3,T4,T5)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1,T2,T3,T4,T5))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1,p2,p3,p4,p5);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5,T6)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	class Event<void(T0,T1,T2,T3,T4,T5,T6)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1,T2,T3,T4,T5,T6)>		function;
			EventHandlerImpl(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1,T2,T3,T4,T5,T6))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5,T6)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5,T6))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5,T6)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1,p2,p3,p4,p5,p6);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5,T6,T7)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	class Event<void(T0,T1,T2,T3,T4,T5,T6,T7)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>		function;
			EventHandlerImpl(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1,T2,T3,T4,T5,T6,T7))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
	class Event<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>		function;
			EventHandlerImpl(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	class Event<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public Object, private NotCopyable
	{
	protected:
		class EventHandlerImpl : public EventHandler
		{
		public:
			bool								attached;
			Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>		function;
			EventHandlerImpl(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& _function)
				:attached(true)
				, function(_function)
			{
			}
 
			bool IsAttached()override
			{
				return attached;
			}
		};
 
		collections::SortedList<Ptr<EventHandlerImpl>>	handlers;
	public:
		Ptr<EventHandler> Add(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)
		{
			Ptr<EventHandlerImpl> handler = new EventHandlerImpl(function);
			handlers.Add(handler);
			return handler;
		}
 
		template<typename C>
		Ptr<EventHandler> Add(void(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>(function));
		}
 
		template<typename C>
		Ptr<EventHandler> Add(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
		{
			return Add(Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>(sender, function));
		}
 
		bool Remove(Ptr<EventHandler> handler)
		{
			Ptr<EventHandlerImpl> impl = handler.Cast<EventHandlerImpl>();
			if (!impl) return false;
			vint index = handlers.IndexOf(impl);
			if (index == -1) return false;
			impl->attached = false;
			handlers.RemoveAt(index);
			return true;
		}
 
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
		{
			for(vint i = 0; i < handlers.Count(); i++)
			{
				handlers[i]->function(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		}
	};
 
}
#endif