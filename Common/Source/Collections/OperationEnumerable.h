/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Data Structure::Operations

***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATIONENUMERABLE
#define VCZH_COLLECTIONS_OPERATIONENUMERABLE

#include "Interfaces.h"
#include "..\Pointer.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
空迭代器
***********************************************************************/

		template<typename T>
		class EmptyEnumerable : public Object, public IEnumerable<T>
		{
		private:
			class Enumerator : public Object, public virtual IEnumerator<T>
			{
				IEnumerator<T>* Clone()const override
				{
					return new Enumerator;
				}

				const T& Current()const override
				{
					return *(T*)0;
				}

				vint Index()const override
				{
					return -1;
				}

				bool Next()override
				{
					return false;
				}

				void Reset()override
				{
				}

				bool Evaluated()const override
				{
					return true;
				}
			};
		public:
			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator;
			}
		};

/***********************************************************************
自包含迭代器
***********************************************************************/

		template<typename T, typename TContainer>
		class ContainerEnumerator : public Object, public virtual IEnumerator<T>
		{
		private:
			Ptr<TContainer>					container;
			vint							index;

		public:
			ContainerEnumerator(Ptr<TContainer> _container, vint _index=-1)
			{
				container=_container;
				index=_index;
			}

			IEnumerator<T>* Clone()const override
			{
				return new ContainerEnumerator(container, index);
			}

			const T& Current()const override
			{
				return container->Get(index);
			}

			vint Index()const override
			{
				return index;
			}

			bool Next()override
			{
				index++;
				return index>=0 && index<container->Count();
			}

			void Reset()override
			{
				index=-1;
			}

			bool Evaluated()const override
			{
				return true;
			}
		};

/***********************************************************************
迭代器比较
***********************************************************************/

		template<typename T, typename U>
		vint CompareEnumerable(const IEnumerable<T>& a, const IEnumerable<U>& b)
		{
			Ptr<IEnumerator<T>> ator=a.CreateEnumerator();
			Ptr<IEnumerator<U>> btor=b.CreateEnumerator();
			while()
			{
				bool a=ator->Next();
				bool b=btor->Next();
				if(a&&!b) return 1;
				if(!a&&b) return -1;
				if(!a&&!b) break;

				const T& ac=ator->Current();
				const U& bc=btor->Current();
				if(ac<bc)
				{
					return -1;
				}
				else if(ac>bc)
				{
					return 1;
				}
				ator->Next();
				btor->Next();
			}
			return 0;
		}
	}
}

#endif