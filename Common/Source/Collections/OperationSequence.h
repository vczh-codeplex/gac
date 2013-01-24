/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONSEQUENCE
#define VCZH_COLLECTIONS_OPERATIONSEQUENCE

#include "OperationEnumerable.h"
#include "OperationCopyFrom.h"
#include "List.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
Take
***********************************************************************/

		template<typename T>
		class TakeEnumerator : public virtual IEnumerator<T>
		{
		protected:
			IEnumerator<T>*			enumerator;
			vint					count;
		public:
			TakeEnumerator(IEnumerator<T>* _enumerator, vint _count)
				:enumerator(_enumerator)
				,count(_count)
			{
			}

			~TakeEnumerator()
			{
				delete enumerator;
			}

			IEnumerator<T>* Clone()const
			{
				return new TakeEnumerator(enumerator->Clone(), count);
			}

			const T& Current()const
			{
				return enumerator->Current();
			}

			vint Index()const
			{
				return enumerator->Index();
			}

			bool Next()
			{
				if(enumerator->Index()>=count-1) return false;
				return enumerator->Next();
			}

			void Reset()
			{
				enumerator->Reset();
			}
		};

/***********************************************************************
Skip
***********************************************************************/

		template<typename T>
		class SkipEnumerator : public virtual IEnumerator<T>
		{
		protected:
			IEnumerator<T>*			enumerator;
			vint					count;
		public:
			SkipEnumerator(IEnumerator<T>* _enumerator, vint _count, bool runSkip=true)
				:enumerator(_enumerator)
				,count(_count)
			{
				if(runSkip)
				{
					for(vint i=0;i<count;i++)
					{
						enumerator->Next();
					}
				}
			}

			~SkipEnumerator()
			{
				delete enumerator;
			}

			IEnumerator<T>* Clone()const
			{
				return new SkipEnumerator(enumerator->Clone(), count, false);
			}

			const T& Current()const
			{
				return enumerator->Current();
			}

			vint Index()const
			{
				return enumerator->Index()-count;
			}

			bool Next()
			{
				return enumerator->Next();
			}

			void Reset()
			{
				enumerator->Reset();
				for(vint i=0;i<count;i++)
				{
					enumerator->Next();
				}
			}
		};

/***********************************************************************
Repeat
***********************************************************************/

		template<typename T>
		class RepeatEnumerator : public virtual IEnumerator<T>
		{
		protected:
			IEnumerator<T>*			enumerator;
			vint					count;
			vint					index;
			vint					repeatedCount;
		public:
			RepeatEnumerator(IEnumerator<T>* _enumerator, vint _count, vint _index=-1, vint _repeatedCount=0)
				:enumerator(_enumerator)
				,count(_count)
				,index(_index)
				,repeatedCount(_repeatedCount)
			{
			}

			~RepeatEnumerator()
			{
				delete enumerator;
			}

			IEnumerator<T>* Clone()const
			{
				return new RepeatEnumerator(enumerator->Clone(), count, index, repeatedCount);
			}

			const T& Current()const
			{
				return enumerator->Current();
			}

			vint Index()const
			{
				return index;
			}

			bool Next()
			{
				while(repeatedCount<count)
				{
					if(enumerator->Next())
					{
						index++;
						return true;
					}
					repeatedCount++;
					enumerator->Reset();
				}
				return false;
			}

			void Reset()
			{
				enumerator->Reset();
				index=-1;
				repeatedCount=0;
			}
		};

/***********************************************************************
Distinct
***********************************************************************/

		template<typename T>
		class DistinctEnumerator : public virtual IEnumerator<T>
		{
		protected:
			IEnumerator<T>*		enumerator;
			SortedList<T>		distinct;
			T					lastValue;

		public:
			DistinctEnumerator(IEnumerator<T>* _enumerator)
				:enumerator(_enumerator)
			{
			}

			DistinctEnumerator(const DistinctEnumerator& _enumerator)
			{
				enumerator=_enumerator.enumerator->Clone();
				CopyFrom(distinct, _enumerator.distinct);
			}

			~DistinctEnumerator()
			{
				delete enumerator;
			}

			IEnumerator<T>* Clone()const
			{
				return new DistinctEnumerator(*this);
			}

			const T& Current()const
			{
				return lastValue;
			}

			vint Index()const
			{
				return distinct.Count()-1;
			}

			bool Next()
			{
				while(enumerator->Next())
				{
					const T& current=enumerator->Current();
					if(!distinct.Contains(current))
					{
						lastValue=current;
						distinct.Add(current);
						return true;
					}
				}
				return false;
			}

			void Reset()
			{
				enumerator->Reset();
				distinct.Clear();
			}
		};

/***********************************************************************
Reverse
***********************************************************************/

		template<typename T>
		class ReverseEnumerator : public virtual IEnumerator<T>
		{
		protected:
			List<T>						cache;
			vint						index;
		public:
			ReverseEnumerator(const IEnumerable<T>& enumerable)
				:index(-1)
			{
				CopyFrom(cache, enumerable);
			}

			ReverseEnumerator(const ReverseEnumerator& _enumerator)
			{
				CopyFrom(cache, _enumerator.cache);
				index=_enumerator.index;
			}

			~ReverseEnumerator()
			{
			}

			IEnumerator<T>* Clone()const
			{
				return new ReverseEnumerator(*this);
			}

			const T& Current()const
			{
				return cache.Get(cache.Count()-1-index);
			}

			vint Index()const
			{
				return index;
			}

			bool Next()
			{
				index++;
				return index<cache.Count();
			}

			void Reset()
			{
				index=0;
			}
		};

/***********************************************************************
FromIterator
***********************************************************************/

		template<typename T, typename I>
		class FromIteratorEnumerable : public Object, public IEnumerable<T>
		{
			friend class Enumerable<T>;
		private:
			class Enumerator : public Object, public IEnumerator<T>
			{
			private:
				I				begin;
				I				end;
				I				current;

			public:
				Enumerator(I _begin, I _end, I _current)
					:begin(_begin)
					,end(_end)
					,current(_current)
				{
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(begin, end, current);
				}

				const T& Current()const
				{
					return *current;
				}

				vint Index()const
				{
					return current-begin;
				}

				bool Next()
				{
					current++;
					return begin<=current && current<end;
				}

				void Reset()
				{
					current=begin-1;
				}
			};
		private:
			I					begin;
			I					end;
		public:
			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(begin, end, begin-1);
			}

			FromIteratorEnumerable(I _begin, I _end)
				:begin(_begin)
				,end(_end)
			{
			}
		};

		template<typename T>
		class FromIterator
		{
		public:
			template<typename I>
			static FromIteratorEnumerable<T, I> Wrap(I begin, I end)
			{
				return FromIteratorEnumerable<T, I>(begin, end);
			}
		};

		template<typename T>
		FromIteratorEnumerable<T, T*> FromPointer(T* begin, T* end)
		{
			return FromIteratorEnumerable<T, T*>(begin, end);
		}

		template<typename T, int size>
		FromIteratorEnumerable<T, T*> FromArray(T (&items)[size])
		{
			return FromIteratorEnumerable<T, T*>(&items[0], &items[size]);
		}
	}
}

#endif