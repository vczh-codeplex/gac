/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Data Structure::Operations

Functions:
	CopyFrom(TargetContainer, SourceContainer)
	[T]		.Select(T->K) => [K]
	[T]		.Where(T->bool) => [T]
	[Ptr<T>].Cast<K>() => [Ptr<K>]
	[Ptr<T>].FindType<K>() => [Ptr<K>]
	[T]		.OrderBy(T->T->int) => [T]

	[T]		.Aggregate(T->T->T) => T
	[T]		.Aggregate(T->T->T, T) => T
	[T]		.All(T->bool) => bool
	[T]		.Any(T->bool) => bool
	[T]		.Max() => T
	[T]		.Min() => T
	[T]		.First() => T
	[T]		.FirstOrDefault(T) => T
	[T]		.Last() => T
	[T]		.LastOrDefault(T) => T
	[T]		.Count() => vint
	[T]		.IsEmpty() => bool

	[T]		.Concat([T]) => [T]
	[T]		.Repeat(vint) => [T]
	[T]		.Take(vint) => [T]
	[T]		.Skip(vint) => [T]
	[T]		.Distinct() => [T]
	[T]		.Reverse() => [T]

	[T]		.Pairwise([K]) => [(T,K)]
	[T]		.Intersect([T]) => [T]
	[T]		.Union([T]) => [T]
	[T]		.Except([T]) => [T]

	From(begin, end) => [T]
	From(array) => [T]

	FOREACH(X, a, XList)
	FOREACH_INDEXER(X, a, index, XList)
***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATION
#define VCZH_COLLECTIONS_OPERATION

#include "OperationCopyFrom.h"
#include "OperationEnumerable.h"
#include "OperationSelect.h"
#include "OperationWhere.h"
#include "OperationConcat.h"
#include "OperationSequence.h"
#include "OperationSet.h"
#include "OperationPair.h"
#include "OperationString.h"
#include "OperationForEach.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
OrderBy Quick Sort
***********************************************************************/

		template<typename T>
		void Sort(T* items, vint length, const Func<vint(T, T)>& orderer)
		{
			if(length==0) return;
			vint pivot=0;
			vint left=0;
			vint right=0;
			bool flag=false;

			while(left+right+1!=length)
			{
				vint& mine=(flag?left:right);
				vint& theirs=(flag?right:left);
				vint candidate=(flag?left:length-right-1);
				vint factor=(flag?-1:1);

				if(orderer(items[pivot], items[candidate])*factor<=0)
				{
					mine++;
				}
				else
				{
					theirs++;
					T temp=items[pivot];
					items[pivot]=items[candidate];
					items[candidate]=temp;
					pivot=candidate;
					flag=!flag;
				}
			}

			Sort(items, left, orderer);
			Sort(items+left+1, right, orderer);
		}

/***********************************************************************
LazyList
***********************************************************************/

		template<typename T>
		class LazyList : public Object, public IEnumerable<T>
		{
		protected:
			Ptr<IEnumerator<T>>			enumeratorPrototype;

			IEnumerator<T>* xs()
			{
				return enumeratorPrototype->Clone();
			}
		public:
			LazyList(Ptr<IEnumerator<T>> enumerator)
				:enumeratorPrototype(enumerator)
			{
			}

			LazyList(const IEnumerable<T>& enumerable)
				:enumeratorPrototype(enumerable.CreateEnumerator())
			{
			}
			
			LazyList()
				:enumeratorPrototype(EmptyEnumerable<T>().CreateEnumerator())
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return enumeratorPrototype->Clone();
			}

			//-------------------------------------------------------

			template<typename F>
			LazyList<FUNCTION_RESULT_TYPE(F)> Select(F f)
			{
				return new SelectEnumerator<T, FUNCTION_RESULT_TYPE(F)>(xs(), f);
			}
			
			template<typename F>
			LazyList<T> Where(F f)
			{
				return new WhereEnumerator<T>(xs(), f);
			}

			template<typename U>
			LazyList<Ptr<U>> Cast()
			{
				Func<Ptr<U>(T)> f=[](T t)->Ptr<U>{return t.Cast<U>();};
				return new SelectEnumerator<T, Ptr<U>>(xs(), f);
			}

			template<typename U>
			LazyList<Ptr<U>> FindType()
			{
				return Cast<U>().Where([](T t){return t;});
			}

			template<typename F>
			LazyList<T> OrderBy(F f)
			{
				Ptr<List<T>> sorted=new List<T>;
				CopyFrom(*sorted.Obj(), *this);
				if(sorted->Count()>0)
				{
					Sort<T>(&sorted->operator[](0), sorted->Count(), f);
				}
				return new ContainerEnumerator<T, List<T>>(sorted);
			}

			//-------------------------------------------------------

			template<typename F>
			T Aggregate(F f)
			{
				Ptr<IEnumerator<T>> enumerator=CreateEnumerator();
				if(!enumerator->Next())
				{
					throw Error(L"LazyList<T>::Aggregate(F)#容器为空并且没有初始值，Aggregate操作失败。");
				}
				T result=enumerator->Current();
				while(enumerator->Next())
				{
					result=f(result, enumerator->Current());
				}
				return result;
			}

			template<typename I, typename F>
			I Aggregate(I init, F f)
			{
				FOREACH(T, t, *this)
				{
					init=f(init, t);
				}
				return init;
			}

			template<typename F>
			bool All(F f)
			{
				return Select(f).Aggregate(true, [](bool a, bool b){return a&&b;});
			}

			template<typename F>
			bool Any(F f)
			{
				return Select(f).Aggregate(false, [](bool a, bool b){return a||b;});
			}

			T Max()
			{
				return Aggregate([](T a, T b){return a>b?a:b;});
			}

			T Min()
			{
				return Aggregate([](T a, T b){return a<b?a:b;});
			}

			T First()
			{
				Ptr<IEnumerator<T>> enumerator=CreateEnumerator();
				if(!enumerator->Next())
				{
					throw Error(L"LazyList<T>::First(F)#容器为空并且没有初始值，Aggregate操作失败。");
				}
				return enumerator->Current();
			}

			T First(T defaultValue)
			{
				Ptr<IEnumerator<T>> enumerator=CreateEnumerator();
				if(!enumerator->Next())
				{
					return defaultValue;
				}
				return enumerator->Current();
			}

			T Last()
			{
				Ptr<IEnumerator<T>> enumerator=CreateEnumerator();
				if(!enumerator->Next())
				{
					throw Error(L"LazyList<T>::Last(F)#容器为空并且没有初始值，Aggregate操作失败。");
				}
				else
				{
					T value=enumerator->Current();
					while(enumerator->Next())
					{
						value=enumerator->Current();
					}
					return value;
				}
			}

			T Last(T defaultValue)
			{
				Ptr<IEnumerator<T>> enumerator=CreateEnumerator();
				while(enumerator->Next())
				{
					defaultValue=enumerator->Current();
				}
				return defaultValue;
			}

			vint Count()
			{
				vint result=0;
				Ptr<IEnumerator<T>> enumerator=CreateEnumerator();
				while(enumerator->Next())
				{
					result++;
				}
				return result;
			}

			bool IsEmpty()
			{
				Ptr<IEnumerator<T>> enumerator=CreateEnumerator();
				return enumerator->Next();
			}

			//-------------------------------------------------------

			LazyList<T> Concat(IEnumerable<T>& remains)
			{
				return new ConcatEnumerator<T>(xs(), remains.CreateEnumerator());
			}

			LazyList<T> Take(vint count)
			{
				return new TakeEnumerator<T>(xs(), count);
			}

			LazyList<T> Skip(vint count)
			{
				return new SkipEnumerator<T>(xs(), count);
			}

			LazyList<T> Repeat(vint count)
			{
				return new RepeatEnumerator<T>(xs(), count);
			}

			LazyList<T> Distinct()
			{
				return new DistinctEnumerator<T>(xs());
			}

			LazyList<T> Reverse()
			{
				return new ReverseEnumerator<T>(*this);
			}

			//-------------------------------------------------------

			template<typename U>
			LazyList<Pair<T, U>> Pairwise(IEnumerable<U>& remains)
			{
				return new PairwiseEnumerator<T, U>(xs(), remains.CreateEnumerator());
			}

			LazyList<T> Intersect(IEnumerable<T>& remains)
			{
				return new IntersectExceptEnumerator<T, true>(xs(), remains);
			}

			LazyList<T> Except(IEnumerable<T>& remains)
			{
				return new IntersectExceptEnumerator<T, false>(xs(), remains);
			}

			LazyList<T> Union(IEnumerable<T>& remains)
			{
				return Concat(remains).Distinct();
			}
		};

		template<typename T>
		LazyList<T> From(const IEnumerable<T>& enumerable)
		{
			return enumerable;
		}

		template<typename T>
		LazyList<T> From(const T* begin, const T* end)
		{
			return FromPointer(begin, end);
		}

		template<typename T, int size>
		LazyList<T> From(T (&items)[size])
		{
			return FromArray(items);
		}

		template<typename T, int size>
		LazyList<T> From(const T (&items)[size])
		{
			return FromArray(items);
		}
	}
}

#endif