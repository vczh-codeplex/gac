/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONSELECT
#define VCZH_COLLECTIONS_OPERATIONSELECT

#include "OperationEnumerable.h"
#include "..\Function.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
Select
***********************************************************************/

		template<typename T, typename K>
		class SelectEnumerator : public virtual IEnumerator<K>
		{
		protected:
			IEnumerator<T>*		enumerator;
			Func<K(T)>			selector;
			K					current;
		public:
			SelectEnumerator(IEnumerator<T>* _enumerator, const Func<K(T)>& _selector)
				:enumerator(_enumerator)
				,selector(_selector)
			{
			}

			~SelectEnumerator()
			{
				delete enumerator;
			}

			IEnumerator<K>* Clone()const
			{
				return new SelectEnumerator(enumerator->Clone(), selector);
			}

			const K& Current()const
			{
				return current;
			}

			vint Index()const
			{
				return enumerator->Index();
			}

			bool Next()
			{
				if(enumerator->Next())
				{
					current=selector(enumerator->Current());
					return true;
				}
				else
				{
					return false;
				}
			}

			void Reset()
			{
				enumerator->Reset();
			}
		};
	}
}

#endif