/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONORDERBY
#define VCZH_COLLECTIONS_OPERATIONORDERBY

#include "OperationEnumerable.h"
#include "OperationCopyFrom.h"
#include "..\Function.h"
#include "List.h"

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
OrderBy
***********************************************************************/

		template<typename T>
		class OrderByEnumerable : public virtual IEnumerable<T>
		{
		protected:
			List<T>					values;
		public:
			OrderByEnumerable(const IEnumerable<T>& enumerable, const Func<vint(T, T)>& orderer)
			{
				CopyFrom(values.Wrap(), enumerable);
				if(values.Count()>0)
				{
					Sort(&values[0], values.Count(), orderer);
				}
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return values.Wrap().CreateEnumerator();
			}
		};

		template<typename T>
		class OrderByProcessor : public EnumerableProcessor<T, OrderByEnumerable<T>>
		{
		protected:
			Func<vint(T, T)>		orderer;
		public:
			OrderByProcessor(const Func<vint(T, T)>& _orderer)
				:orderer(_orderer)
			{
			}

			OrderByEnumerable<T> operator()(const IEnumerable<T>& enumerable)const
			{
				return OrderByEnumerable<T>(enumerable, orderer);
			}
		};

		template<typename T>
		OrderByProcessor<T> OrderBy(const Func<vint(T, T)>& orderer)
		{
			return OrderByProcessor<T>(orderer);
		}

		template<typename T>
		OrderByProcessor<T> OrderBy(vint(*orderer)(T, T))
		{
			return OrderByProcessor<T>(orderer);
		}
	}
}

#endif