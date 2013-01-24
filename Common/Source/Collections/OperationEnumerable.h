/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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

		template<typename T>
		class Enumerable;

/***********************************************************************
�յ�����
***********************************************************************/

		template<typename T>
		class EmptyEnumerable : public Object, public IEnumerable<T>
		{
			friend class Enumerable<T>;
		private:
			class Enumerator : public Object, public IEnumerator<T>
			{
				IEnumerator<T>* Clone()const
				{
					return new Enumerator;
				}

				const T& Current()const
				{
					return *(T*)0;
				}

				vint Index()const
				{
					return -1;
				}

				bool Next()
				{
					return false;
				}

				void Reset()
				{
				}
			};
		public:
			IEnumerator<T>* CreateEnumerator()const
			{
				return new Enumerator;
			}
		};

/***********************************************************************
�԰���������
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

			IEnumerator<T>* Clone()const
			{
				return new ContainerEnumerator(container, index);
			}

			const T& Current()const
			{
				return container->Get(index);
			}

			vint Index()const
			{
				return index;
			}

			bool Next()
			{
				index++;
				return index>=0 && index<container->Count();
			}

			void Reset()
			{
				index=-1;
			}
		};

/***********************************************************************
�������Ƚ�
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