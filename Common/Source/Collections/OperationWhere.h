/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONWHERE
#define VCZH_COLLECTIONS_OPERATIONWHERE

#include "OperationSelect.h"

namespace vl
{
	namespace collections_internal
	{
		template<typename T>
		bool WhereNotNull(Ptr<T> pointer)
		{
			return pointer;
		}
	}

	namespace collections
	{

/***********************************************************************
Where
***********************************************************************/

		template<typename T>
		class WhereEnumerable : public EnumerableStore<T>, public virtual IEnumerable<T>
		{
		private:
			class Enumerator : public virtual IEnumerator<T>
			{
			protected:
				IEnumerator<T>*			enumerator;
				Func<bool(T)>			selector;
				vint					index;

			public:
				Enumerator(IEnumerator<T>* _enumerator, const Func<bool(T)>& _selector, vint _index=-1)
					:enumerator(_enumerator)
					,selector(_selector)
					,index(_index)
				{
				}

				~Enumerator()
				{
					delete enumerator;
				}

				IEnumerator<T>* Clone()const
				{
					return new Enumerator(enumerator->Clone(), selector, index);
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
					while(enumerator->Next())
					{
						if(selector(enumerator->Current()))
						{
							index++;
							return true;
						}
					}
					return false;
				}

				void Reset()
				{
					enumerator->Reset();
				}
			};
		protected:
			Func<bool(T)>			selector;
		public:
			WhereEnumerable(const IEnumerable<T>& enumerable, const Func<bool(T)>& _selector)
				:EnumerableStore<T>(enumerable)
				,selector(_selector)
			{
			}

			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator(CopyEnumerator(), selector);
			}
		};

		template<typename T>
		class WhereProcessor : public EnumerableProcessor<T, WhereEnumerable<T>>
		{
		protected:
			Func<bool(T)>			selector;
		public:
			WhereProcessor(const Func<bool(T)>& _selector)
				:selector(_selector)
			{
			}

			WhereEnumerable<T> operator()(const IEnumerable<T>& enumerable)const
			{
				return WhereEnumerable<T>(enumerable, selector);
			}
		};

		template<typename T>
		WhereProcessor<T> Where(const Func<bool(T)>& selector)
		{
			return WhereProcessor<T>(selector);
		}

		template<typename T>
		WhereProcessor<T> Where(bool(*selector)(T))
		{
			return WhereProcessor<T>(selector);
		}

		template<typename T, typename K>
		class FindTypeProcessor : public EnumerableProcessor<Ptr<T>, WhereEnumerable<Ptr<K>>>
		{
		public:
			WhereEnumerable<Ptr<K>> operator()(const IEnumerable<Ptr<T>>& enumerable)const
			{
				return enumerable>>Cast<T, K>()>>Where<Ptr<K>>(&collections_internal::WhereNotNull);
			}
		};

		template<typename T, typename K>
		FindTypeProcessor<T, K> FindType()
		{
			return FindTypeProcessor<T, K>();
		}
	}
}

#endif