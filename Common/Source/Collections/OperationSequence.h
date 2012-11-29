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
		class TakeEnumerable : public EnumerableStore<T>, public virtual IEnumerable<T>
		{
		private:
			class Enumerator : public virtual IEnumerator<T>
			{
			protected:
				IEnumerator<T>*		enumerator;
				vint					count;
			public:
				Enumerator(IEnumerator<T>* _enumerator, vint _count)
					:enumerator(_enumerator)
					,count(_count)
				{
				}

				~Enumerator()
				{
					delete enumerator;
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(enumerator->Clone(), count);
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
					if(enumerator->Index()==count-1)
					{
						enumerator->Next();
						return false;
					}
					else if(enumerator->Index()>=count)
					{
						return false;
					}
					else
					{
						return enumerator->Next();
					}
				}

				bool Available()const
				{
					return enumerator->Available() && enumerator->Index()<count;
				}

				void Reset()
				{
					enumerator->Reset();
				}
			};
		protected:
			vint					count;
		public:
			TakeEnumerable(const IEnumerable<T>& enumerable, vint _count)
				:EnumerableStore<T>(enumerable)
				,count(_count)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(CopyEnumerator(), count);
			}
		};

		class TakeProcessor : public SequenceEnumerableProcessor<TakeEnumerable>
		{
		protected:
			vint					count;
		public:
			TakeProcessor(vint _count)
				:count(_count)
			{
			}

			template<typename T>
			TakeEnumerable<T> operator()(const IEnumerable<T>& enumerable)const
			{
				return TakeEnumerable<T>(enumerable, count);
			}
		};

		extern TakeProcessor Take(vint count);

/***********************************************************************
Skip
***********************************************************************/

		template<typename T>
		class SkipEnumerable : public EnumerableStore<T>, public virtual IEnumerable<T>
		{
		private:
			class Enumerator : public virtual IEnumerator<T>
			{
			protected:
				IEnumerator<T>*		enumerator;
				vint					count;
			public:
				Enumerator(IEnumerator<T>* _enumerator, vint _count)
					:enumerator(_enumerator)
					,count(_count)
				{
					for(vint i=0;i<count;i++)
					{
						enumerator->Next();
					}
				}

				~Enumerator()
				{
					delete enumerator;
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(enumerator->Clone(), count);
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

				bool Available()const
				{
					return enumerator->Available();
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
		protected:
			vint					count;
		public:
			SkipEnumerable(const IEnumerable<T>& enumerable, vint _count)
				:EnumerableStore<T>(enumerable)
				,count(_count)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(CopyEnumerator(), count);
			}
		};

		class SkipProcessor : public SequenceEnumerableProcessor<SkipEnumerable>
		{
		protected:
			vint					count;
		public:
			SkipProcessor(vint _count)
				:count(_count)
			{
			}

			template<typename T>
			SkipEnumerable<T> operator()(const IEnumerable<T>& enumerable)const
			{
				return SkipEnumerable<T>(enumerable, count);
			}
		};

		extern SkipProcessor Skip(vint count);

/***********************************************************************
Repeat
***********************************************************************/

		template<typename T>
		class RepeatEnumerable : public EnumerableStore<T>, public virtual IEnumerable<T>
		{
		private:
			class Enumerator : public virtual IEnumerator<T>
			{
			protected:
				IEnumerator<T>*		enumerator;
				vint					count;
				vint					index;
				vint					repeatedCount;
			public:
				Enumerator(IEnumerator<T>* _enumerator, vint _count, vint _index=0, vint _repeatedCount=0)
					:enumerator(_enumerator)
					,count(_count)
					,index(_index)
					,repeatedCount(_repeatedCount)
				{
				}

				~Enumerator()
				{
					delete enumerator;
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(enumerator->Clone(), count, index, repeatedCount);
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
					if(repeatedCount>=count)
					{
						return false;
					}
					else if(enumerator->Next())
					{
						index++;
						return true;
					}
					else
					{
						repeatedCount++;
						if(repeatedCount<count)
						{
							index++;
							enumerator->Reset();
							return enumerator->Available();
						}
						else
						{
							return false;
						}
					}
				}

				bool Available()const
				{
					return enumerator->Available() && repeatedCount<count;
				}

				void Reset()
				{
					enumerator->Reset();
					index=0;
					repeatedCount=0;
				}
			};
		protected:
			vint					count;
		public:
			RepeatEnumerable(const IEnumerable<T>& enumerable, vint _count)
				:EnumerableStore<T>(enumerable)
				,count(_count)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(CopyEnumerator(), count);
			}
		};

		class RepeatProcessor : public SequenceEnumerableProcessor<RepeatEnumerable>
		{
		protected:
			vint					count;
		public:
			RepeatProcessor(vint _count)
				:count(_count)
			{
			}

			template<typename T>
			RepeatEnumerable<T> operator()(const IEnumerable<T>& enumerable)const
			{
				return RepeatEnumerable<T>(enumerable, count);
			}
		};

		extern RepeatProcessor Repeat(vint count);

/***********************************************************************
Distinct
***********************************************************************/

		template<typename T>
		class DistinctEnumerable : public EnumerableStore<T>, public virtual IEnumerable<T>
		{
		private:
			class Enumerator : public virtual IEnumerator<T>
			{
			protected:
				IEnumerator<T>*		enumerator;
				SortedList<T>		distinct;
				T					lastValue;

				void GoNearest()
				{
					while(enumerator->Available())
					{
						const T& current=enumerator->Current();
						if(!distinct.Contains(current))
						{
							lastValue=current;
							distinct.Add(current);
							break;
						}
						enumerator->Next();
					}
				}
			public:
				Enumerator(IEnumerator<T>* _enumerator)
					:enumerator(_enumerator)
				{
					GoNearest();
				}

				Enumerator(const Enumerator& _enumerator)
				{
					enumerator=_enumerator.enumerator->Clone();
					CopyFrom(distinct, _enumerator.distinct);
				}

				~Enumerator()
				{
					delete enumerator;
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(*this);
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
					enumerator->Next();
					GoNearest();
					return Available();
				}

				bool Available()const
				{
					return enumerator->Available();
				}

				void Reset()
				{
					enumerator->Reset();
					distinct.Clear();
					GoNearest();
				}
			};
		public:
			DistinctEnumerable(const IEnumerable<T>& enumerable)
				:EnumerableStore<T>(enumerable)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(CopyEnumerator());
			}
		};

		class DistinctProcessor : public SequenceEnumerableProcessor<DistinctEnumerable>
		{
		public:
			DistinctProcessor()
			{
			}

			template<typename T>
			DistinctEnumerable<T> operator()(const IEnumerable<T>& enumerable)const
			{
				return DistinctEnumerable<T>(enumerable);
			}
		};

		extern DistinctProcessor Distinct();

/***********************************************************************
Reverse
***********************************************************************/

		template<typename T>
		class ReverseEnumerable : public Object, public virtual IEnumerable<T>
		{
		private:
			class Enumerator : public virtual IEnumerator<T>
			{
			protected:
				List<T>					cache;
				vint						index;
			public:
				Enumerator(const IEnumerable<T>& enumerable)
					:index(0)
				{
					CopyFrom(cache, enumerable);
				}

				Enumerator(const Enumerator& _enumerator)
				{
					CopyFrom(cache, _enumerator.cache);
					index=_enumerator.index;
				}

				~Enumerator()
				{
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(*this);
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
					return Available();
				}

				bool Available()const
				{
					return index<cache.Count();
				}

				void Reset()
				{
					index=0;
				}
			};
		protected:
			const IEnumerable<T>&		enumerable;
		public:
			ReverseEnumerable(const IEnumerable<T>& _enumerable)
				:enumerable(_enumerable)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(enumerable);
			}
		};

		class ReverseProcessor : public SequenceEnumerableProcessor<ReverseEnumerable>
		{
		public:
			ReverseProcessor()
			{
			}

			template<typename T>
			ReverseEnumerable<T> operator()(const IEnumerable<T>& enumerable)const
			{
				return ReverseEnumerable<T>(enumerable);
			}
		};

		extern ReverseProcessor Reverse();

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
				vint			index;

			public:
				Enumerator(I _begin, I _end, I _current, vint _index)
					:begin(_begin)
					,end(_end)
					,current(_current)
					,index(_index)
				{
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(begin, end, current, index);
				}

				const T& Current()const
				{
					return *current;
				}

				vint Index()const
				{
					return index;
				}

				bool Next()
				{
					if(current==end)
					{
						return false;
					}
					current++;
					if(current==end)
					{
						return false;
					}
					else
					{
						index++;
						return true;
					}
				}

				bool Available()const
				{
					return current!=end;
				}

				void Reset()
				{
					current=begin;
					index=0;
				}
			};
		private:
			I					begin;
			I					end;
		public:
			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(begin, end, begin, 0);
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